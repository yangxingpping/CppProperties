cmake_minimum_required(VERSION 3.28)

set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
  CACHE STRING "Vcpkg toolchain file")

project(libcppproperties VERSION 1.0.0 LANGUAGES CXX)

#Make sure that custom modules like FindXYZ are found
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)

option(BUILD_TESTING "Should the tests be included?" ON)

##############################################
# Declare dependencies


##############################################
# Create target and set properties

file(GLOB HeaderFiles include/cppproperties/*.h)

add_library(cppproperties INTERFACE)
#Set target properties
target_include_directories(cppproperties INTERFACE
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_compile_features(cppproperties INTERFACE cxx_std_17)

if(MSVC)
    add_custom_target(cppproperties.header SOURCES ${HeaderFiles})
endif()

#Add an alias so that library can be used inside the build tree, e.g. when testing
add_library(CppProperties::cppproperties ALIAS cppproperties)

##############################################
# Installation instructions
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/CppProperties)
install(TARGETS cppproperties
    EXPORT cppproperties-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
 )

#This is required so that the exported target has the name CppProperties and not cppproperties
set_target_properties(cppproperties PROPERTIES 
	EXPORT_NAME CppProperties
)

#Export the targets to a script
install(EXPORT cppproperties-targets
  FILE
    CppPropertiesTargets.cmake
  NAMESPACE
    CppProperties::
  DESTINATION
    ${INSTALL_CONFIGDIR}
)

#Create a ConfigVersion.cmake file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/CppPropertiesConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/CppPropertiesConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/CppPropertiesConfig.cmake
    INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

#Install the config, configversion and custom find modules
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/CppPropertiesConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/CppPropertiesConfigVersion.cmake
    DESTINATION ${INSTALL_CONFIGDIR}
)

##############################################
## Exporting from the build tree

export(EXPORT cppproperties-targets FILE ${CMAKE_CURRENT_BINARY_DIR}/CppPropertiesTargets.cmake NAMESPACE CppProperties::)

#Register package in user's package registry
export(PACKAGE CppProperties)

if(BUILD_TESTING)
enable_testing()
add_subdirectory(test)
endif()