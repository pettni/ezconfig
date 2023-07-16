// Copyright (c) 2023 Petter Nilsson. MIT License. https://github.com/pettni/ezconfig

#pragma once

#include <boost/hana/concept/struct.hpp>

namespace YAML {

// forward declarations
template<typename T>
struct convert;

class Node;

/**
 * @brief Decode a boost::hana struct from yaml.
 */
template<typename T>
  requires(boost::hana::Struct<T>::value)
struct convert<T>
{
  static bool decode(const Node & yaml, T & t);
};

}  // namespace YAML
