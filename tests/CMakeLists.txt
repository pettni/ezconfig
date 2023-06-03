include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.3.2
)
FetchContent_MakeAvailable(Catch2)

FetchContent_Declare(
  YamlCpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG yaml-cpp-0.7.0
)
FetchContent_MakeAvailable(YamlCpp)

FetchContent_Declare(
  NlohJson
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(NlohJson)

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
            # -Wold-style-cast
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
target_link_libraries(
  test_readme PRIVATE testopts yaml-cpp nlohmann_json::nlohmann_json
)
catch_discover_tests(test_readme)
