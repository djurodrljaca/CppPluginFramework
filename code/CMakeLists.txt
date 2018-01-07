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

cmake_minimum_required(VERSION 3.1.0)

project(CppPluginFramework VERSION 1.0.0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core)

# --------------------------------------------------------------------------------------------------
# CppPluginFramework library
# --------------------------------------------------------------------------------------------------

include_directories(inc)

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
target_compile_definitions(CppPluginFramework PUBLIC CPPPLUGINFRAMEWORK_BUILD_LIBRARY)
set_target_properties(CppPluginFramework PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

# --------------------------------------------------------------------------------------------------
# Tests
# --------------------------------------------------------------------------------------------------

enable_testing()
add_subdirectory(tests)
