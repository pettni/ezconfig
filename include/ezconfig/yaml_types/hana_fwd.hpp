// Copyright (c) 2023 Petter Nilsson. MIT License. https://github.com/pettni/ezconfig

#pragma once

#include <variant>

#include <boost/hana/concept/struct.hpp>

namespace ezconfig {

/**
 * @brief Type trait for std::variant<> yaml decoding
 *
 * Specializations should define a member "value" that maps
 * yaml tags to type instantiations.
 *
 * Example:
 * @code
 * #include <boost/hana/tuple.hpp>
 *
 * using MyVariant = std::variant<double, string, int>;
 *
 * // Specialize type trait for MyVariant type
 * template<>
 * struct ezconfig::variant_hana_maps<MyVariant>
 * {
 *   static constexpr auto value = boost::hana::make_tuple(
 *     boost::hana::make_pair("!double", boost::hana::type_c<double>),
 *     boost::hana::make_pair("!int", boost::hana::type_c<int>),
 *     boost::hana::make_pair("!string", boost::hana::type_c<std::string>));
 * };
 * @endcode
 */
template<typename T>
struct variant_hana_maps
{};

}  // namespace ezconfig

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

/**
 * @brief Decode std::variant<> from yaml.
 *
 * @ref ezconfig::variant_hana_maps must be specialized for std::variant<Ts...>, and the
 * yaml must contain a tag that matches a key in the map.
 */
template<typename... Ts>
struct convert<std::variant<Ts...>>
{
  static bool decode(const Node & yaml, std::variant<Ts...> & obj);
};

}  // namespace YAML
