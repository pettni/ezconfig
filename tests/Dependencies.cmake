include(${CMAKE_SOURCE_DIR}/cmake/CPM.cmake)

cpmaddpackage("gh:catchorg/Catch2@3.3.2")
include(${Catch2_SOURCE_DIR}/extras/Catch.cmake)

cpmaddpackage("gh:jbeder/yaml-cpp#yaml-cpp-0.7.0@0.7.0")

cpmaddpackage(
  NAME nlohmann_json
  VERSION 3.11.2
  URL https://github.com/nlohmann/json/archive/refs/tags/v3.11.2.tar.gz
)

cpmaddpackage(
  NAME Eigen
  VERSION 3.4.0
  URL https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.tar.gz
  DOWNLOAD_ONLY YES
)
if(Eigen_ADDED)
  add_library(Eigen INTERFACE IMPORTED)
  target_include_directories(Eigen INTERFACE ${Eigen_SOURCE_DIR})
endif()

cpmaddpackage(
  NAME Hana
  VERSION 1.82.0
  GIT_REPOSITORY https://github.com/boostorg/hana.git
  GIT_TAG boost-1.82.0
  DOWNLOAD_ONLY YES
)
if(Hana_ADDED)
  add_library(Hana INTERFACE IMPORTED)
  target_include_directories(Hana INTERFACE ${Hana_SOURCE_DIR}/include)
endif()
