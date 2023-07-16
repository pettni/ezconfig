// Copyright (c) 2023 Petter Nilsson. MIT License. https://github.com/pettni/ezconfig

#include "test_yaml_lib.hpp"

#include "ezconfig/yaml.hpp"

using namespace ezconfig;

EZ_YAML_DEFINE(mylib::TBase);

namespace mylib {

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

class TDerived3 : public TBase
{
public:
  TDerived3() : m_x(0) {}
  TDerived3(int x) : m_x(x) {}

  virtual std::string id() { return std::to_string(m_x); }

private:
  int m_x;
};

}  // namespace mylib

template<>
struct YAML::convert<mylib::TDerived3>
{
  static bool decode(const YAML::Node & y, mylib::TDerived3 & der3)
  {
    der3 = mylib::TDerived3{y.as<int>()};
    return true;
  }
};

EZ_YAML_REGISTER(mylib::TBase, "!d1", mylib::TDerived1, std::string);
EZ_YAML_REGISTER(mylib::TBase, "!d2", mylib::TDerived2, int);
EZ_YAML_REGISTER(mylib::TBase, "!d3", mylib::TDerived3);
