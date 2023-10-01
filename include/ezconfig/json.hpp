// Copyright (c) 2023 Petter Nilsson. MIT License. https://github.com/pettni/ezconfig

/**
 * @file json.hpp
 * @brief Json factory.
 *
 * Include this file in implementation file for the base class and any derived classes.
 */

#pragma once

#include <nlohmann/json.hpp>

#include "factory.hpp"
#include "json_fwd.hpp"

/**
 * @brief Define a global json factory for a base class.
 *
 * Do this in the base class implementation file.
 *
 * @param Base factory base class.
 *
 * Example: Define a \a MyBase yaml factory.
 * @code
 * EZ_YAML_DEFINE(MyBase);
 * @endcode
 */
#define EZ_JSON_DEFINE(Base)                                                           \
  EZ_FACTORY_DEFINE(Base, const nlohmann::json &);                                     \
  template std::unique_ptr<Base> ezconfig::json::Create<Base>(const nlohmann::json &); \
  template struct nlohmann::adl_serializer<std::shared_ptr<Base>>;                     \
  template struct nlohmann::adl_serializer<std::unique_ptr<Base>>

/**
 * @brief Register a tagged conversion with a json factory.
 *
 * Do this in the implementation files for derived classes.
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

/**
 * @brief Add a factory method.
 *
 * @tparam Derived sub-class of Base.
 * @tparam Intermediate type that is parseable from json, and that can construct Derived.
 *
 * Adds conversions from yaml of the form
 *
 * !tag
 * object
 *
 * to Derived, where object is yaml-converted to Intermediate.
 */
template<typename Base, typename Derived, typename Intermediate = Derived>
  requires(
    std::is_base_of_v<Base, Derived> && JsonParseable<Intermediate>
    && std::is_constructible_v<Derived, Intermediate &&>)
void Add(const std::string & tag)
{
  auto creator = [](const nlohmann::json & json) { return std::make_unique<Derived>(json.get<Intermediate>()); };
  EZ_FACTORY_INSTANCE(Base, const nlohmann::json &).add(tag, std::move(creator));
}

template<typename Base>
std::unique_ptr<Base> Create(const nlohmann::json & json)
{
  if (!json.is_object() || json.size() != 1) {
    throw std::logic_error("Expected dictionary of size 1 of format {tag: object}");
  }
  return EZ_FACTORY_INSTANCE(Base, const nlohmann::json &).create(json.begin().key(), json.begin().value());
}

}  // namespace ezconfig::json

template<ezconfig::json::Constructible Base>
void nlohmann::adl_serializer<std::shared_ptr<Base>>::from_json(const json & j, std::shared_ptr<Base> & ptr)
{
  ptr = ::ezconfig::json::Create<Base>(j);
}

template<ezconfig::json::Constructible Base>
void nlohmann::adl_serializer<std::unique_ptr<Base>>::from_json(const json & j, std::unique_ptr<Base> & ptr)
{
  ptr = ::ezconfig::json::Create<Base>(j);
}
