// Copyright (c) 2023 Petter Nilsson. MIT License. https://github.com/pettni/ezconfig

#pragma once

#include <yaml-cpp/yaml.h>

#include "eigen.hpp"
#include "smooth_fwd.hpp"

namespace YAML {

template<typename T>
bool convert<smooth::SO2<T>>::decode(const Node & yaml, smooth::SO2<T> & obj)
{
  if (yaml.IsMap()) {
    if (yaml["qz"]) {
      obj = smooth::SO2<T>(yaml["qz"].template as<T>(), yaml["qw"].template as<T>());
    } else if (yaml["z"]) {
      obj = smooth::SO2<T>(yaml["z"].template as<T>(), yaml["w"].template as<T>());
    }
    return false;
  } else {
    obj = smooth::SO2<T>(yaml.template as<T>());
  }
  return true;
}

template<typename T>
bool convert<smooth::SE2<T>>::decode(const Node & yaml, smooth::SE2<T> & obj)
{
  if (yaml["qw"] || yaml["w"]) {
    obj.so2() = yaml.as<smooth::SO2<T>>();
    obj.r2()  = yaml.as<Eigen::Vector2<T>>();
  } else if (yaml["yaw"]) {
    obj.so2() = yaml["yaw"].as<smooth::SO2<T>>();
    obj.r2()  = yaml.as<Eigen::Vector2<T>>();
  } else {
    obj.so2() = yaml["orientation"].as<smooth::SO2<T>>();
    obj.r2()  = yaml["translation"].as<Eigen::Vector2<T>>();
  }
  return true;
}

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
