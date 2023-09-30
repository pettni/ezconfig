// Copyright (c) 2023 Petter Nilsson. MIT License. https://github.com/pettni/ezconfig

#pragma once

#include <yaml-cpp/yaml.h>

#include "eigen.hpp"
#include "smooth_fwd.hpp"

namespace YAML {

template<typename T>
bool convert<smooth::SO3<T>>::decode(const Node & yaml, smooth::SO3<T> & obj)
{
  obj = smooth::SO3<T>(yaml.as<Eigen::Quaternion<T>>());
  return true;
}

template<typename T>
bool convert<smooth::SE3<T>>::decode(const Node & yaml, smooth::SE3<T> & obj)
{
  if (yaml["qw"]) {
    obj.so3() = yaml.as<smooth::SO3<T>>();
    obj.r3()  = yaml.as<Eigen::Vector3<T>>();

  } else {
    obj.so3() = yaml["orientation"].as<smooth::SO3<T>>();
    obj.r3()  = yaml["translation"].as<Eigen::Vector3<T>>();
  }
  return true;
}

}  // namespace YAML
