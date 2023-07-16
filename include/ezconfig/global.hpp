// Copyright (c) 2023 Petter Nilsson. MIT License. https://github.com/pettni/ezconfig

/**
 * @file global.hpp
 * @brief Utilities to define and maintain a global instance.
 */

#pragma once

namespace ezconfig {

/**
 * @brief Getter for global instances.
 *
 * Use the macros below to declare and define global instance getters.
 */
template<typename T>
T & gInstance();

};  // namespace ezconfig

/**
 * @brief Declare a template specialization of gInstance.
 *
 * Use this in the '.hpp' file to declare a global instance getter for the class.
 *
 * @note Must be used outside of namespaces.
 */
#define EZ_GLOBAL_DECLARE(...) \
  template<>                   \
  __VA_ARGS__ & ::ezconfig::gInstance<__VA_ARGS__>()

/**
 * @brief Define a template specialization of gInstance that returns the global instance.
 *
 * Use this in the '.cpp' file to define a unique global instance for the class.
 *
 * @note Must be used outside of namespaces.
 */
#define EZ_GLOBAL_DEFINE(...)                        \
  template<>                                         \
  __VA_ARGS__ & ::ezconfig::gInstance<__VA_ARGS__>() \
  {                                                  \
    static __VA_ARGS__ global_instance{};            \
    return global_instance;                          \
  }                                                  \
  static_assert(true)

/**
 * @brief Retrieve the global instance for a type.
 */
#define EZ_GLOBAL_INSTANCE(...) ::ezconfig::gInstance<__VA_ARGS__>()
