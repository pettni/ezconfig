// Copyright (c) 2023 Petter Nilsson. MIT License. https://github.com/pettni/ezconfig

#pragma once

#include <filesystem>
#include <optional>
#include <unordered_map>
#include <variant>

namespace YAML {

// forward declarations
template<typename T>
struct convert;

class Node;

/**
 * @brief Decode std::optional<> from yaml
 *
 * A yaml "null" value is mapped to std::nullopt.
 */
template<typename T>
struct convert<std::optional<T>>
{
  static bool decode(const Node & yaml, std::optional<T> & obj);
};

/**
 * @brief Decode a std::unordered_map<string, T> from yaml.
 */
template<typename T>
struct convert<std::unordered_map<std::string, T>>
{
  static bool decode(const Node & yaml, std::unordered_map<std::string, T> & obj);
};

/**
 * @brief Decode a path from yaml.
 */
template<>
struct convert<std::filesystem::path>
{
  static bool decode(const Node & yaml, std::filesystem::path & obj);
};

/**
 * @brief Decode a chrono type from yaml.
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
};

/**
 * @brief Type trait for std::variant<> yaml decoding
 *
 * Specializations should define a member "value" that maps
 * yaml tags to type instantiations.
 *
 * Example:
 * @code
 * using MyVariant = std::variant<double, string, int>;
 * // Specialize type trait for MyVariant type
 * template<>
 * struct YAML::variant_type_names<MyVariant>
 * {
 *   inline static const std::unordered_map<std::string, MyVariant> value{
 *     {"!double", double{}},
 *     {"!string", std::string{}},
 *     {"!int", int{}},
 *   };
 * };
 * @endcode
 */
template<typename T>
struct variant_type_names;

/**
 * @brief Decode std::variant<> from yaml.
 *
 * variant_type_names must be specialized for the <std::variant<Ts...>>, and the
 * yaml must contain a tag that matches a key in variant_type_names<std::variant<Ts...>>.
 */
template<typename... Ts>
struct convert<std::variant<Ts...>>
{
  static bool decode(const Node & yaml, std::variant<Ts...> & obj);
};

}  // namespace YAML
