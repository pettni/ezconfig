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
 * @tparam many whether to create a one-to-many factory or a one-to-one factory.
 * @tparam Signature type signature Base(Args...) where Base is the hierarchy base class, and Args... are the inputs to
 * the factory methods.
 *
 * If many = false, then the factory is (Args...) -> std::unique_ptr<Base>
 * If many = true,  then the factory is (Args...) -> std::vector<std::unique_ptr<Base>>
 */
template<bool many, typename Base, typename... Args>
class GeneralFactory
{
public:
  using OutputT    = std::conditional_t<many, std::vector<std::unique_ptr<Base>>, std::unique_ptr<Base>>;
  using GeneratorT = std::function<OutputT(Args...)>;

  /**
   * @brief Add a factory method to the factory.
   *
   * @param tag
   * @param factory
   */
  void add(const std::string & tag, GeneratorT factory)
  {
    if (m_tags.contains(tag)) { throw std::logic_error("Tag '" + tag + "' already present"); }
    m_tags[tag] = std::move(factory);
  }

  /**
   * @brief Create an object.
   */
  OutputT create(const std::string & tag, auto &&... args)
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
  std::map<std::string, GeneratorT> m_tags;
};

/**
 * @brief A factory that creates a single instance.
 */
template<typename Base, typename... Args>
using Factory = GeneralFactory<false, Base, Args...>;

/**
 * @brief A factory that creates a vector of instances.
 */
template<typename Base, typename... Args>
using ManyFactory = GeneralFactory<false, Base, Args...>;

/// @brief Type trait that marks existing factories.
template<bool many, typename Base, typename... Args>
struct has_factory : public std::false_type
{};

/// @brief Concept that identifies existing factories.
template<typename Base, typename... Args>
concept Constructible = requires { has_factory<false, Base, Args...>::value == true; };

/// @brief Concept that identifies existing factories.
template<typename Base, typename... Args>
concept ManyConstructible = requires { has_factory<true, Base, Args...>::value == true; };

/// @brief Declare a global factory instance.
#define EZ_GENERAL_FACTORY_DECLARE(many, Base, ...)                                           \
  EZ_GLOBAL_DECLARE(ezconfig::GeneralFactory<many, Base __VA_OPT__(, ) __VA_ARGS__>);         \
  template<>                                                                                  \
  struct ezconfig::has_factory<many, Base __VA_OPT__(, ) __VA_ARGS__> : public std::true_type \
  {}

/// @brief Define a global factory instance.
#define EZ_GENERAL_FACTORY_DEFINE(many, Base, ...) \
  EZ_GLOBAL_DEFINE(ezconfig::GeneralFactory<many, Base __VA_OPT__(, ) __VA_ARGS__>)

/// @brief Retrieve the global factory instance.
#define EZ_GENERAL_FACTORY_INSTANCE(many, Base, ...) \
  EZ_GLOBAL_INSTANCE(ezconfig::GeneralFactory<many, Base __VA_OPT__(, ) __VA_ARGS__>)

/// @brief Register a tag and creator method in the global factory instance.
#define EZ_GENERAL_FACTORY_REGISTER(many, tag, creator, Base, ...)         \
  EZ_STATIC_INVOKE(                                                        \
    &ezconfig::GeneralFactory<many, Base __VA_OPT__(, ) __VA_ARGS__>::add, \
    EZ_GENERAL_FACTORY_INSTANCE(many, Base, __VA_ARGS__),                  \
    tag,                                                                   \
    creator)

#define EZ_FACTORY_DECLARE(Base, ...) EZ_GENERAL_FACTORY_DECLARE(false, Base, __VA_ARGS__)
#define EZ_FACTORY_DEFINE(Base, ...) EZ_GENERAL_FACTORY_DEFINE(false, Base, __VA_ARGS__)
#define EZ_FACTORY_INSTANCE(Base, ...) EZ_GENERAL_FACTORY_INSTANCE(false, Base, __VA_ARGS__)
#define EZ_FACTORY_REGISTER(tag, creator, Base, ...) EZ_GENERAL_FACTORY_REGISTER(false, tag, creator, Base, __VA_ARGS__)

}  // namespace ezconfig
