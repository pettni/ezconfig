// Copyright (c) 2023 Petter Nilsson. MIT License. https://github.com/pettni/ezconfig

/**
 * @file json.hpp
 * @brief Json factory.
 */

#pragma once

#include <nlohmann/json.hpp>

#include "factory.hpp"
#include "json_fwd.hpp"

#define EZ_JSON_DEFINE(Base)                                                           \
  EZ_FACTORY_DEFINE(Base, const nlohmann::json &);                                     \
  template std::unique_ptr<Base> ezconfig::json::Create<Base>(const nlohmann::json &); \
  template struct nlohmann::adl_serializer<std::shared_ptr<Base>>;                     \
  template struct nlohmann::adl_serializer<std::unique_ptr<Base>>

/**
 * @brief Register a tagged conversion with a json factory.
 *
 * @param Base factory base class.
 * @param tag conversion identifier (string).
 * @param Derived factory derived class.
 * @param Intermediate optional intermediate class.
 *
 * @note Intermediate can be omitted. In that case Derived must be json-parseable.
 * @note If provided, Intermediate must be a json-parsable type.
 *
 * Example: Register a creator for \a MyDerived with tag "mytag".
 * @code
 * EZ_JSON_REGISTER(MyBase, "mytag", MyDerived, MyDerivedConfig);
 * @endcode
 */
#define EZ_JSON_REGISTER(Base, tag, Derived, ...) \
  EZ_STATIC_INVOKE(&ezconfig::json::Add<Base, Derived __VA_OPT__(, ) __VA_ARGS__>, tag)

namespace ezconfig::json {

// clang-format off
template<typename T>
concept JsonParseable = requires(const nlohmann::json & j) {
  {j.get<T>()} -> std::convertible_to<T>;
};
// clang-format on

template<typename Base>
std::unique_ptr<Base> Create(const nlohmann::json & json)
{
  if (!json.is_object() || json.size() != 1) {
    throw std::logic_error("Expected dictionary of size 1 of format {tag: object}");
  }
  return EZ_FACTORY_INSTANCE(Base, const nlohmann::json &).create(json.begin().key(), json.begin().value());
}

/**
 * @brief A JsonFactory creates objects in a class hierarchy from json.
 */
template<typename Base, typename Derived, typename Intermediate>
  requires(std::is_base_of_v<Base, Derived> && std::is_constructible_v<Derived, Intermediate &&>)
void Add(const std::string & tag)
{
  static_assert(JsonParseable<Intermediate>, "Intermediate must be json-parseable!");
  auto creator = [](const nlohmann::json & json) {
    auto interm = json.get<Intermediate>();
    return std::make_unique<Derived>(std::move(interm));
  };
  EZ_FACTORY_INSTANCE(Base, const nlohmann::json &).add(tag, std::move(creator));
}

}  // namespace ezconfig::json

template<typename Base>
void nlohmann::adl_serializer<std::shared_ptr<Base>>::from_json(const json & j, std::shared_ptr<Base> & ptr)
{
  ptr = ::ezconfig::json::Create<Base>(j);
}

template<typename Base>
void nlohmann::adl_serializer<std::unique_ptr<Base>>::from_json(const json & j, std::unique_ptr<Base> & ptr)
{
  ptr = ::ezconfig::json::Create<Base>(j);
}
