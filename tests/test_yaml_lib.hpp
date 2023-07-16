// Copyright (c) 2023 Petter Nilsson. MIT License. https://github.com/pettni/ezconfig

#include <memory>
#include <string>

#include "ezconfig/yaml_fwd.hpp"

using namespace ezconfig;

namespace mylib {

class TBase
{
public:
  virtual ~TBase()         = default;
  virtual std::string id() = 0;
};

}  // namespace mylib

EZ_YAML_DECLARE(mylib::TBase);
