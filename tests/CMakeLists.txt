cmake_minimum_required(VERSION 3.25)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.3.2
  GIT_SHALLOW ON
  SYSTEM
)
FetchContent_MakeAvailable(Catch2)

FetchContent_Declare(
  YamlCpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG yaml-cpp-0.7.0
  GIT_SHALLOW ON
  SYSTEM
)
FetchContent_MakeAvailable(YamlCpp)

FetchContent_Declare(
  NlohJson
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.2
  GIT_SHALLOW ON
  SYSTEM
)
FetchContent_MakeAvailable(NlohJson)

FetchContent_Declare(
  Eigen
  GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
  GIT_TAG 3.4.0
  GIT_SHALLOW ON
  SYSTEM
)
set(BUILD_TESTING OFF) # TODO: for eigen, should be in sub-directory
set(EIGEN_BUILD_PKGCONFIG OFF)
FetchContent_MakeAvailable(Eigen)

FetchContent_Declare(
  Hana
  GIT_REPOSITORY https://github.com/boostorg/hana.git
  GIT_TAG boost-1.82.0
  GIT_SHALLOW ON
  SYSTEM
)
FetchContent_MakeAvailable(Hana)

include(CTest)
include(Catch)

add_library(testopts INTERFACE)
target_link_libraries(testopts INTERFACE ezconfig Catch2::Catch2WithMain)
target_compile_options(
  testopts
  INTERFACE -fdiagnostics-color=always
            -Wall
            -Wcast-align
            -Wconversion
            -Wdouble-promotion
            -Werror
            -Wextra
            -Wimplicit-fallthrough
            -Wnon-virtual-dtor
            -Wnull-dereference
            -Wold-style-cast
            -Woverloaded-virtual
            -Wpedantic
            -Wshadow
            -Wsign-conversion
            -Wunused
)

add_executable(test_general registration.cpp test_general.cpp)
target_link_libraries(test_general PRIVATE testopts)
catch_discover_tests(test_general)

add_executable(test_json test_json.cpp)
target_link_libraries(test_json PRIVATE testopts nlohmann_json::nlohmann_json)
catch_discover_tests(test_json)

add_executable(test_yaml test_yaml.cpp)
target_link_libraries(test_yaml PRIVATE testopts yaml-cpp)
catch_discover_tests(test_yaml)

add_executable(test_readme test_readme.cpp)
target_link_libraries(test_readme PRIVATE testopts yaml-cpp nlohmann_json::nlohmann_json)
catch_discover_tests(test_readme)

add_executable(test_yaml_extra test_yaml_extra.cpp)
target_include_directories(test_yaml_extra SYSTEM PRIVATE ${boost_hana_SOURCE_DIR}/include)
target_link_libraries(test_yaml_extra PRIVATE Eigen3::Eigen testopts yaml-cpp)
catch_discover_tests(test_yaml_extra)
