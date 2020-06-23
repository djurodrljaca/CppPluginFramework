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

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Test REQUIRED)

# --------------------------------------------------------------------------------------------------
# Custom (meta) targets
# --------------------------------------------------------------------------------------------------
add_custom_target(all_tests)

# --------------------------------------------------------------------------------------------------
# Helper methods
# --------------------------------------------------------------------------------------------------
function(CppPluginFramework_AddTest)
    # Function parameters
    set(options)                # Boolean parameters
    set(oneValueParams          # Parameters with one value
            TEST_NAME
        )
    set(multiValueParams        # Parameters with multiple values
            ADDITIONAL_SOURCES
            ADDITIONAL_HEADERS
            ADDITIONAL_LIBS
            LABELS
        )

    cmake_parse_arguments(PARAM "${options}" "${oneValueParams}" "${multiValueParams}" ${ARGN})

    # Create test executable
    add_executable(${PARAM_TEST_NAME}
            ${PARAM_TEST_NAME}.cpp
            ${PARAM_ADDITIONAL_SOURCES}
            ${PARAM_ADDITIONAL_HEADERS}
        )

    target_include_directories(${PARAM_TEST_NAME} PUBLIC
            ${CMAKE_CURRENT_BINARY_DIR}
        )

    target_link_libraries(${PARAM_TEST_NAME}
            PUBLIC CppPluginFramework
            PUBLIC Qt5::Test
            PUBLIC ${PARAM_ADDITIONAL_LIBS}
        )

    add_test(${PARAM_TEST_NAME} ${PARAM_TEST_NAME})

    # Add test to target "all_tests"
    add_dependencies(all_tests ${PARAM_TEST_NAME})

    # Set test labels
    if (PARAM_LABELS)
        set_tests_properties(${PARAM_TEST_NAME} PROPERTIES
                LABELS "CPPPLUGINFRAMEWORK;${PARAM_LABELS}"
            )
    endif()
endfunction()

# --------------------------------------------------------------------------------------------------
# Tests
# --------------------------------------------------------------------------------------------------
add_subdirectory(unit)
add_subdirectory(integration)

# --------------------------------------------------------------------------------------------------
# Code Coverage
# --------------------------------------------------------------------------------------------------
if (CppPluginFramework_CodeCoverage MATCHES ON)
    # Prepare targets for code coverage
    SETUP_TARGET_FOR_COVERAGE(
            NAME         ctest_coverage_all_tests
            EXECUTABLE   ctest
            DEPENDENCIES all_tests
            PROJECT_PATH ${PROJECT_SOURCE_DIR}
            EXCLUDES     ${CppPluginFramework_CodeCoverage_EXCLUDES}
        )
endif()
