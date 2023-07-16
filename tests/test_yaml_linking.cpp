// Copyright (c) 2023 Petter Nilsson. MIT License. https://github.com/pettni/ezconfig

#include <iostream>

#include <catch2/catch_test_macros.hpp>
#include <yaml-cpp/yaml.h>

#include "test_yaml_lib.hpp"

TEST_CASE("YamlCreateLinked")
{
  const auto y1 = YAML::Load(R"(
!d1
hello
)");

  auto d1 = yaml::Create<mylib::TBase>(y1);
  REQUIRE(d1->id() == "hello");

  const auto y2 = YAML::Load(R"(
!d2
123
)");

  auto d2 = yaml::Create<mylib::TBase>(y2);
  REQUIRE(d2->id() == "123");

  const auto y3 = YAML::Load(R"(
!d3
124
)");

  auto d3 = y3.as<std::shared_ptr<mylib::TBase>>();
  REQUIRE(d3->id() == "124");

  const auto y4 = YAML::Load(R"(
!d3
125
)");

  auto d4 = y4.as<std::unique_ptr<mylib::TBase>>();
  REQUIRE(d4->id() == "125");
}
