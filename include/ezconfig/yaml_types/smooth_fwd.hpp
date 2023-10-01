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
 *
 * Supported formats: same as Eigen quaternion.
 */
template<typename T>
struct convert<smooth::SO3<T>>
{
  static bool decode(const Node & yaml, smooth::SO3<T> & obj);
};

/**
 * @brief Decode se3 object from yaml.
 *
 * Supported formats:
 *
 * Format 1:
 *
 *   translation: <vec3>
 *   orientation: <so3>
 *
 * Format 2:
 *   x: <floating>
 *   y: <floating>
 *   z: <floating>
 *   qw: <floating>
 *   qz: <floating>
 *   qy: <floating>
 *   qz: <floating>
 */
template<typename T>
struct convert<smooth::SE3<T>>
{
  static bool decode(const Node & yaml, smooth::SE3<T> & obj);
};

}  // namespace YAML
