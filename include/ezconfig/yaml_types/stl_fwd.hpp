// Copyright (c) 2023 Petter Nilsson. MIT License. https://github.com/pettni/ezconfig

#pragma once

#include <filesystem>
#include <map>
#include <optional>
#include <unordered_map>
#include <variant>
#include <vector>

#include "../meta.hpp"

namespace YAML {

// forward declarations
template<typename T>
struct convert;

class Node;

/**
 * @brief Convert std::optional<> to/from yaml
 *
 * A yaml "null" value is mapped to std::nullopt.
 */
template<typename T>
struct convert<std::optional<T>>
{
  static bool decode(const Node & yaml, std::optional<T> & obj);
  static Node encode(const std::optional<T> & rhs);
};

/**
 * @brief Convert a std::unordered_map to/from yaml.
 */
template<typename K, typename V, typename C, typename A>
struct convert<std::unordered_map<K, V, C, A>>
{
  static bool decode(const Node & yaml, std::unordered_map<K, V, C, A> & obj);
  static Node encode(const std::unordered_map<K, V, C, A> & rhs);
};

/**
 * @brief Convert a path to/from yaml.
 */
template<>
struct convert<std::filesystem::path>
{
  static bool decode(const Node & yaml, std::filesystem::path & obj);
  static Node encode(const std::filesystem::path & rhs);
};

/**
 * @brief Convert a chrono type to/from yaml.
 *
 * The yaml representation is e.g. "40ms" or "100us". The supported suffixes are
 * - h: hours
 * - m: minutes
 * - s: seconds
 * - ms: milliseconds
 * - us: microseconds
 * - ns: nanoseconds
 */
template<intmax_t Num, intmax_t Den>
struct convert<std::chrono::duration<int64_t, std::ratio<Num, Den>>>
{
  static bool decode(const Node & yaml, std::chrono::duration<int64_t, std::ratio<Num, Den>> & obj);
  static Node encode(const std::chrono::duration<int64_t, std::ratio<Num, Den>> & rhs);
};

}  // namespace YAML
