// Copyright (c) 2023 Petter Nilsson. MIT License. https://github.com/pettni/ezconfig

#pragma once

#include <Eigen/Core>
#include <Eigen/Geometry>

namespace YAML {

// forward declarations
template<typename T>
struct convert;

class Node;

/**
 * @brief Decode a fixed-size Eigen vector from yaml.
 *
 * The YAML representation is a list, e.g. "[1, 2, 3]",
 * or, for vectors of size at most 3, a dictionary like
 * "{x: 1, y: 2, z: 3}".
 */
template<typename T, int Rows, int Cols, int Opts>
struct convert<Eigen::Matrix<T, Rows, Cols, Opts>>
{
  static bool decode(const Node & yaml, Eigen::Matrix<T, Rows, Cols, Opts> & obj);
};

/**
 * @brief Decode a Eigen quaternion from yaml.
 *
 * The YAML representation is a map with keys qw, qx, qy, qz.
 */
template<typename T, int Opts>
struct convert<Eigen::Quaternion<T, Opts>>
{
  static bool decode(const Node & yaml, Eigen::Quaternion<T, Opts> & obj);
};

}  // namespace YAML
