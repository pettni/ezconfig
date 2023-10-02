// Copyright (c) 2023 Petter Nilsson. MIT License. https://github.com/pettni/ezconfig

#pragma once

#include <smooth/se2.hpp>
#include <smooth/se3.hpp>
#include <smooth/so2.hpp>
#include <smooth/so3.hpp>

namespace YAML {

// forward declarations
template<typename T>
struct convert;

class Node;

/**
 * @brief Decode so2 from yaml.
 *
 * Supported formats
 *
 * Format 1: <floating>   [angle in radians]
 *
 * Format 2:
 *   qw: <floating>
 *   qz: <floating>
 *
 * Format 3:
 *   w: <floating>
 *   z: <floating>
 */
template<typename T>
struct convert<smooth::SO2<T>>
{
  static bool decode(const Node & yaml, smooth::SO2<T> & obj);
};

/**
 * @brief Decode se2 from yaml.
 *
 * Supported formats
 *
 * Format 1:
 *   x: <floating>
 *   y: <floating>
 *   yaw: <floating>  [radians]
 *
 * Format 2:
 *   x: <floating>
 *   y: <floating>
 *   qw: <floating>
 *   qz: <floating>
 *
 * Format 3:
 *   translation: <vec2>
 *   orientation: <so2>
 */
template<typename T>
struct convert<smooth::SE2<T>>
{
  static bool decode(const Node & yaml, smooth::SE2<T> & obj);
};

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
