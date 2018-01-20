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

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(Qt5Core)

# --------------------------------------------------------------------------------------------------
# CppPluginFramework library
# --------------------------------------------------------------------------------------------------
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)

set(CppPluginFramework_HEADERS
    inc/CppPluginFramework/AbstractPlugin.hpp
    inc/CppPluginFramework/ConfigFile.hpp
    inc/CppPluginFramework/IPlugin.hpp
    inc/CppPluginFramework/LibraryExport.hpp
    inc/CppPluginFramework/Plugin.hpp
    inc/CppPluginFramework/PluginConfig.hpp
    inc/CppPluginFramework/PluginExport.hpp
    inc/CppPluginFramework/PluginInstanceConfig.hpp
    inc/CppPluginFramework/PluginManager.hpp
    inc/CppPluginFramework/Validation.hpp
    inc/CppPluginFramework/VersionInfo.hpp
    )

set(CppPluginFramework_SOURCES
    src/AbstractPlugin.cpp
    src/ConfigFile.cpp
    src/Plugin.cpp
    src/PluginConfig.cpp
    src/PluginInstanceConfig.cpp
    src/PluginManager.cpp
    src/Validation.cpp
    src/VersionInfo.cpp
    )

add_library(CppPluginFramework SHARED ${CppPluginFramework_SOURCES} ${CppPluginFramework_HEADERS})
target_link_libraries(CppPluginFramework Qt5::Core)
target_compile_definitions(CppPluginFramework PRIVATE CPPPLUGINFRAMEWORK_BUILD_LIBRARY)
set_target_properties(CppPluginFramework PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    )

# --------------------------------------------------------------------------------------------------
# Code Coverage
# --------------------------------------------------------------------------------------------------
option(CppPluginFramework_CodeCoverage "C++ Plugin Framework Code Coverage" OFF)

if (CppPluginFramework_CodeCoverage MATCHES ON)
    include(CodeCoverage)
    APPEND_COVERAGE_COMPILER_FLAGS()

    # Prepare a function for appending exclude filters to COVERAGE_EXCLUDES variable
    macro(APPEND_COVERAGE_EXCLUDES start_path)
        # Iterate over all files
        file(GLOB_RECURSE items RELATIVE ${start_path} ${start_path}/*)

        foreach (item ${items})
            # Check for source files
            string(TOLOWER item item_lower)

            if (${item_lower} MATCHES "(\.cpp|\.h|\.hpp|\.moc)$")
                # Extract directory
                get_filename_component(item_dir ${start_path}/${item} DIRECTORY)

                # Prepare exclude filter
                set(exclude_filter "'${item_dir}/*'")

                # Add the exclude filter if needed
                if (NOT (exclude_filter IN_LIST COVERAGE_EXCLUDES))
                    list(APPEND COVERAGE_EXCLUDES ${exclude_filter})
                endif()
            endif()
        endforeach()
    endmacro()

    # Exclude paths from the report
    APPEND_COVERAGE_EXCLUDES("/usr/include")
    APPEND_COVERAGE_EXCLUDES("${CMAKE_CURRENT_SOURCE_DIR}/tests")
    APPEND_COVERAGE_EXCLUDES("${CMAKE_CURRENT_BINARY_DIR}")
endif()

# --------------------------------------------------------------------------------------------------
# Tests
# --------------------------------------------------------------------------------------------------
enable_testing()
add_subdirectory(tests)
