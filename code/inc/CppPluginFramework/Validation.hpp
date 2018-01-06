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

#ifndef CPPPLUGINFRAMEWORK_VALIDATION_HPP
#define CPPPLUGINFRAMEWORK_VALIDATION_HPP

// C++ Plugin Framework includes

// Qt includes
#include <QtCore/QSet>

// System includes

// Forward declarations

// Macros

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
bool validatePluginInstanceName(const QString &name);

/*!
 * Validates plugin's version
 *
 * \param   version     Plugin's version
 *
 * \retval  true    Plugin's version is valid
 * \retval  false   Plugin's version is not valid
 */
bool validateVersion(const QString &version);

/*!
 * Validates plugin's version range
 *
 * \param   minVersion  Minimum version
 * \param   maxVersion  Maximum version
 *
 * \retval  true    Plugin's version range is valid
 * \retval  false   Plugin's version range is not valid
 */
bool validateVersionRange(const QString &minVersion, const QString &maxVersion);

/*!
 * Checks if specified version is in version range
 *
 * \param   version     Version to check
 * \param   minVersion  Minimum version
 * \param   maxVersion  Maximum version
 *
 * \retval  true    Version is in range
 * \retval  false   Version is in not range
 */
bool isVersionInRange(const QString &version, const QString &minVersion, const QString &maxVersion);

/*!
 * Validates plugin's interface name
 *
 * \param   name    Plugin's interface name
 *
 * \retval  true    Plugin's interface name is valid
 * \retval  false   Plugin's interface name is not valid
 */
bool validateInterfaceName(const QString &name);

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
bool validateExportedInterfaces(const QSet<QString> &exportedInterfaces);

/*!
 * Validates the environment variable name
 *
 * \param   name    Environment variable name
 *
 * \retval  true    Valid
 * \retval  false   Invalid
 */
bool validateEnvironmentVariableName(const QString &name);

/*!
 * Validates file path
 *
 * \param   filePath    File path to validate
 *
 * \retval  true    Valid
 * \retval  false   Invalid
 */
bool validateFilePath(const QString &filePath);

}
}

#endif // CPPPLUGINFRAMEWORK_VALIDATION_HPP
