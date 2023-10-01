// Copyright (c) 2023 Petter Nilsson. MIT License. https://github.com/pettni/ezconfig

/**
 * @file factory.hpp
 * @brief Factory base class.
 */

#pragma once

#include <functional>
#include <map>
#include <memory>
#include <sstream>

#include "global.hpp"

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
   * @brief Add a factory method to the factory.
   *
   * @param tag
   * @param factory
   */
  void add(const std::string & tag, std::function<std::unique_ptr<Base>(Args...)> factory)
  {
    if (m_tags.contains(tag)) { throw std::logic_error("Tag '" + tag + "' already present"); }
    m_tags[tag] = std::move(factory);
  }

  /**
   * @brief Create an object.
   */
  std::unique_ptr<Base> create(const std::string & tag, auto &&... args)
  {
    if (auto it = m_tags.find(tag); it != m_tags.end()) {
      return std::invoke(it->second, std::forward<decltype(args)>(args)...);
    } else {
      std::stringstream ss;
      ss << "Could not find tag '" << tag << "'. ";
      ss << "Available tags: [";
      for (auto i = 0u; const auto & [tag_i, f] : m_tags) {
        ss << "'" << tag_i << "'";
        if (++i < m_tags.size()) { ss << ", "; }
      }
      ss << "]";
      throw std::logic_error(ss.str());
    }
  }

protected:
  std::map<std::string, std::function<std::unique_ptr<Base>(Args...)>> m_tags;
};

/// @brief Type trait that marks existing factories.
template<typename Base, typename... Args>
struct has_factory : public std::false_type
{};

/// @brief Concept that identifies existing factories.
template<typename Base, typename... Args>
concept Constructible = requires { has_factory<Base, Args...>::value == true; };

/// @brief Declare a global factory instance.
#define EZ_FACTORY_DECLARE(Base, ...)                                                   \
  EZ_GLOBAL_DECLARE(ezconfig::Factory<Base __VA_OPT__(, ) __VA_ARGS__>);                \
  template<>                                                                            \
  struct ezconfig::has_factory<Base __VA_OPT__(, ) __VA_ARGS__> : public std::true_type \
  {}

/// @brief Define a global factory instance.
#define EZ_FACTORY_DEFINE(Base, ...) EZ_GLOBAL_DEFINE(ezconfig::Factory<Base __VA_OPT__(, ) __VA_ARGS__>)

/// @brief Retrieve the global factory instance.
#define EZ_FACTORY_INSTANCE(Base, ...) EZ_GLOBAL_INSTANCE(ezconfig::Factory<Base __VA_OPT__(, ) __VA_ARGS__>)

/// @brief Register a tag and creator method in the global factory instance.
#define EZ_FACTORY_REGISTER(tag, creator, Base, ...) \
  EZ_STATIC_INVOKE(                                  \
    &ezconfig::Factory<Base __VA_OPT__(, ) __VA_ARGS__>::add, EZ_FACTORY_INSTANCE(Base, __VA_ARGS__), tag, creator);

}  // namespace ezconfig
