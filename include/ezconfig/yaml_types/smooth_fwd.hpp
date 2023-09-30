// Copyright (c) 2023 Petter Nilsson. MIT License. https://github.com/pettni/ezconfig

#pragma once

#include <smooth/se3.hpp>
#include <smooth/so3.hpp>

namespace YAML {

// forward declarations
template<typename T>
struct convert;

class Node;

/**
 * @brief Decode an enum from yaml.
 */
template<typename T>
struct convert<smooth::SO3<T>>
{
  static bool decode(const Node & yaml, smooth::SO3<T> & obj);
};

/**
 * @brief Decode an enum from yaml.
 */
template<typename T>
struct convert<smooth::SE3<T>>
{
  static bool decode(const Node & yaml, smooth::SE3<T> & obj);
};

}  // namespace YAML
