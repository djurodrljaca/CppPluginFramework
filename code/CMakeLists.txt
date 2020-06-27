# This file is part of C++ Plugin Framework.
#
# C++ Plugin Framework is free software: you can redistribute it and/or modify it under the terms
# of the GNU Lesser General Public License as published by the Free Software Foundation, either
# version 3 of the License, or (at your option) any later version.
#
# C++ Plugin Framework is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
# without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License along with C++ Plugin
# Framework. If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.3.0)

project(CppPluginFramework VERSION 1.0.0)

set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(CppConfigFramework REQUIRED)

# --------------------------------------------------------------------------------------------------
# CppPluginFramework library
# --------------------------------------------------------------------------------------------------
add_library(CppPluginFramework SHARED
        inc/CppPluginFramework/AbstractPlugin.hpp
        inc/CppPluginFramework/IPlugin.hpp
        inc/CppPluginFramework/IPluginFactory.hpp
        inc/CppPluginFramework/LibraryExport.hpp
        inc/CppPluginFramework/LoggingCategories.hpp
        inc/CppPluginFramework/Plugin.hpp
        inc/CppPluginFramework/PluginConfig.hpp
        inc/CppPluginFramework/PluginExport.hpp
        inc/CppPluginFramework/PluginFactoryTemplate.hpp
        inc/CppPluginFramework/PluginInstanceConfig.hpp
        inc/CppPluginFramework/PluginManager.hpp
        inc/CppPluginFramework/PluginManagerConfig.hpp
        inc/CppPluginFramework/Validation.hpp
        inc/CppPluginFramework/VersionInfo.hpp

        src/AbstractPlugin.cpp
        src/LoggingCategories.cpp
        src/Plugin.cpp
        src/PluginConfig.cpp
        src/PluginInstanceConfig.cpp
        src/PluginManager.cpp
        src/PluginManagerConfig.cpp
        src/Validation.cpp
        src/VersionInfo.cpp
    )

target_include_directories(CppPluginFramework PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
        $<INSTALL_INTERFACE:include>
    )

target_link_libraries(CppPluginFramework PUBLIC
        CppConfigFramework::CppConfigFramework
    )

target_compile_definitions(CppPluginFramework PRIVATE
        CPPPLUGINFRAMEWORK_BUILD_LIBRARY
    )

set_target_properties(CppPluginFramework PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
    )

# --------------------------------------------------------------------------------------------------
# Package
# --------------------------------------------------------------------------------------------------
add_library(CppPluginFramework::CppPluginFramework ALIAS CppPluginFramework)

install(TARGETS CppPluginFramework EXPORT CppPluginFrameworkTargets DESTINATION
        LIBRARY  DESTINATION lib
        ARCHIVE  DESTINATION lib
        RUNTIME  DESTINATION bin
        INCLUDES DESTINATION include
    )

install(
        DIRECTORY   ${CMAKE_CURRENT_SOURCE_DIR}/inc/CppPluginFramework
        DESTINATION include
        COMPONENT   Devel
    )

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/CppPluginFramework/CppPluginFrameworkConfigVersion.cmake"
        VERSION ${CppPluginFramework_VERSION}
        COMPATIBILITY AnyNewerVersion
    )

export(EXPORT CppPluginFrameworkTargets
        FILE "${CMAKE_CURRENT_BINARY_DIR}/CppPluginFramework/CppPluginFrameworkTargets.cmake"
        NAMESPACE CppPluginFramework::
    )

configure_file(cmake/CppPluginFrameworkConfig.cmake
        "${CMAKE_CURRENT_BINARY_DIR}/CppPluginFramework/CppPluginFrameworkConfig.cmake"
        COPYONLY
    )

set(ConfigPackageLocation lib/cmake/CppPluginFramework)

install(
        EXPORT CppPluginFrameworkTargets
        FILE        CppPluginFrameworkTargets.cmake
        NAMESPACE   CppPluginFramework::
        DESTINATION ${ConfigPackageLocation}
    )

install(
        FILES
            cmake/CppPluginFrameworkConfig.cmake
            "${CMAKE_CURRENT_BINARY_DIR}/CppPluginFramework/CppPluginFrameworkConfigVersion.cmake"
        DESTINATION ${ConfigPackageLocation}
        COMPONENT   Devel
    )

# --------------------------------------------------------------------------------------------------
# Code Coverage
# --------------------------------------------------------------------------------------------------
option(CppPluginFramework_CodeCoverage "C++ Plugin Framework Code Coverage" OFF)

if (CppPluginFramework_CodeCoverage MATCHES ON)
    include(cmake/CodeCoverage.cmake)
    APPEND_COVERAGE_COMPILER_FLAGS()

    set(excludes_cmake_prefix_path)
    foreach(item ${CMAKE_PREFIX_PATH})
        get_filename_component(item ${item} ABSOLUTE)
        list(APPEND excludes_cmake_prefix_path "'${item}/*'")
    endforeach()

    set(CppPluginFramework_CodeCoverage_EXCLUDES
            "'/opt/*'"
            "'/usr/*'"
            ${excludes_cmake_prefix_path}
            "'${CMAKE_SOURCE_DIR}/tests/*'"
            "'${CMAKE_BINARY_DIR}/*'"
        )
endif()

# --------------------------------------------------------------------------------------------------
# Tests
# --------------------------------------------------------------------------------------------------
enable_testing()
add_subdirectory(tests)
