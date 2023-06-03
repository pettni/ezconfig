// Copyright (c) 2023 Petter Nilsson. MIT License. https://github.com/pettni/ezconfig

#include "ezconfig/factory.hpp"

class TestBase
{
public:
  virtual ~TestBase() = default;
  virtual int id() = 0;
};

EZ_DECLARE(TestBase);
EZ_DECLARE(TestBase, int, std::string);
