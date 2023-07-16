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

template<typename T>
class variant_tags;

template<typename... Ts>
class variant_tags<std::variant<Ts...>>
{
public:
  template<typename T>
    requires(std::is_same_v<T, Ts> || ...)
  static void Add(const std::string & tag)
  {
    const auto idx    = ezconfig::index_in_typepack<T, Ts...>();
    s_tag_to_idx[tag] = idx;
    s_idx_to_tag[idx] = tag;
  }

  static std::variant<Ts...> TagToType(const std::string & tag)
  {
    auto idx = s_tag_to_idx.at(tag);
    if (idx == 0) { return std::variant<Ts...>{std::in_place_index<0>}; }
    if (idx == 1) { return std::variant<Ts...>{std::in_place_index<1>}; }
    throw std::runtime_error("WIP");
  }

  template<typename T>
  static std::string TypeToTag()
  {
    const auto idx = ezconfig::index_in_typepack<T, Ts...>();
    return s_idx_to_tag.at(idx);
  }

private:
  static std::map<std::string, std::size_t> s_tag_to_idx;
  static std::map<std::size_t, std::string> s_idx_to_tag;
};

template<typename... Ts>
std::map<std::string, std::size_t> variant_tags<std::variant<Ts...>>::s_tag_to_idx{};
template<typename... Ts>
std::map<std::size_t, std::string> variant_tags<std::variant<Ts...>>::s_idx_to_tag{};

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
  static Node encode(const std::variant<Ts...> & rhs);
};

}  // namespace YAML
