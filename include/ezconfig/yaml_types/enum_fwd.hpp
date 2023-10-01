// Copyright (c) 2023 Petter Nilsson. MIT License. https://github.com/pettni/ezconfig

#pragma once

#include <type_traits>

namespace ezconfig {

template<typename T>
concept ScopedEnum = requires { std::is_enum_v<T> && !std::is_convertible_v<T, std::underlying_type_t<T>>; };

}  // namespace ezconfig

namespace YAML {

// forward declarations
template<typename T>
struct convert;

class Node;

/**
 * @brief Decode an enum from yaml.
 */
template<ezconfig::ScopedEnum T>
struct convert<T>
{
  static bool decode(const Node & yaml, T & obj);
};

}  // namespace YAML
