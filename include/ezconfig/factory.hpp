// Copyright (c) 2023 Petter Nilsson. MIT License. https://github.com/pettni/ezconfig

/**
 * @file factory.hpp
 * @brief Factory base class.
 */

#pragma once

#include <functional>
#include <map>
#include <memory>

namespace ezconfig {

/**
 * @brief A Factory creates objects in a class hierarchy.
 *
 * @tparam Signature type signature Base(Args...) where Base is the hierarchy base class, and Args... are the inputs to
 * the factory methods.
 */
template<typename Base, typename... Args>
class Factory
{
public:
  /**
   * @brief Add a factory method to the registry.
   *
   * @param tag
   * @param factory
   */
  static void Register(const std::string & tag, std::function<std::unique_ptr<Base>(Args...)> factory);

  /**
   * @brief Create an object.
   */
  static std::unique_ptr<Base> Create(const std::string & tag, auto &&... args);

private:
  static std::map<std::string, std::function<std::unique_ptr<Base>(Args...)>> s_tags;
};

}  // namespace ezconfig

/**
 * @brief Declare a Factory by instantiating its template.
 *
 * Example: Declare a \a MyBase factory with two arguments of type \a int and \a std::string.
 * @code
 * EZ_DECLARE(MyBase, int, std::string);
 * @endcode
 */
#define EZ_DECLARE(...) template class ezconfig::Factory<__VA_ARGS__>

#include "factory_impl.hpp"
