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

cpmaddpackage(
  NAME smooth
  VERSION master
  GIT_REPOSITORY https://github.com/pettni/smooth.git
  GIT_TAG master
  DOWNLOAD_ONLY YES
)
if(smooth_ADDED)
  add_library(smooth INTERFACE IMPORTED)
  target_include_directories(smooth INTERFACE ${smooth_SOURCE_DIR}/include)
endif()

cpmaddpackage(
  NAME magic_enum
  VERSION v0.9.3
  GIT_REPOSITORY https://github.com/Neargye/magic_enum.git
  GIT_TAG v0.9.3
  DOWNLOAD_ONLY YES
)
if(magic_enum_ADDED)
  add_library(magic_enum INTERFACE IMPORTED)
  target_include_directories(magic_enum INTERFACE ${magic_enum_SOURCE_DIR}/include)
endif()
