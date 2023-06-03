// Copyright (c) 2023 Petter Nilsson. MIT License. https://github.com/pettni/ezconfig

#include <iostream>

#include <catch2/catch_test_macros.hpp>

#include "declaration.hpp"

using namespace ezconfig;

TEST_CASE("CreateWithoutArgs")
{
  auto d1 = Factory<TestBase>::Create("d1");
  auto d2 = Factory<TestBase>::Create("d2");

  REQUIRE(d1->id() == 1);
  REQUIRE(d2->id() == 2);
}

TEST_CASE("CreateWithArgs")
{
  auto d1 = Factory<TestBase, int, std::string>::Create("d1", 1, "hello");
  auto d2 = Factory<TestBase, int, std::string>::Create("d2", 1, "hello");

  REQUIRE(d1->id() == 1);
  REQUIRE(d2->id() == 2);
}

class TestDerived3 : public TestBase
{
public:
  virtual int id() { return 3; }
};

TEST_CASE("AddExisting")
{
  REQUIRE_THROWS_AS(
    Factory<TestBase>::Register("d1", []() -> std::unique_ptr<TestBase> { return std::make_unique<TestDerived3>(); }),
    std::logic_error);
}

TEST_CASE("CreateDoesNotExist") { REQUIRE_THROWS_AS(Factory<TestBase>::Create("d5"), std::logic_error); }
