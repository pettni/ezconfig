// Copyright (c) 2023 Petter Nilsson. MIT License. https://github.com/pettni/ezconfig

/**
 * @file yaml.hpp
 * @brief Yaml factory.
 */

#pragma once

#include <yaml-cpp/yaml.h>

#include "factory.hpp"
#include "macro.hpp"

namespace ezconfig {

// @brief Argument for yaml factory.
using YamlArg = const YAML::Node &;

// clang-format off
template<typename T>
concept YamlParseable = requires(YamlArg y) {
  {y.as<T>()} -> std::convertible_to<T>;
};
// clang-format on

/**
 * @brief A YamlFactory creates objects in a class hierarchy from yaml.
 */
template<typename Base>
class YamlFactory : public Factory<Base, YamlArg>
{
public:
  using FactoryBase = Factory<Base, YamlArg>;

  /**
   * @brief Register a factory method.
   *
   * @tparam Derived sub-class of Base.
   * @tparam Intermediate type that is parseable from yaml, and that can construct Derived.
   *
   * Adds conversions from yaml of the form
   *
   * !tag
   * object
   *
   * to Derived, where object can be yaml-converted to Intermediate.
   */
  template<typename Derived, typename Intermediate = Derived>
    requires(
      std::is_base_of_v<Base, Derived> && YamlParseable<Intermediate>
      && std::is_constructible_v<Derived, Intermediate &&>)
  static void Register(const std::string & tag)
  {
    if (tag.size() < 2 || tag[0] != '!') { throw std::logic_error("yaml tag must start with !"); }
    auto creator = [](YamlArg y) { return std::make_unique<Derived>(y.as<Intermediate>()); };
    FactoryBase::Register(tag, std::move(creator));
  }

  /**
   * @brief Create an object from yaml.
   */
  static std::unique_ptr<Base> Create(YamlArg y) { return FactoryBase::Create(y.Tag(), y); }
};

}  // namespace ezconfig

/**
 * @brief Declare a yaml factory for a base class.
 *
 * @param Base factory base class.
 *
 * The factory creates pointers to Base.
 *
 * Example: Declare a \a MyBase yaml factory.
 * @code
 * EZ_YAML_DECLARE(MyBase);
 * @endcode
 */
#define EZ_YAML_DECLARE(Base) EZ_DECLARE(Base, ezconfig::YamlArg)

/**
 * @brief Register a tagged conversion with a yaml factory.
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
 * EZ_YAML_REGISTER(MyBase, "mytag", MyDerived, MyDerivedConfig);
 * @endcode
 */
#define EZ_YAML_REGISTER(Base, tag, Derived, ...) \
  EZ_STATIC_INVOKE(ezconfig::YamlFactory<Base>::Register<Derived __VA_OPT__(, ) __VA_ARGS__>, tag)

/**
 * @brief Create an object from yaml..
 *
 * @param Base factory base class
 * @param y yaml data
 *
 * @code
 * EZ_YAML_CREATE(MyBase, YAML::Load(data));
 * @endcode
 */
#define EZ_YAML_CREATE(Base, y) ezconfig::YamlFactory<Base>::Create(y)

/**
 * @brief Converter yaml -> std::shared_ptr<Base> using YamlFactory<Base>.
 */
template<typename Base>
struct YAML::convert<std::shared_ptr<Base>>
{
  static bool decode(const YAML::Node & y, std::shared_ptr<Base> & ptr)
  {
    ptr = ezconfig::YamlFactory<Base>::Create(y);
    return true;
  }
};

/**
 * @brief Converter yaml -> std::unique_ptr<Base> using YamlFactory<Base>.
 */
template<typename Base>
struct YAML::convert<std::unique_ptr<Base>>
{
  static bool decode(const YAML::Node & y, std::unique_ptr<Base> & ptr)
  {
    ptr = ezconfig::YamlFactory<Base>::Create(y);
    return true;
  }
};
