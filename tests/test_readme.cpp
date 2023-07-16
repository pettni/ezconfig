// Copyright (c) 2023 Petter Nilsson. MIT License. https://github.com/pettni/ezconfig

#include <catch2/catch_test_macros.hpp>

#include "ezconfig/json.hpp"
#include "ezconfig/yaml.hpp"

struct MyBase
{};

struct MyDerived : public MyBase
{
  struct Config
  {
    int x, y;
  };

  MyDerived(const Config &) {}
};

/// JSON

// declare factory for hierarchy (in header)
EZ_JSON_DECLARE(MyBase);

// declare factory for hierarchy (in implementatiohn file)
EZ_JSON_DEFINE(MyBase);

// make Config struct parse-able from json
// this can be automated with boost::hana
void from_json(const nlohmann::json & j, MyDerived::Config & p)
{
  p.x = j.at("x").get<int>();
  p.y = j.at("y").get<int>();
}

// register a factory method with a tag via MyDerived::Config
EZ_JSON_REGISTER(MyBase, "mytag", MyDerived, MyDerived::Config);

/// YAML

// declare factory for hierarchy (in header)
EZ_YAML_DECLARE(MyBase);

// define factory for hierarchy (in implementation file)
EZ_YAML_DEFINE(MyBase);

// make Config struct parse-able from json
// this can be automated with boost::hana
template<>
struct YAML::convert<MyDerived::Config>
{
  static bool decode(const YAML::Node & yaml, MyDerived::Config & obj)
  {
    obj.x = yaml["x"].as<int>();
    obj.y = yaml["y"].as<int>();
    return true;
  }
};

// register a factory method with a tag via MyDerived::Config
EZ_YAML_REGISTER(MyBase, "!mytag", MyDerived, MyDerived::Config);

TEST_CASE("ReadmeYaml")
{
  std::string yaml_data = R"(
  !mytag
  x: 1
  y: 2
  )";

  auto obj = YAML::Load(yaml_data).as<std::unique_ptr<MyBase>>();
}

TEST_CASE("ReadmeJson")
{
  std::string json_data = R"(
  {"mytag": {"x": 1, "y": 2}}
  )";

  auto obj = nlohmann::json::parse(json_data).get<std::unique_ptr<MyBase>>();
}

TEST_CASE("ReadmeJsonMap")
{
  std::string json_data = R"(
{
    "key1": {"mytag": {"x": 1, "y": 2}},
    "key2": {"mytag": {"x": 3, "y": 4}}
}
)";

  auto obj = nlohmann::json::parse(json_data).get<std::map<std::string, std::unique_ptr<MyBase>>>();

  REQUIRE(obj.size() == 2);
}
