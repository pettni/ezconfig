// Copyright (c) 2023 Petter Nilsson. MIT License. https://github.com/pettni/ezconfig

#pragma once

#include <magic_enum.hpp>
#include <yaml-cpp/yaml.h>

#include "enum_fwd.hpp"

template<ezconfig::ScopedEnum T>
bool YAML::convert<T>::decode(const Node & yaml, T & obj)
{
  auto maybe_val = magic_enum::enum_cast<T>(yaml.as<std::string>());
  if (maybe_val.has_value()) {
    obj = maybe_val.value();
    return true;
  }
  return false;
}
