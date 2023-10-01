// Copyright (c) 2023 Petter Nilsson. MIT License. https://github.com/pettni/ezconfig

#include "declaration.hpp"
#include "ezconfig/macro.hpp"

using namespace ezconfig;

EZ_FACTORY_DEFINE(TestBase);
EZ_FACTORY_DEFINE(TestBase, int, std::string);

class TestDerived1 : public TestBase
{
public:
  virtual int id() { return 1; }
};

class TestDerived2 : public TestBase
{
public:
  virtual int id() { return 2; }
};

EZ_FACTORY_REGISTER(
  "d1", [] { return std::make_unique<TestDerived1>(); }, TestBase);
EZ_FACTORY_REGISTER(
  "d2", [] { return std::make_unique<TestDerived2>(); }, TestBase);

EZ_FACTORY_REGISTER(
  "d1", [](int, std::string) { return std::make_unique<TestDerived1>(); }, TestBase, int, std::string);
EZ_FACTORY_REGISTER(
  "d2", [](int, std::string) { return std::make_unique<TestDerived2>(); }, TestBase, int, std::string);
