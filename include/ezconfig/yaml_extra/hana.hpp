// Copyright (c) 2023 Petter Nilsson. MIT License. https://github.com/pettni/ezconfig

#pragma once

#include <boost/hana/at_key.hpp>
#include <boost/hana/for_each.hpp>
#include <boost/hana/keys.hpp>
#include <yaml-cpp/yaml.h>

#include "hana_fwd.hpp"

namespace YAML {

template<typename T>
  requires(boost::hana::Struct<T>::value) bool
convert<T>::decode(const Node & yaml, T & t)
{
  boost::hana::for_each(boost::hana::keys(t), [&](auto key) {
    using ValT                  = std::decay_t<decltype(boost::hana::at_key(t, key))>;
    char const * key_c          = boost::hana::to<char const *>(key);
    boost::hana::at_key(t, key) = yaml[key_c].template as<ValT>();
  });

  return true;
}

}  // namespace YAML
