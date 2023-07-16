// Copyright (c) 2023 Petter Nilsson. MIT License. https://github.com/pettni/ezconfig

/**
 * @file yaml.hpp
 * @brief Yaml factory.
 *
 * Include this file in implementation file for the base class and any derived classes.
 */

#pragma once

#include <yaml-cpp/yaml.h>

#include "factory.hpp"
#include "yaml_fwd.hpp"

/**
 * @brief Define a global yaml factory for a base class.
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
#define EZ_YAML_DEFINE(Base)                                                 \
  EZ_FACTORY_DEFINE(Base, const YAML::Node &);                               \
  template std::unique_ptr<Base> ezconfig::yaml::Create(const YAML::Node &); \
  template struct YAML::convert<std::shared_ptr<Base>>;                      \
  template struct YAML::convert<std::unique_ptr<Base>>

/**
 * @brief Register a conversion method with the global yaml factory.
 *
 * Do this in the implementation files for derived classes.
 *
 * @param Base factory base class.
 * @param tag conversion identifier (string).
 * @param Derived factory derived class.
 * @param Intermediate optional intermediate class.
 *
 * @note Intermediate can be omitted. In that case Derived must be yaml-parseable.
 * @note If provided, Intermediate must be a yaml-parsable type.
 *
 * Example: Register a creator for \a MyDerived with tag "!mytag".
 * @code
 * EZ_YAML_REGISTER(MyBase, "!mytag", MyDerived, MyDerivedConfig);
 * @endcode
 */
#define EZ_YAML_REGISTER(Base, tag, Derived, ...) \
  EZ_STATIC_INVOKE(&ezconfig::yaml::Add<Base, Derived __VA_OPT__(, ) __VA_ARGS__>, tag)

namespace ezconfig::yaml {

// clang-format off
template<typename T>
concept YamlParseable = requires(const YAML::Node & y) {
  {y.as<T>()} -> std::convertible_to<T>;
};
// clang-format on

/**
 * @brief Add a factory method.
 *
 * @tparam Derived sub-class of Base.
 * @tparam Intermediate type that is parseable from yaml, and that can construct Derived.
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
    std::is_base_of_v<Base, Derived> && YamlParseable<Intermediate>
    && std::is_constructible_v<Derived, Intermediate &&>)
void Add(const std::string & tag)
{
  if (tag.size() < 2 || tag[0] != '!') { throw std::logic_error("yaml tag must start with !"); }
  auto creator = [](const YAML::Node & y) { return std::make_unique<Derived>(y.as<Intermediate>()); };
  EZ_FACTORY_INSTANCE(Base, const YAML::Node &).add(tag, std::move(creator));
}

template<typename Base>
std::unique_ptr<Base> Create(const YAML::Node & y)
{
  return EZ_FACTORY_INSTANCE(Base, const YAML::Node &).create(y.Tag(), y);
}

}  // namespace ezconfig::yaml

template<typename Base>
bool YAML::convert<std::shared_ptr<Base>>::decode(const YAML::Node & y, std::shared_ptr<Base> & ptr)
{
  ptr = ::ezconfig::yaml::Create<Base>(y);
  return true;
}

template<typename Base>
bool YAML::convert<std::unique_ptr<Base>>::decode(const YAML::Node & y, std::unique_ptr<Base> & ptr)
{
  ptr = ::ezconfig::yaml::Create<Base>(y);
  return true;
}
