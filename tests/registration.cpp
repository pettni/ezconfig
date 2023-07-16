// Copyright (c) 2023 Petter Nilsson. MIT License. https://github.com/pettni/ezconfig

#include "declaration.hpp"
#include "ezconfig/macro.hpp"

using namespace ezconfig;

EZ_GLOBAL_DEFINE(Factory<TestBase>);
EZ_GLOBAL_DEFINE(Factory<TestBase, int, std::string>);

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

EZ_STATIC_INVOKE(&Factory<TestBase>::add, gInstance<Factory<TestBase>>(), "d1", [] {
  return std::make_unique<TestDerived1>();
});
EZ_STATIC_INVOKE(&Factory<TestBase>::add, gInstance<Factory<TestBase>>(), "d2", [] {
  return std::make_unique<TestDerived2>();
});

EZ_STATIC_INVOKE(
  &Factory<TestBase, int, std::string>::add,
  gInstance<Factory<TestBase, int, std::string>>(),
  "d1",
  [](int, std::string) { return std::make_unique<TestDerived1>(); });
EZ_STATIC_INVOKE(
  &Factory<TestBase, int, std::string>::add,
  gInstance<Factory<TestBase, int, std::string>>(),
  "d2",
  [](int, std::string) { return std::make_unique<TestDerived2>(); });
