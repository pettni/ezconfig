// Copyright (c) 2023 Petter Nilsson. MIT License. https://github.com/pettni/ezconfig

/**
 * @file json_fwd.hpp
 * @brief Json factory forward declarations.
 *
 * Include this file in the header for the base class.
 */

#pragma once

#include <nlohmann/json_fwd.hpp>

#include "factory.hpp"
#include "macro.hpp"

namespace ezconfig::json {

/**
 * @brief Create an object using the factory.
 *
 * @tparam Base factory base class
 *
 * @param j json data
 *
 * @code
 * auto obj = JsonCreate<MyBase>(nlohmann::json::parse(data));
 * @endcode
 */
template<typename Base>
std::unique_ptr<Base> Create(const nlohmann::json & j);

}  // namespace ezconfig::json

/**
 * @brief Declare a json factory for a base class.
 *
 * @param Base factory base class.
 *
 * The factory creates pointers to Base.
 *
 * Example: Declare a \a MyBase json factory.
 * @code
 * EZ_JSON_DECLARE(MyBase);
 * @endcode
 */
#define EZ_JSON_DECLARE(Base) EZ_FACTORY_DECLARE(Base, const nlohmann::json &)

/**
 * @brief Converter json -> std::shared_ptr<Base> using JsonFactory<Base>.
 */
template<typename Base>
struct nlohmann::adl_serializer<std::shared_ptr<Base>>
{
  static void from_json(const json & j, std::shared_ptr<Base> & ptr);
};

/**
 * @brief Converter json -> std::unique_ptr<Base> using JsonFactory<Base>.
 */
template<typename Base>
struct nlohmann::adl_serializer<std::unique_ptr<Base>>
{
  static void from_json(const json & j, std::unique_ptr<Base> & ptr);
};
