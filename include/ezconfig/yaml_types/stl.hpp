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
Node convert<std::optional<T>>::encode(const std::optional<T> & obj)
{
  if (obj.has_value()) {
    return Node(*obj);
  } else {
    return Node(Null);
  }
}

template<typename K, typename V, typename C, typename A>
bool convert<std::unordered_map<K, V, C, A>>::decode(const Node & yaml, std::unordered_map<K, V, C, A> & obj)
{
  obj.clear();
  for (const auto & node : yaml) {
    const auto key = node.first.as<std::string>();
    if (obj.contains(key)) { throw YAML::ParserException(yaml.Mark(), "Double key '" + key + "' in map"); }
    obj[node.first.as<K>()] = node.second.as<V>();
  }
  return true;
}

template<typename K, typename V, typename C, typename A>
Node convert<std::unordered_map<K, V, C, A>>::encode(const std::unordered_map<K, V, C, A> & obj)
{
  YAML::Node node;
  for (const auto & [key, val] : obj) { node[key] = YAML::Node(val); }
  return node;
}

inline bool convert<std::filesystem::path>::decode(const Node & yaml, std::filesystem::path & obj)
{
  obj = yaml.as<std::string>();
  return true;
}

inline Node convert<std::filesystem::path>::encode(const std::filesystem::path & obj) { return Node(obj.string()); }

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

template<intmax_t Num, intmax_t Den>
Node convert<std::chrono::duration<int64_t, std::ratio<Num, Den>>>::encode(
  const std::chrono::duration<int64_t, std::ratio<Num, Den>> & obj)
{
  using R = std::ratio<Num, Den>;
  if constexpr (std::is_same_v<R, std::milli>) {
    return std::to_string(obj.count()) + "ms";
  } else if constexpr (std::is_same_v<R, std::micro>) {
    return std::to_string(obj.count()) + "us";
  } else if constexpr (std::is_same_v<R, std::nano>) {
    return std::to_string(obj.count()) + "ns";
  } else if constexpr (std::is_same_v<R, std::ratio<1>>) {
    return std::to_string(obj.count()) + "s";
  } else if constexpr (std::is_same_v<R, std::ratio<60>>) {
    return std::to_string(obj.count()) + "m";
  } else if constexpr (std::is_same_v<R, std::ratio<3600>>) {
    return std::to_string(obj.count()) + "h";
  } else {
    return std::to_string(std::chrono::duration_cast<std::chrono::nanoseconds>(obj).count()) + "ns";
  }
}

}  // namespace YAML
