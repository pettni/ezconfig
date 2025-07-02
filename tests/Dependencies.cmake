file(
  DOWNLOAD
  https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.40.8/CPM.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
  EXPECTED_HASH SHA256=78ba32abdf798bc616bab7c73aac32a17bbd7b06ad9e26a6add69de8f3ae4791
)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)

cpmaddpackage("gh:catchorg/Catch2@3.4.0")
include(${Catch2_SOURCE_DIR}/extras/Catch.cmake)
cpmaddpackage("gh:jbeder/yaml-cpp#master@0.8.0")
cpmaddpackage("gh:nlohmann/json@3.12.0")

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
  VERSION 1.88.0
  GIT_REPOSITORY https://github.com/boostorg/hana.git
  GIT_TAG boost-1.88.0
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
  # hack to create version file
  configure_file(
    ${smooth_SOURCE_DIR}/config/version.hpp.in
    ${smooth_SOURCE_DIR}/include/smooth/version.hpp @ONLY
  )
endif()

cpmaddpackage(
  NAME magic_enum
  VERSION v0.9.7
  GIT_REPOSITORY https://github.com/Neargye/magic_enum.git
  GIT_TAG v0.9.7
  DOWNLOAD_ONLY YES
)
if(magic_enum_ADDED)
  add_library(magic_enum INTERFACE IMPORTED)
  target_include_directories(magic_enum INTERFACE ${magic_enum_SOURCE_DIR}/include)
endif()
