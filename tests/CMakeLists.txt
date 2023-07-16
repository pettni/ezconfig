cmake_minimum_required(VERSION 3.25)

include(Dependencies.cmake)

include(CTest)
set(SANITIZERS "address,leak,undefined")

add_library(testopts INTERFACE)
target_link_libraries(testopts INTERFACE ezconfig Catch2::Catch2WithMain)
target_compile_options(
  testopts
  INTERFACE -fdiagnostics-color=always
            $<$<CONFIG:Debug>:-fsanitize=${SANITIZERS}>
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
target_link_options(testopts INTERFACE $<$<CONFIG:Debug>:-fsanitize=${SANITIZERS}>)

add_executable(test_general registration.cpp test_general.cpp)
target_link_libraries(test_general PRIVATE testopts)
catch_discover_tests(test_general)

add_library(reg_test_lib SHARED registration.cpp)
target_link_libraries(reg_test_lib PUBLIC ezconfig)

add_executable(test_linking test_general.cpp)
target_link_libraries(test_linking PRIVATE testopts reg_test_lib)
catch_discover_tests(test_linking)

# add_executable(test_json test_json.cpp)
# target_link_libraries(test_json PRIVATE testopts nlohmann_json::nlohmann_json)
# catch_discover_tests(test_json)
#
add_executable(test_yaml test_yaml.cpp)
target_link_libraries(test_yaml PRIVATE testopts yaml-cpp)
catch_discover_tests(test_yaml)

add_library(test_yaml_lib STATIC test_yaml_lib.cpp)
target_link_libraries(test_yaml_lib PUBLIC ezconfig PRIVATE yaml-cpp)

add_executable(test_yaml_linking test_yaml_linking.cpp)
target_link_libraries(test_yaml_linking PRIVATE testopts test_yaml_lib yaml-cpp)
catch_discover_tests(test_yaml_linking)

add_executable(test_readme test_readme.cpp)
target_link_libraries(test_readme PRIVATE testopts yaml-cpp nlohmann_json::nlohmann_json)
catch_discover_tests(test_readme)

add_executable(test_yaml_extra test_yaml_extra.cpp)
target_link_libraries(test_yaml_extra PRIVATE testopts Eigen Hana yaml-cpp)
catch_discover_tests(test_yaml_extra)
