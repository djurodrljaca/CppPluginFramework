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

// C++ Plugin Framework includes
#include <CppPluginFramework/Validation.hpp>

// Qt includes
#include <QtCore/QFileInfo>
#include <QtCore/QRegularExpression>
#include <QtCore/QStringBuilder>

// System includes

// Forward declarations

// Macros

namespace CppPluginFramework
{
namespace Validation
{

/*!
 * Structure that holds version information
 */
struct VersionInfo
{
    /*!
     * Major version number
     */
    quint32 major;

    /*!
     * Minor version number
     */
    quint32 minor;

    /*!
     * Patch version number
     */
    quint32 patch;

    /*!
     * Development version number
     */
    QString dev;
};

/*!
 * Checks if the two specified versions are equal
 *
 * \param   left    Version
 * \param   right   Version
 *
 * \retval  true    Versions are equal
 * \retval  false   Versions are not equal
 */
static bool operator==(const VersionInfo &left, const VersionInfo &right)
{
    return ((left.major == right.major) &&
            (left.minor == right.minor) &&
            (left.patch == right.patch) &&
            (left.dev   == right.dev));
}

/*!
 * Checks if the one version is smaller than the other
 *
 * \param   left    Version
 * \param   right   Version
 *
 * \retval  true    Version "left" is smaller than version "right"
 * \retval  false   Version "left" is not smaller than version "right"
 */
static bool operator<(const VersionInfo &left, const VersionInfo &right)
{
    bool smaller = false;
    bool finished = false;

    // Check Major version part
    if (left.major < right.major)
    {
        // Object "left" is smaller than object "right"
        smaller = true;
        finished = true;
    }
    else if (left.major == right.major)
    {
        // Object "left" could still be smaller than object "right"
        finished = false;
    }
    else
    {
        // Object "left" is greater than object "right"
        smaller = false;
        finished = true;
    }

    // Check Minor version part
    if (!finished)
    {
        if (left.minor < right.minor)
        {
            // Object "left" is smaller than object "right"
            smaller = true;
            finished = true;
        }
        else if (left.minor == right.minor)
        {
            // Object "left" could still be smaller than object "right"
            finished = false;
        }
        else
        {
            // Object "left" is greater than object "right"
            smaller = false;
            finished = true;
        }
    }

    // Check Patch version part
    if (!finished)
    {
        if (left.patch < right.patch)
        {
            // Object "left" is smaller than object "right"
            smaller = true;
            finished = true;
        }
        else if (left.patch == right.patch)
        {
            // Object "left" could still be smaller than object "right"
            finished = false;
        }
        else
        {
            // Object "left" is greater than object "right"
            smaller = false;
            finished = true;
        }
    }

    // Check Dev version part
    if (!finished)
    {
        smaller = (left.dev < right.dev);
    }

    return smaller;
}

/*!
 * Checks if the one version is smaller than or equal to the other
 *
 * \param   left    Version
 * \param   right   Version
 *
 * \retval  true    Version "left" is smaller than or equal to version "right"
 * \retval  false   Version "left" is not smaller than or equal to version "right"
 */
static bool operator<=(const VersionInfo &left, const VersionInfo &right)
{
    return ((left == right) || (left < right));
}

/*!
 * Parses the version information
 *
 * \param       version     Version in string format
 * \param[out]  versionInfo Version info
 *
 * \retval  true    Success
 * \retval  false   Failure
 */
static bool parseVersionInfo(const QString &version, VersionInfo *versionInfo)
{
    bool success = false;

    // Parse version info by first extracting the individual parts of the version
    // Major, Minor, and Patch parts are also converted to integers
    if (versionInfo != nullptr)
    {
        QRegularExpression regex("^(?<major>[0-9]+)\\.(?<minor>[0-9]+)(\\.(?<patch>[0-9]+))?"
                                 "(-(?<dev>[0-9a-zA-Z][0-9a-zA-Z\\._-]*))?$");

        auto match = regex.match(version);

        if (match.hasMatch())
        {
            const QString major = match.captured("major");
            const QString minor = match.captured("minor");
            const QString patch = match.captured("patch");
            const QString dev = match.captured("dev");

            if ((!major.isEmpty()) && (!minor.isEmpty()))
            {
                // Major
                versionInfo->major = major.toUInt(&success);

                // Minor
                if (success)
                {
                    versionInfo->minor = minor.toUInt(&success);
                }

                // Build
                if (success)
                {
                    if (patch.isEmpty())
                    {
                        versionInfo->patch = 0u;
                    }
                    else
                    {
                        versionInfo->patch = patch.toUInt(&success);
                    }
                }

                // Dev
                if (success)
                {
                    versionInfo->dev = dev;
                }
            }
        }
    }

    return success;
}

bool validatePluginInstanceName(const QString &name)
{
    QRegularExpression regex("^[a-zA-Z][a-zA-Z0-9\\._-]*$");
    auto match = regex.match(name);

    return match.hasMatch();
}

bool validateVersion(const QString &version)
{
    QRegularExpression regex("^[0-9]+\\.[0-9]+(\\.[0-9]+)?(-[0-9a-zA-Z][0-9a-zA-Z\\._-]*)?$");
    auto match = regex.match(version);

    return match.hasMatch();
}

bool validateVersionRange(const QString &minVersion, const QString &maxVersion)
{
    // Parse min version
    VersionInfo minVersionInfo;
    bool valid = parseVersionInfo(minVersion, &minVersionInfo);

    // Parse max version
    VersionInfo maxVersionInfo;

    if (valid)
    {
        valid = parseVersionInfo(maxVersion, &maxVersionInfo);
    }

    // Check if version range is valid
    if (valid)
    {
        valid = (minVersionInfo <= maxVersionInfo);
    }

    return valid;
}

bool isVersionInRange(const QString &version, const QString &minVersion, const QString &maxVersion)
{
    // Parse version
    VersionInfo versionInfo;
    bool success = parseVersionInfo(version, &versionInfo);

    // Parse min version
    VersionInfo minVersionInfo;

    if (success)
    {
        success = parseVersionInfo(minVersion, &minVersionInfo);
    }

    // Parse max version
    VersionInfo maxVersionInfo;

    if (success)
    {
        success = parseVersionInfo(maxVersion, &maxVersionInfo);
    }

    // Compare versions
    bool inRange = false;

    if (success)
    {
        inRange = ((minVersionInfo <= versionInfo) && (versionInfo < maxVersionInfo));
    }

    return inRange;
}

bool validateInterfaceName(const QString &name)
{
    QRegularExpression regex("^[a-zA-Z][a-zA-Z0-9]*(::[a-zA-Z][a-zA-Z0-9]*)*$");
    auto match = regex.match(name);

    return match.hasMatch();
}

bool validateExportedInterfaces(const QSet<QString> &exportedInterfaces)
{
    bool valid = false;

    for (const QString &item : exportedInterfaces)
    {
        valid = validateInterfaceName(item);

        if (!valid)
        {
            break;
        }
    }

    return valid;
}

bool validateEnvironmentVariableName(const QString &name)
{
    QRegularExpression regex("^[a-zA-Z0-9_]+$");
    auto match = regex.match(name);

    return match.hasMatch();
}

bool validateFilePath(const QString &filePath)
{
    return QFileInfo(filePath).isFile();
}

}
}
