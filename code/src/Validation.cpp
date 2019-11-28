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

// Own header
#include <CppPluginFramework/Validation.hpp>

// C++ Plugin Framework includes
#include <CppPluginFramework/VersionInfo.hpp>

// Qt includes
#include <QtCore/QFileInfo>
#include <QtCore/QRegularExpression>

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppPluginFramework
{

namespace Validation
{

bool validatePluginInstanceName(const QString &name)
{
    QRegularExpression regex("^[a-zA-Z][a-zA-Z0-9\\._-]*$");
    auto match = regex.match(name);

    return match.hasMatch();
}

// -------------------------------------------------------------------------------------------------

bool validateInterfaceName(const QString &name)
{
    QRegularExpression regex("^[a-zA-Z][a-zA-Z0-9]*(::[a-zA-Z][a-zA-Z0-9]*)*$");
    auto match = regex.match(name);

    return match.hasMatch();
}

// -------------------------------------------------------------------------------------------------

bool validateExportedInterfaces(const QSet<QString> &exportedInterfaces)
{
    if (exportedInterfaces.isEmpty())
    {
        return false;
    }

    for (const QString &item : exportedInterfaces)
    {
        if (!validateInterfaceName(item))
        {
            return false;
        }
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

bool validateEnvironmentVariableName(const QString &name)
{
    QRegularExpression regex("^[a-zA-Z_][a-zA-Z0-9_]*$");
    auto match = regex.match(name);

    return match.hasMatch();
}

// -------------------------------------------------------------------------------------------------

bool validateFilePath(const QString &filePath)
{
    return QFileInfo(filePath).isFile();
}

} // namespace Validation

} // namespace CppPluginFramework
