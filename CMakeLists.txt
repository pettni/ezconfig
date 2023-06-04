cmake_minimum_required(VERSION 3.15)
project(ezconfig VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_TESTS "Build the tests." OFF)

# ---------------------------------------------------------------------------------------
# TARGETS
# ---------------------------------------------------------------------------------------

add_library(ezconfig INTERFACE)
target_include_directories(
  ezconfig INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                     $<INSTALL_INTERFACE:include>
)

# ---------------------------------------------------------------------------------------
# INSTALLATION
# ---------------------------------------------------------------------------------------

include(GNUInstallDirs)

# Header files
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Targets
install(
  TARGETS ezconfig
  EXPORT ${PROJECT_NAME}_targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# ---------------------------------------------------------------------------------------
# CMAKE CONFIGURATION
# ---------------------------------------------------------------------------------------

set(EXPORT_DEST_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

# Generate TARGETS files
install(
  EXPORT ${PROJECT_NAME}_targets
  FILE ${PROJECT_NAME}Targets.cmake
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION ${EXPORT_DEST_DIR}
)

# Generate CONFIG file
include(CMakePackageConfigHelpers)
configure_package_config_file(
  cmake/ConfigTemplate.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
  INSTALL_DESTINATION ${EXPORT_DEST_DIR}
  NO_SET_AND_CHECK_MACRO
)

# Generate VERSION file
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Version.cmake
  VERSION ${PACKAGE_VERSION}
  COMPATIBILITY SameMajorVersion
)

# Install CONFIG and VERSION files
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Version.cmake
        DESTINATION ${EXPORT_DEST_DIR}
)

# ---------------------------------------------------------------------------------------
# TESTING
# ---------------------------------------------------------------------------------------

if(BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
