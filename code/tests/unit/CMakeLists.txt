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
# Helper methods
# --------------------------------------------------------------------------------------------------
function(CppPluginFramework_AddUnitTest)
    # Function parameters
    set(options         DATA_DIR_ENABLED)
    set(oneValueArgs    TEST_NAME)
    set(multiValueArgs  LINKED_LIBS)
    cmake_parse_arguments(PARAM "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

    # Create test executable
    add_executable(${PARAM_TEST_NAME} ${PARAM_TEST_NAME}.cpp)
    target_link_libraries(${PARAM_TEST_NAME} CppPluginFramework Qt5::Test ${PARAM_LINKED_LIBS})
    add_test(${PARAM_TEST_NAME} ${PARAM_TEST_NAME})

    # Prepare a define for the test data directory path so that it can be used in the C++ code
    if(PARAM_DATA_DIR_ENABLED)
        get_filename_component(CURRENT_SOURCE_DIR_ABSOLUTE "${CURRENT_SOURCE_DIR}" ABSOLUTE)
        target_compile_definitions(${PARAM_TEST_NAME} PUBLIC
            TEST_DATA_DIR_PATH="${CURRENT_SOURCE_DIR_ABSOLUTE}/data")
    endif()
endfunction()

# --------------------------------------------------------------------------------------------------
# Unit tests
# --------------------------------------------------------------------------------------------------
add_subdirectory(PluginConfig)
add_subdirectory(PluginInstanceConfig)
add_subdirectory(Validation)
add_subdirectory(VersionInfo)

# TODO: add unit tests for: ConfigFile
