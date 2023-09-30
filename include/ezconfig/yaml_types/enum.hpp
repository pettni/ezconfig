// Copyright (c) 2023 Petter Nilsson. MIT License. https://github.com/pettni/ezconfig

#pragma once

#include <magic_enum.hpp>
#include <yaml-cpp/yaml.h>

#include "enum_fwd.hpp"

namespace YAML {

template<typename T>
  requires(ezconfig::detail::is_scoped_enum<T>::value)
bool convert<T>::decode(const Node & yaml, T & obj)
{
  auto maybe_val = magic_enum::enum_cast<T>(yaml.as<std::string>());
  if (maybe_val.has_value()) {
    obj = maybe_val.value();
    return true;
  }
  return false;
}

}  // namespace YAML
