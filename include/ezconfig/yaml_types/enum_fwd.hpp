// Copyright (c) 2023 Petter Nilsson. MIT License. https://github.com/pettni/ezconfig

#pragma once

#include <type_traits>

namespace ezconfig::detail {

template<typename T, bool = std::is_enum_v<T>>
struct is_scoped_enum : std::false_type
{};

template<typename T>
struct is_scoped_enum<T, true> : std::bool_constant<!std::is_convertible_v<T, std::underlying_type_t<T>>>
{};

}  // namespace ezconfig::detail

namespace YAML {

// forward declarations
template<typename T>
struct convert;

class Node;

/**
 * @brief Decode an enum from yaml.
 */
template<typename T>
  requires(ezconfig::detail::is_scoped_enum<T>::value)
struct convert<T>
{
  static bool decode(const Node & yaml, T & obj);
};

}  // namespace YAML
