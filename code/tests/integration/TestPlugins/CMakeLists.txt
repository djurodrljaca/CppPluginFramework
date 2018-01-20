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
set(CppPluginFramework_TestPlugin1_HEADERS
    ITestPlugin1.hpp
    TestPlugin1.hpp
    )

set(CppPluginFramework_TestPlugin1_SOURCES
    TestPlugin1.cpp
    )

add_library(TestPlugin1 SHARED
    ${CppPluginFramework_TestPlugin1_SOURCES}
    ${CppPluginFramework_TestPlugin1_HEADERS})
target_link_libraries(TestPlugin1 CppPluginFramework Qt5::Core)
target_compile_definitions(TestPlugin1 PRIVATE CPPPLUGINFRAMEWORK_BUILD_PLUGIN)
set_target_properties(TestPlugin1 PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    )

# --------------------------------------------------------------------------------------------------
# Test Plugin 2
# --------------------------------------------------------------------------------------------------
set(CppPluginFramework_TestPlugin2_HEADERS
    ITestPlugin2.hpp
    TestPlugin2.hpp
    )

set(CppPluginFramework_TestPlugin2_SOURCES
    TestPlugin2.cpp
    )

add_library(TestPlugin2 SHARED
    ${CppPluginFramework_TestPlugin2_SOURCES}
    ${CppPluginFramework_TestPlugin2_HEADERS})
target_link_libraries(TestPlugin2 CppPluginFramework Qt5::Core)
target_compile_definitions(TestPlugin2 PRIVATE CPPPLUGINFRAMEWORK_BUILD_PLUGIN)
set_target_properties(TestPlugin2 PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    )
