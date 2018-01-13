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
# Custom (meta) targets
# --------------------------------------------------------------------------------------------------
add_custom_target(all_unit_tests)

# --------------------------------------------------------------------------------------------------
# Helper methods
# --------------------------------------------------------------------------------------------------
function(CppPluginFramework_AddUnitTest)
    # Function parameters
    set(options)                        # Boolean parameters
    set(oneValueParams      TEST_NAME)  # Parameters with one value
    set(multiValueParams)               # Parameters with multiple values

    cmake_parse_arguments(PARAM "${options}" "${oneValueParams}" "${multiValueParams}" ${ARGN})

    # Add test
    CppPluginFramework_AddTest(${ARGN} LABELS CPPPLUGINFRAMEWORK_UNITTESTS)

    # Add test to target "all_unit_tests"
    add_dependencies(all_unit_tests ${PARAM_TEST_NAME})
endfunction()

# --------------------------------------------------------------------------------------------------
# Unit tests
# --------------------------------------------------------------------------------------------------
add_subdirectory(ConfigFile)
add_subdirectory(PluginConfig)
add_subdirectory(PluginInstanceConfig)
add_subdirectory(Validation)
add_subdirectory(VersionInfo)
