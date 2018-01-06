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

set(HEADERS
    inc/CppPluginFramework/AbstractPlugin.hpp
    inc/CppPluginFramework/ConfigFile.hpp
    inc/CppPluginFramework/IPlugin.hpp
    inc/CppPluginFramework/Plugin.hpp
    inc/CppPluginFramework/PluginConfig.hpp
    inc/CppPluginFramework/PluginInstanceConfig.hpp
    inc/CppPluginFramework/PluginManager.hpp
    inc/CppPluginFramework/PluginSharedExport.hpp
    inc/CppPluginFramework/Validation.hpp
    )

set(SOURCES
    src/AbstractPlugin.cpp
    src/ConfigFile.cpp
    src/Plugin.cpp
    src/PluginConfig.cpp
    src/PluginInstanceConfig.cpp
    src/PluginManager.cpp
    src/Validation.cpp
    )

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC inc)
target_link_libraries(${PROJECT_NAME} Qt5::Core)
target_compile_definitions(${PROJECT_NAME} PUBLIC CPPPLUGINFRAMEWORK_PLUGIN)
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
