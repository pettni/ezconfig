// Copyright (c) 2023 Petter Nilsson. MIT License. https://github.com/pettni/ezconfig

#pragma once

#include <sstream>

#include "factory.hpp"

namespace ezconfig {

template<typename Base, typename... Args>
void Factory<Base, Args...>::Register(const std::string & tag, std::function<std::unique_ptr<Base>(Args...)> factory)
{
  if (s_tags.contains(tag)) { throw std::logic_error("Tag '" + tag + "' already present"); }
  s_tags[tag] = std::move(factory);
}

template<typename Base, typename... Args>
std::unique_ptr<Base> Factory<Base, Args...>::Create(const std::string & tag, auto &&... args)
{
  if (auto it = s_tags.find(tag); it != s_tags.end()) {
    return std::invoke(it->second, std::forward<decltype(args)>(args)...);
  } else {
    std::stringstream ss;
    ss << "Could not find tag '" << tag << "'. ";
    ss << "Available tags: [";
    for (auto i = 0u; const auto & [tag_i, f] : s_tags) {
      ss << "'" << tag_i << "'";
      if (++i < s_tags.size()) { ss << ", "; }
    }
    ss << "]";
    throw std::logic_error(ss.str());
  }
}

// Instantiate static member
template<typename Base, typename... Args>
std::map<std::string, std::function<std::unique_ptr<Base>(Args...)>> Factory<Base, Args...>::s_tags{};

}  // namespace ezconfig
