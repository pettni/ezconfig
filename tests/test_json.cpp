// Copyright (c) 2023 Petter Nilsson. MIT License. https://github.com/pettni/ezconfig

#include <iostream>

#include <catch2/catch_test_macros.hpp>

#include "ezconfig/json.hpp"

using namespace ezconfig;

class TBase
{
public:
  virtual ~TBase()         = default;
  virtual std::string id() = 0;
};

EZ_JSON_DECLARE(TBase);
EZ_JSON_DEFINE(TBase);

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

struct TDerived3 : public TBase
{
  int x{};
  int y{};
  virtual std::string id() { return std::to_string(x + y); }
};

// conversion from json directly to Derived3
void from_json(const nlohmann::json & j, TDerived3 & p)
{
  j.at("x").get_to(p.x);
  j.at("y").get_to(p.y);
}

EZ_JSON_REGISTER(TBase, "d1", TDerived1, std::string);
EZ_JSON_REGISTER(TBase, "d2", TDerived2, int);
EZ_JSON_REGISTER(TBase, "d3", TDerived3);

EZ_FACTORY_REGISTER(
  "hello", [](const nlohmann::json &) { return std::unique_ptr<TBase>{}; }, TBase, const nlohmann::json &);

TEST_CASE("JsonCreateIntermediate")
{
  auto d1 = json::Create<TBase>(nlohmann::json::parse(R"(
  {
    "d1": "hello"
  }
)"));
  REQUIRE(d1->id() == "hello");

  auto d2 = json::Create<TBase>(nlohmann::json::parse(R"(
  {
    "d2": 123
  }
)"));
  REQUIRE(d2->id() == "123");
}

TEST_CASE("JsonCreateDirect")
{
  auto d3 = json::Create<TBase>(nlohmann::json::parse(R"(
  {
    "d3": {
        "x": 1,
        "y": 2
    }
  }
)"));
  REQUIRE(d3->id() == "3");
}

TEST_CASE("JsonTooLong")
{
  REQUIRE_THROWS(json::Create<TBase>(nlohmann::json::parse(R"(
  {
    "d1": "hello",
    "d2": 123
  }
)")));
}

TEST_CASE("JsonNotObject")
{
  REQUIRE_THROWS(json::Create<TBase>(nlohmann::json::parse(R"(
  {
    "hello"
  }
)")));
}

TEST_CASE("JsonAsUnique")
{
  std::string yaml_str{
    R"(
{"d1": "hello"}
)"};
  auto d1 = nlohmann::json::parse(yaml_str).get<std::unique_ptr<TBase>>();
  REQUIRE(d1->id() == "hello");
}

TEST_CASE("JsonAsShared")
{
  std::string yaml_str{
    R"(
{"d1": "hello"}
)"};
  auto d1 = nlohmann::json::parse(yaml_str).get<std::shared_ptr<TBase>>();
  REQUIRE(d1->id() == "hello");
}

TEST_CASE("JsonAsVector")
{
  std::string yaml_str{
    R"(
[
  {"d1": "hello"},
  {"d1": "holla"},
  {"d2": 4321234},
  {"d3": {"x": 12, "y": 15}}
]
)"};
  const auto vec = nlohmann::json::parse(yaml_str).get<std::vector<std::shared_ptr<TBase>>>();
  REQUIRE(vec.size() == 4);
  REQUIRE(vec[0]->id() == "hello");
  REQUIRE(vec[1]->id() == "holla");
  REQUIRE(vec[2]->id() == "4321234");
  REQUIRE(vec[3]->id() == "27");
}
