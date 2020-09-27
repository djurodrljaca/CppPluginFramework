/* This file is part of C++ Plugin Framework.
 *
 * C++ Plugin Framework is free software: you can redistribute it and/or modify it under the terms
 * of the GNU Lesser General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * C++ Plugin Framework is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with C++ Plugin
 * Framework. If not, see <http://www.gnu.org/licenses/>.
 */

/*!
 * \file
 *
 * Contains validation methods
 */

#pragma once

// C++ Plugin Framework includes
#include <CppPluginFramework/LibraryExport.hpp>

// Qt includes
#include <QtCore/QSet>

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppPluginFramework
{

namespace Validation
{

/*!
 * Validates plugin's instance name
 *
 * \param   name    Plugin's instance name
 *
 * \retval  true    Plugin's instance name is valid
 * \retval  false   Plugin's instance name is not valid
 */
CPPPLUGINFRAMEWORK_LIBRARY_EXPORT bool validatePluginInstanceName(const QString &name);

/*!
 * Validates plugin's interface name
 *
 * \param   name    Plugin's interface name
 *
 * \retval  true    Plugin's interface name is valid
 * \retval  false   Plugin's interface name is not valid
 */
CPPPLUGINFRAMEWORK_LIBRARY_EXPORT bool validateInterfaceName(const QString &name);

/*!
 * Validates plugin's exported interfaces
 *
 * \param   exportedInterfaces  Plugin's exported interface names
 *
 * \retval  true    Plugin's exported interfaces are valid
 * \retval  false   Plugin's exported interfaces are not valid
 *
 * This method checks if at least one interface is exported and if the interface names are valid.
 */
CPPPLUGINFRAMEWORK_LIBRARY_EXPORT
bool validateExportedInterfaces(const QSet<QString> &exportedInterfaces);

/*!
 * Validates the environment variable name
 *
 * \param   name    Environment variable name
 *
 * \retval  true    Valid
 * \retval  false   Invalid
 */
CPPPLUGINFRAMEWORK_LIBRARY_EXPORT bool validateEnvironmentVariableName(const QString &name);

/*!
 * Validates file path
 *
 * \param   filePath    File path to validate
 *
 * \retval  true    Valid
 * \retval  false   Invalid
 */
CPPPLUGINFRAMEWORK_LIBRARY_EXPORT bool validateFilePath(const QString &filePath);

} // namespace Validation

} // namespace CppPluginFramework
