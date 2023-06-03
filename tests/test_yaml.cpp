// Copyright (c) 2023 Petter Nilsson. MIT License. https://github.com/pettni/ezconfig

#include <catch2/catch_test_macros.hpp>

#include "ezconfig/yaml.hpp"

using namespace ezconfig;

class TBase
{
public:
  virtual ~TBase() = default;
  virtual std::string id() = 0;
};

EZ_YAML_DECLARE(TBase);

class TDerived1 : public TBase
{
public:
  TDerived1(std::string x) : m_x(x) {}

  virtual std::string id() { return m_x; }

private:
  std::string m_x;
};

class TDerived2 : public TBase
{
public:
  TDerived2(int x) : m_x(x) {}

  virtual std::string id() { return std::to_string(m_x); }

private:
  int m_x;
};

EZ_YAML_REGISTER(TBase, "!d1", TDerived1, std::string);
EZ_YAML_REGISTER(TBase, "!d2", TDerived2, int);

TEST_CASE("JsonCreate")
{
  auto d1 = EZ_YAML_CREATE(TBase, YAML::Load(R"(
!d1
hello
)"));
  REQUIRE(d1->id() == "hello");

  auto d2 = EZ_YAML_CREATE(TBase, YAML::Load(R"(
!d2
123
)"));
  REQUIRE(d2->id() == "123");
}

TEST_CASE("YamlInvalidTag1")
{
  auto f = [] { YamlFactory<TBase>::Register<TDerived1, std::string>("d3"); };
  REQUIRE_THROWS_AS(f(), std::logic_error);
}

TEST_CASE("YamlInvalidTag2")
{
  auto f = [] { YamlFactory<TBase>::Register<TDerived1, std::string>("!"); };
  REQUIRE_THROWS_AS(f(), std::logic_error);
}

TEST_CASE("YamlAsUnique")
{
  std::string yaml_str{
    R"(
!d1
hallo
)"};
  auto d1 = YAML::Load(yaml_str).as<std::unique_ptr<TBase>>();
  REQUIRE(d1->id() == "hallo");
}

TEST_CASE("YamlAsShared")
{
  std::string yaml_str{
    R"(
!d1
hallo
)"};
  auto d1 = YAML::Load(yaml_str).as<std::shared_ptr<TBase>>();
  REQUIRE(d1->id() == "hallo");
}

TEST_CASE("YamlAsVector")
{
  std::string yaml_str{
    R"(
- !d1
  hello
- !d1
  holla
- !d2
  4321234
)"};
  const auto vec = YAML::Load(yaml_str).as<std::vector<std::shared_ptr<TBase>>>();
  REQUIRE(vec.size() == 3);
  REQUIRE(vec[0]->id() == "hello");
  REQUIRE(vec[1]->id() == "holla");
  REQUIRE(vec[2]->id() == "4321234");
}
