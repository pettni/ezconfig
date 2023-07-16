// Copyright (c) 2023 Petter Nilsson. MIT License. https://github.com/pettni/ezconfig

/**
 * @file yaml_fwd.hpp
 * @brief Yaml factory forward declarations.
 *
 * Include this file in the header for the base class.
 */

#pragma once

#include "factory.hpp"
#include "global.hpp"
#include "macro.hpp"

/// YAML forward declarations.
namespace YAML {
class Node;
template<typename T>
struct convert;
};  // namespace YAML

namespace ezconfig::yaml {

/**
 * @brief Create an object from yaml using the global factory.
 *
 * @tparam Base factory base class
 *
 * @param y yaml data
 *
 * @code
 * auto obj = yaml::Create<MyBase>(YAML::Load(data));
 * @endcode
 */
template<typename Base>
std::unique_ptr<Base> Create(const YAML::Node & y);

}  // namespace ezconfig::yaml

/**
 * @brief Declare a global yaml factory for a base class.
 *
 * Do this in the base class header file.
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
#define EZ_YAML_DECLARE(Base) EZ_FACTORY_DECLARE(Base, const YAML::Node &)

/**
 * @brief Converter yaml -> std::shared_ptr<Base> using YamlFactory<Base>.
 */
template<typename Base>
struct YAML::convert<std::shared_ptr<Base>>
{
  static bool decode(const YAML::Node & y, std::shared_ptr<Base> & ptr);
};

/**
 * @brief Converter yaml -> std::unique_ptr<Base> using YamlFactory<Base>.
 */
template<typename Base>
struct YAML::convert<std::unique_ptr<Base>>
{
  static bool decode(const YAML::Node & y, std::unique_ptr<Base> & ptr);
};
