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
add_custom_target(all_integration_tests)

# --------------------------------------------------------------------------------------------------
# Helper methods
# --------------------------------------------------------------------------------------------------
function(CppPluginFramework_AddIntegrationTest)
    # Function parameters
    set(options)                # Boolean parameters
    set(oneValueParams          # Parameters with one value
            TEST_NAME
        )
    set(multiValueParams)       # Parameters with multiple values

    cmake_parse_arguments(PARAM "${options}" "${oneValueParams}" "${multiValueParams}" ${ARGN})

    # Add test
    CppPluginFramework_AddTest(${ARGN} LABELS CPPPLUGINFRAMEWORK_INTEGRATION_TESTS)

    # Add test to target "all_integration_tests"
    add_dependencies(all_integration_tests ${PARAM_TEST_NAME})
endfunction()

# --------------------------------------------------------------------------------------------------
# Test plugins
# --------------------------------------------------------------------------------------------------
add_subdirectory(TestPlugins)

# --------------------------------------------------------------------------------------------------
# Integration tests
# --------------------------------------------------------------------------------------------------
add_subdirectory(Plugin)
add_subdirectory(PluginManager)

# --------------------------------------------------------------------------------------------------
# Code Coverage
# --------------------------------------------------------------------------------------------------
if (CppPluginFramework_CodeCoverage MATCHES ON)
    # Prepare targets for code coverage
    SETUP_TARGET_FOR_COVERAGE(
            NAME         ctest_coverage_all_integration_tests
            EXECUTABLE   ctest -L CPPPLUGINFRAMEWORK_INTEGRATION_TESTS
            DEPENDENCIES all_integration_tests
            PROJECT_PATH ${PROJECT_SOURCE_DIR}
            EXCLUDES     ${CppPluginFramework_CodeCoverage_EXCLUDES}
        )
endif()
