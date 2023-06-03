// Copyright (c) 2023 Petter Nilsson. MIT License. https://github.com/pettni/ezconfig

/**
 * @file json.hpp
 * @brief Json factory.
 */

#pragma once

#include <nlohmann/json.hpp>

#include "factory.hpp"
#include "macro.hpp"

namespace ezconfig {

// @brief Argument for yaml factory.
using JsonArg = const nlohmann::json &;

// clang-format off
template<typename T>
concept JsonParseable = requires(JsonArg j) {
  {j.get<T>()} -> std::convertible_to<T>;
};
// clang-format on

/**
 * @brief A JsonFactory creates objects in a class hierarchy from json.
 */
template<typename Base>
class JsonFactory : public Factory<Base, JsonArg>
{
public:
  using FactoryBase = Factory<Base, JsonArg>;

  /**
   * @brief Register a factory method.
   *
   * @tparam Derived sub-class of Base.
   * @tparam Intermediate type that is parseable from json, and that can construct Derived.
   *
   * Adds conversions from json of the form '{ tag: data }' to Derived,
   * where 'tag' is a string and 'data' can be json-converted to Intermediate.
   */
  template<typename Derived, typename Intermediate = Derived>
    requires(
      std::is_base_of_v<Base, Derived> && JsonParseable<Intermediate>
      && std::is_constructible_v<Derived, Intermediate &&>)
  static void Register(const std::string & tag)
  {
    auto creator = [](JsonArg json) {
      auto interm = json.get<Intermediate>();
      return std::make_unique<Derived>(std::move(interm));
    };
    FactoryBase::Register(tag, std::move(creator));
  }

  /**
   * @brief Create an object from json.
   */
  static std::unique_ptr<Base> Create(JsonArg json)
  {
    if (!json.is_object() || json.size() != 1) {
      throw std::logic_error("Expected dictionary of size 1 of format {tag: object}");
    }
    return FactoryBase::Create(json.begin().key(), json.begin().value());
  }
};

}  // namespace ezconfig

/**
 * @brief Declare a json factory for a base class.
 *
 * @param Base factory base class.
 *
 * The factory creates pointers to Base.
 *
 * Example: Declare a \a MyBase json factory.
 * @code
 * EZ_JSON_DECLARE(MyBase);
 * @endcode
 */
#define EZ_JSON_DECLARE(Base) EZ_DECLARE(Base, ezconfig::JsonArg)

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
  EZ_STATIC_INVOKE(ezconfig::JsonFactory<Base>::Register<Derived __VA_OPT__(, ) __VA_ARGS__>, tag)

/**
 * @brief Create an object using the factory.
 *
 * @param Base factory base class
 * @param j json data
 *
 * @code
 * EZ_JSON_CREATE(MyBase, nlohmann::json::parse(data));
 * @endcode
 */
#define EZ_JSON_CREATE(Base, j) ezconfig::JsonFactory<Base>::Create(j)

/**
 * @brief Converter json -> std::shared_ptr<Base> using JsonFactory<Base>.
 */
template<typename Base>
struct nlohmann::adl_serializer<std::shared_ptr<Base>>
{
  static void from_json(const json & j, std::shared_ptr<Base> & ptr) { ptr = ezconfig::JsonFactory<Base>::Create(j); };
};

/**
 * @brief Converter json -> std::unique_ptr<Base> using JsonFactory<Base>.
 */
template<typename Base>
struct nlohmann::adl_serializer<std::unique_ptr<Base>>
{
  static void from_json(const json & j, std::unique_ptr<Base> & ptr) { ptr = ezconfig::JsonFactory<Base>::Create(j); };
};
