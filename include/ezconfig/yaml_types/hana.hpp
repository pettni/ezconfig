// Copyright (c) 2023 Petter Nilsson. MIT License. https://github.com/pettni/ezconfig

#pragma once

#include <boost/hana/at_key.hpp>
#include <boost/hana/for_each.hpp>
#include <boost/hana/keys.hpp>
#include <boost/hana/map.hpp>
#include <yaml-cpp/yaml.h>

#include "hana_fwd.hpp"

namespace YAML {

template<typename T>
  requires(boost::hana::Struct<T>::value)
bool convert<T>::decode(const Node & yaml, T & t)
{
  boost::hana::for_each(boost::hana::keys(t), [&](auto key) {
    using ValT                  = std::decay_t<decltype(boost::hana::at_key(t, key))>;
    char const * key_c          = boost::hana::to<char const *>(key);
    boost::hana::at_key(t, key) = yaml[key_c].template as<ValT>();
  });

  return true;
}

template<typename... Ts>
bool convert<std::variant<Ts...>>::decode(const Node & yaml, std::variant<Ts...> & obj)
{
  const auto & hana_map = ::ezconfig::variant_hana_maps<std::variant<Ts...>>::value;

  bool ret = false;
  boost::hana::for_each(hana_map, [&](const auto & entry) {
    if (boost::hana::first(entry) == yaml.Tag()) {
      using type = std::decay_t<decltype(boost::hana::second(entry))>::type;
      obj        = yaml.template as<type>();
      ret        = true;
    }
  });

  return ret;
}

}  // namespace YAML
