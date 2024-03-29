// Copyright (c) 2023 Petter Nilsson. MIT License. https://github.com/pettni/ezconfig

#include <boost/hana/adapt_struct.hpp>
#include <boost/hana/tuple.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "ezconfig/yaml_types/eigen.hpp"
#include "ezconfig/yaml_types/enum.hpp"
#include "ezconfig/yaml_types/hana.hpp"
#include "ezconfig/yaml_types/smooth.hpp"
#include "ezconfig/yaml_types/stl.hpp"

using namespace std::literals;

std::string yaml_to_str(const YAML::Node & yaml)
{
  YAML::Emitter em;
  em << yaml;
  return em.c_str();
}

TEST_CASE("std_optional_decode")
{
  REQUIRE(YAML::Load("null").as<std::optional<int>>() == std::nullopt);
  REQUIRE(YAML::Load("~").as<std::optional<int>>() == std::nullopt);
  REQUIRE(YAML::Load("123").as<std::optional<int>>() == 123);
}

TEST_CASE("std_optional_encode")
{
  std::optional<std::string> obj1 = "hello";
  REQUIRE(yaml_to_str(YAML::Node(obj1)) == std::string{"hello"});

  std::optional<std::string> obj2 = std::nullopt;
  REQUIRE(yaml_to_str(YAML::Node(obj2)) == std::string{"~"});
}

TEST_CASE("std_map_decode")
{
  const std::string yaml_str = R"(
obj1: 1.23
obj2: 2.23
obj3: 3.23
)";

  auto obj = YAML::Load(yaml_str).as<std::unordered_map<std::string, double>>();

  REQUIRE(obj.size() == 3);
  REQUIRE(obj["obj1"] == 1.23);
  REQUIRE(obj["obj2"] == 2.23);
  REQUIRE(obj["obj3"] == 3.23);
}

TEST_CASE("std_map_encode")
{
  std::map<std::string, int> obj1{
    {"obj1", 3},
    {"obj2", 4},
    {"obj3", 4},
  };

  auto str = yaml_to_str(YAML::Node(obj1));
  REQUIRE(str == R"(obj1: 3
obj2: 4
obj3: 4)");
}

using MyVariant = std::variant<double, std::string, int>;

template<>
struct ezconfig::variant_hana_maps<MyVariant>
{
  static constexpr auto value = boost::hana::make_tuple(
    boost::hana::make_pair("!double", boost::hana::type_c<double>),
    boost::hana::make_pair("!int", boost::hana::type_c<int>),
    boost::hana::make_pair("!string", boost::hana::type_c<std::string>));
};

TEST_CASE("variant")
{
  const auto x = YAML::Load("!double 3.14").as<MyVariant>();
  REQUIRE(x.index() == 0);
  REQUIRE_THAT(std::get<double>(x), Catch::Matchers::WithinRel(3.14));

  const auto y = YAML::Load("!string 3.14").as<MyVariant>();
  REQUIRE(y.index() == 1);
  REQUIRE(std::get<std::string>(y) == "3.14");

  const auto z = YAML::Load("!int 3").as<MyVariant>();
  REQUIRE(z.index() == 2);
  REQUIRE(std::get<int>(z) == 3);

  REQUIRE_THROWS_AS(YAML::Load("!undefined 3.14").as<MyVariant>(), YAML::BadConversion);
  REQUIRE_THROWS_AS(YAML::Load("!double hello").as<MyVariant>(), YAML::BadConversion);
}

TEST_CASE("variant_vec")
{
  const std::string yaml_str = R"(
- !double
  3.14
- !int
  3
- !string
  3.14
- !double
  6.14
)";

  const auto data = YAML::Load(yaml_str).as<std::vector<MyVariant>>();

  REQUIRE(data.size() == 4);
  REQUIRE(data[0].index() == 0);
  REQUIRE(std::get<double>(data[0]) == 3.14);
  REQUIRE(data[1].index() == 2);
  REQUIRE(std::get<int>(data[1]) == 3);
  REQUIRE(data[2].index() == 1);
  REQUIRE(std::get<std::string>(data[2]) == "3.14");
  REQUIRE(data[3].index() == 0);
  REQUIRE(std::get<double>(data[3]) == 6.14);
}

