// Copyright (c) 2023 Petter Nilsson. MIT License. https://github.com/pettni/ezconfig

#pragma once

#include <yaml-cpp/yaml.h>

#include "stl_fwd.hpp"

namespace YAML {

template<typename T>
bool convert<std::optional<T>>::decode(const Node & yaml, std::optional<T> & obj)
{
  if (yaml.IsNull()) {
    obj = std::nullopt;
  } else {
    obj = yaml.as<T>();
  }
  return true;
}

template<typename T>
bool convert<std::unordered_map<std::string, T>>::decode(const Node & yaml, std::unordered_map<std::string, T> & obj)
{
  obj.clear();
  for (const auto & node : yaml) {
    const auto key = node.first.as<std::string>();
    if (obj.contains(key)) { throw YAML::ParserException(yaml.Mark(), "Double key '" + key + "' in map"); }
    obj[node.first.as<std::string>()] = node.second.as<T>();
  }
  return true;
}

inline bool convert<std::filesystem::path>::decode(const Node & yaml, std::filesystem::path & obj)
{
  obj = yaml.as<std::string>();
  return true;
}

template<intmax_t Num, intmax_t Den>
bool convert<std::chrono::duration<int64_t, std::ratio<Num, Den>>>::decode(
  const Node & yaml, std::chrono::duration<int64_t, std::ratio<Num, Den>> & obj)
{
  using RetType  = std::chrono::duration<int64_t, std::ratio<Num, Den>>;
  const auto str = yaml.as<std::string>();
  if (str.ends_with("ms")) {
    obj = std::chrono::duration_cast<RetType>(std::chrono::milliseconds(std::stoi(str.substr(0, str.size() - 2))));
  } else if (str.ends_with("us")) {
    obj = std::chrono::duration_cast<RetType>(std::chrono::microseconds(std::stoi(str.substr(0, str.size() - 2))));
  } else if (str.ends_with("ns")) {
    obj = std::chrono::duration_cast<RetType>(std::chrono::nanoseconds(std::stoi(str.substr(0, str.size() - 2))));
  } else if (str.ends_with("s")) {
    obj = std::chrono::duration_cast<RetType>(std::chrono::seconds(std::stoi(str.substr(0, str.size() - 1))));
  } else if (str.ends_with("m")) {
    obj = std::chrono::duration_cast<RetType>(std::chrono::minutes(std::stoi(str.substr(0, str.size() - 1))));
  } else if (str.ends_with("h")) {
    obj = std::chrono::duration_cast<RetType>(std::chrono::hours(std::stoi(str.substr(0, str.size() - 1))));
  } else {
    throw YAML::ParserException{
      yaml.Mark(),
      "Could not detect suffix in '" + str + "', expected s, ms, us, or ns",
    };
  }

  return true;
}

template<typename... Ts>
bool convert<std::variant<Ts...>>::decode(const Node & yaml, std::variant<Ts...> & obj)
{
  const auto & type_names = variant_type_names<std::variant<Ts...>>::value;

  if (type_names.contains(yaml.Tag())) {
    std::visit([&]<typename T>(const T &) { obj = yaml.template as<T>(); }, type_names.at(yaml.Tag()));
    return true;
  }

  throw YAML::ParserException{
    yaml.Mark(),
    "Tag '" + yaml.Tag() + "' not found in variant_type_names<>",
  };
}

}  // namespace YAML
