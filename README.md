# ezconfig: Easy creation of C++ classes from json and yaml

[![Github][lines-shield]][lines-link]
[![CI Build and Test][ci-shield]][ci-link]
[![License][license-shield]][license-link]

* Small header-only library for creating objects from dynamic json or yaml data.
* Uses [nlohmann_json][nlohjson-link] and [yaml-cpp][yamlcpp-link] for parsing.
* [Documentation][doc-link].

## Examples

Suppose we have a class hierarchy with members that need to be instantiated from config files or
some other dynamic data.
```cpp
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
```
This library helps create objects in the hierarchy from json or yaml data.

After registering tags with derived types (see below) creating instances is as easy as this for yaml:

```cpp
std::string yaml_data = R"(
!mytag
x: 1
y: 2
)";

auto obj = YAML::Load(yaml_data).as<std::unique_ptr<MyBase>>();
```
Similarly for json:
```cpp
std::string json_data = R"(
{"mytag": {"x": 1, "y": 2}}
)";

auto obj = nlohmann::json::parse(json_data).get<std::unique_ptr<MyBase>>();
```
It also works with STL types. For example:
```cpp
std::string json_data = R"(
{
    "key1": {"mytag": {"x": 1, "y": 2}},
    "key2": {"mytag": {"x": 3, "y": 4}}
}
)";

auto obj = nlohmann::json::parse(json_data).get<std::map<std::string, std::unique_ptr<MyBase>>>();
```

### Register yaml converter

Yaml conversion uses [`yaml-cpp`][yamlcpp-link].

```cpp
#include <ezconfig/yaml.hpp>

// declare factory for hierarchy
EZ_YAML_DECLARE(MyBase);

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
```

### Register json converter

Json conversion uses [`nlohmann_json`][nlohjson-link].

```cpp
#include <ezconfig/json.hpp>

// declare factory for hierarchy
EZ_JSON_DECLARE(MyBase);

// make Config struct parse-able from json
// this can be automated with boost::hana
void from_json(const nlohmann::json & j, MyDerived::Config & p)
{
  p.x = j.at("x").get<int>();
  p.y = j.at("y").get<int>();
}

// register a factory method with a tag via MyDerived::Config
EZ_JSON_REGISTER(MyBase, "mytag", MyDerived, MyDerived::Config);
```

## TODOs

* Add missing yaml and json conversions, including from boost::hana structs.


<!-- MARKDOWN LINKS AND IMAGES -->
[doc-link]: https://pettni.github.io/ezconfig

[ci-shield]: https://img.shields.io/github/actions/workflow/status/pettni/ezconfig/build_and_test.yml?style=flat-square
[ci-link]: https://github.com/pettni/ezconfig/actions/workflows/build_and_test.yml

[license-shield]: https://img.shields.io/github/license/pettni/ezconfig.svg?style=flat-square
[license-link]: https://github.com/pettni/ezconfig/blob/master/LICENSE

[lines-shield]: https://img.shields.io/tokei/lines/github/pettni/ezconfig?style=flat-square
[lines-link]: https://github.com/pettni/ezconfig

[yamlcpp-link]: https://github.com/jbeder/yaml-cpp
[nlohjson-link]: https://github.com/nlohmann/json