TEST_CASE("variant_map")
{
  const std::string yaml_str = R"(
first_item: !double
            3.14
second_item: !int
             3
third_item: !string
  3.14
fourth_item: !double 6.14
)";

  const auto data = YAML::Load(yaml_str).as<std::unordered_map<std::string, MyVariant>>();

  REQUIRE(data.size() == 4);
  REQUIRE(data.at("first_item").index() == 0);
  REQUIRE(std::get<double>(data.at("first_item")) == 3.14);
  REQUIRE(data.at("second_item").index() == 2);
  REQUIRE(std::get<int>(data.at("second_item")) == 3);
  REQUIRE(data.at("third_item").index() == 1);
  REQUIRE(std::get<std::string>(data.at("third_item")) == "3.14");
  REQUIRE(data.at("fourth_item").index() == 0);
  REQUIRE(std::get<double>(data.at("fourth_item")) == 6.14);
}

TEST_CASE("stl_chrono")
{
  using namespace std::chrono_literals;

  REQUIRE(YAML::Load("5ns").as<std::chrono::nanoseconds>() == 5ns);
  REQUIRE(YAML::Load("5ns").as<std::chrono::hours>() == 0h);  // casts away precision..
  REQUIRE(YAML::Load("5us").as<std::chrono::microseconds>() == 5us);
  REQUIRE(YAML::Load("5ms").as<std::chrono::milliseconds>() == 5ms);
  REQUIRE(YAML::Load("5s").as<std::chrono::seconds>() == 5s);
  REQUIRE(YAML::Load("5m").as<std::chrono::minutes>() == 300s);
  REQUIRE(YAML::Load("5h").as<std::chrono::hours>() == 5h);
  REQUIRE(YAML::Load("5h").as<std::chrono::nanoseconds>() == 5h);
}

TEST_CASE("stl_filesystem")
{
  REQUIRE(YAML::Load("my/file").as<std::filesystem::path>() == std::filesystem::path("my/file"));
}

struct MyStruct
{
  MyVariant member1;
  std::vector<MyVariant> member2;
  std::optional<std::string> member3;
};

BOOST_HANA_ADAPT_STRUCT(MyStruct, member1, member2, member3);

TEST_CASE("boost_hana")
{
  const std::string yaml_str = R"(
member1: !string
  hello
member2:
  - !int
    5
  - !int
    6
member3: null
)";

  const auto data = YAML::Load(yaml_str).as<MyStruct>();

  REQUIRE(data.member1 == MyVariant("hello"));
  REQUIRE(data.member2.size() == 2);
  REQUIRE(data.member2[0] == MyVariant(5));
  REQUIRE(data.member2[1] == MyVariant(6));
  REQUIRE(data.member3 == std::nullopt);
}

TEST_CASE("eigen_vec_static")
{
  REQUIRE(YAML::Load("[1., 2., 3.]").as<Eigen::Vector3d>().isApprox(Eigen::Vector3d{1, 2, 3}));
  auto vec_str = R"(
x: 1.
y: 2.
z: 3.
  )";
  REQUIRE(YAML::Load(vec_str).as<Eigen::Vector3d>().isApprox(Eigen::Vector3d{{1, 2, 3}}));
  REQUIRE(YAML::Load("[1., 2., 3., 4.]").as<Eigen::Vector4d>().isApprox(Eigen::Vector4d{1, 2, 3, 4}));
}

TEST_CASE("eigen_vec_wrongsize")
{
  REQUIRE_THROWS_AS(YAML::Load("[1., 2., 3., 4.]").as<Eigen::Vector3d>(), YAML::ParserException);
}

