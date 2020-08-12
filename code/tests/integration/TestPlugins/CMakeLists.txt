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

# --------------------------------------------------------------------------------------------------
# Test Plugin 1
# --------------------------------------------------------------------------------------------------
add_library(TestPlugin1 SHARED
        ITestPlugin1.hpp
        TestPlugin1.hpp
        TestPlugin1.cpp
    )

target_include_directories(TestPlugin1
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    )

target_link_libraries(TestPlugin1
        PUBLIC CppPluginFramework
        PUBLIC Qt5::Core
    )

target_compile_definitions(TestPlugin1
        PRIVATE CPPPLUGINFRAMEWORK_BUILD_PLUGIN
    )

set_target_properties(TestPlugin1 PROPERTIES
        PREFIX ""
        SUFFIX ".plugin"
    )

# --------------------------------------------------------------------------------------------------
# Test Plugin 2
# --------------------------------------------------------------------------------------------------
add_library(TestPlugin2 SHARED
        ITestPlugin2.hpp
        TestPlugin2.hpp
        TestPlugin2.cpp
    )

target_include_directories(TestPlugin2
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    )

target_link_libraries(TestPlugin2
        PUBLIC CppPluginFramework
        PUBLIC Qt5::Core
    )

target_compile_definitions(TestPlugin2
        PRIVATE CPPPLUGINFRAMEWORK_BUILD_PLUGIN
    )

set_target_properties(TestPlugin2 PROPERTIES
        PREFIX ""
        SUFFIX ".plugin"
    )