TEST_CASE("eigen_mat_static")
{
  REQUIRE(YAML::Load("[[1., 2., 3.], [4., 5., 6.]]")
            .as<Eigen::Matrix<double, 2, 3>>()
            .isApprox(Eigen::MatrixXd{{1, 2, 3}, {4, 5, 6}}));
}

TEST_CASE("eigen_vec_dynamic")
{
  REQUIRE(YAML::Load("[1., 2., 3.]").as<Eigen::VectorXd>().isApprox(Eigen::VectorXd{{1, 2, 3}}));
  auto vec_str = R"(
x: 1.
y: 2.
z: 3.
  )";
  REQUIRE(YAML::Load(vec_str).as<Eigen::VectorXd>().isApprox(Eigen::Vector3d{{1, 2, 3}}));
  REQUIRE(YAML::Load("[1., 2., 3., 4.]").as<Eigen::VectorXd>().isApprox(Eigen::VectorXd{{1, 2, 3, 4}}));
}

TEST_CASE("eigen_mat_dynamic")
{
  REQUIRE(
    YAML::Load("[[1., 2., 3.], [4., 5., 6.]]").as<Eigen::MatrixXd>().isApprox(Eigen::MatrixXd{{1, 2, 3}, {4, 5, 6}}));
}

TEST_CASE("eigen_mat_empty") { REQUIRE_THROWS_AS(YAML::Load("[]").as<Eigen::MatrixXd>(), YAML::ParserException); }

TEST_CASE("eigen_mat_wrongsize")
{
  REQUIRE_THROWS_AS(YAML::Load("[[1., 2., 3.], [4., 5.]]").as<Eigen::MatrixXd>(), YAML::ParserException);
}

TEST_CASE("eigen_quat")
{
  auto quat_str1 = R"(
w: 0.
x: 0.
y: 0.
z: 1.
  )";
  REQUIRE(YAML::Load(quat_str1).as<Eigen::Quaterniond>().isApprox(Eigen::Quaterniond{0, 0, 0, 1}));

  auto quat_str2 = R"(
qw: 0.
qx: 0.
qy: 0.
qz: 1.
  )";
  REQUIRE(YAML::Load(quat_str2).as<Eigen::Quaterniond>().isApprox(Eigen::Quaterniond{0, 0, 0, 1}));
}

TEST_CASE("smooth")
{
  auto quat_str = R"(
w: 0.
x: 0.
y: 0.
z: 1.
  )";

  auto pose_str1 = R"(
translation:
  x: 1.
  y: -1.
  z: 1.
orientation:
  w: 0.
  x: 0.
  y: 0.
  z: 1.
  )";

  auto pose_str2 = R"(
  x: 1.
  y: -1.
  z: 1.
  qw: 0.
  qx: 0.
  qy: 0.
  qz: 1.
  )";

  REQUIRE(YAML::Load(quat_str).as<smooth::SO3d>().isApprox(smooth::SO3d(Eigen::Quaterniond{0, 0, 0, 1})));
  REQUIRE(YAML::Load(pose_str1).as<smooth::SE3d>().isApprox(
    smooth::SE3d(smooth::SO3d{Eigen::Quaterniond{0, 0, 0, 1}}, Eigen::Vector3d{1, -1, 1})));
  REQUIRE(YAML::Load(pose_str2).as<smooth::SE3d>().isApprox(
    smooth::SE3d(smooth::SO3d{Eigen::Quaterniond{0, 0, 0, 1}}, Eigen::Vector3d{1, -1, 1})));
}

enum class TestEnum {
  VALUE_1,
  VALUE_2,
  VALUE_3,
};

TEST_CASE("enum")
{
  REQUIRE(YAML::Load("VALUE_1").as<TestEnum>() == TestEnum::VALUE_1);
  REQUIRE(YAML::Load("VALUE_3").as<TestEnum>() == TestEnum::VALUE_3);
  REQUIRE_THROWS(YAML::Load("VALUE_4").as<TestEnum>());
}
