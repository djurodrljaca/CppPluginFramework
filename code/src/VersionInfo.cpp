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
 * Contains a class that holds version information
 */

// Own header
#include <CppPluginFramework/VersionInfo.hpp>

// C++ Plugin Framework includes
#include <CppPluginFramework/Validation.hpp>

// Qt includes
#include <QtCore/QFileInfo>
#include <QtCore/QRegularExpression>

// System includes

// Forward declarations

// Macros

namespace CppPluginFramework
{

VersionInfo::VersionInfo()
    : m_major(-1),
      m_minor(-1),
      m_patch(-1),
      m_dev()
{
}

// -------------------------------------------------------------------------------------------------

VersionInfo::VersionInfo(const int major, const int minor, const int patch, const QString &dev)
    : m_major(major),
      m_minor(minor),
      m_patch(patch),
      m_dev(dev)
{
}

// -------------------------------------------------------------------------------------------------

VersionInfo::VersionInfo(const QString &version)
    : m_major(-1),
      m_minor(-1),
      m_patch(-1),
      m_dev()
{
    // Parse version info by first extracting the individual parts of the version
    // Major, Minor, and Patch parts are also converted to integers
    const QRegularExpression regex(
                "^(?<major>[0-9]+)\\.(?<minor>[0-9]+)\\.(?<patch>[0-9]+)(-(?<dev>.+))?$");

    auto match = regex.match(version);

    if (match.hasMatch())
    {
        const QString major = match.captured("major");
        const QString minor = match.captured("minor");
        const QString patch = match.captured("patch");
        const QString dev = match.captured("dev");

        if ((!major.isEmpty()) && (!minor.isEmpty()) && (!patch.isEmpty()))
        {
            bool success = false;

            // Major
            m_major = major.toInt(&success);

            // Minor
            if (success)
            {
                m_minor = minor.toInt(&success);
            }

            // Patch
            if (success)
            {
                m_patch = patch.toInt(&success);
            }

            // Dev
            if (success)
            {
                m_dev = dev;
            }
        }
    }
}

// -------------------------------------------------------------------------------------------------

bool VersionInfo::isNull() const
{
    return (*this == VersionInfo());
}

// -------------------------------------------------------------------------------------------------

bool VersionInfo::isValid() const
{
    return ((m_major >= 0) && (m_minor >= 0) && (m_patch >= 0));
}

// -------------------------------------------------------------------------------------------------

int VersionInfo::major() const
{
    return m_major;
}

// -------------------------------------------------------------------------------------------------

void VersionInfo::setMajor(const int &major)
{
    m_major = major;
}

// -------------------------------------------------------------------------------------------------

int VersionInfo::minor() const
{
    return m_minor;
}

// -------------------------------------------------------------------------------------------------

void VersionInfo::setMinor(const int &minor)
{
    m_minor = minor;
}

// -------------------------------------------------------------------------------------------------

int VersionInfo::patch() const
{
    return m_patch;
}

// -------------------------------------------------------------------------------------------------

void VersionInfo::setPatch(const int &patch)
{
    m_patch = patch;
}

// -------------------------------------------------------------------------------------------------

QString VersionInfo::dev() const
{
    return m_dev;
}

// -------------------------------------------------------------------------------------------------

void VersionInfo::setDev(const QString &dev)
{
    m_dev = dev;
}

// -------------------------------------------------------------------------------------------------

QString VersionInfo::toString() const
{
    QString version;

    if (isValid())
    {
        version = QString("%1.%2.%3").arg(m_major).arg(m_minor).arg(m_patch);

        if (!m_dev.isEmpty())
        {
            version.append('-').append(m_dev);
        }
    }

    return version;
}

// -------------------------------------------------------------------------------------------------

bool VersionInfo::isRangeValid(const VersionInfo &minVersion, const VersionInfo &maxVersion)
{
    return minVersion.isValid() && maxVersion.isValid() && (minVersion < maxVersion);
}

// -------------------------------------------------------------------------------------------------

bool VersionInfo::isVersionInRange(const VersionInfo &version,
                                   const VersionInfo &minVersion,
                                   const VersionInfo &maxVersion)
{
    bool inRange = false;

    if (version.isValid() && minVersion.isValid() && maxVersion.isValid())
    {
        inRange = ((minVersion<= version) && (version< maxVersion));
    }

    return inRange;
}

// -------------------------------------------------------------------------------------------------

bool operator==(const VersionInfo &left, const VersionInfo &right)
{
    return ((left.major() == right.major()) &&
            (left.minor() == right.minor()) &&
            (left.patch() == right.patch()) &&
            (left.dev() == right.dev()));
}

// -------------------------------------------------------------------------------------------------

bool operator<(const VersionInfo &left, const VersionInfo &right)
{
    // Check Major version part
    if (left.major() < right.major())
    {
        // Object "left" is smaller than object "right"
        return true;
    }

    if (left.major() > right.major())
    {
        // Object "left" is greater than object "right"
        return false;
    }

    // Check Minor version part
    if (left.minor() < right.minor())
    {
        // Object "left" is smaller than object "right"
        return true;
    }

    if (left.minor() > right.minor())
    {
        // Object "left" is greater than object "right"
        return false;
    }

    // Check Patch version part
    if (left.patch() < right.patch())
    {
        // Object "left" is smaller than object "right"
        return true;
    }

    if (left.patch() > right.patch())
    {
        // Object "left" is greater than object "right"
        return false;
    }

    // Check Dev version part
    if (left.dev().isEmpty() && right.dev().isEmpty())
    {
        // Object "left" is equal to object "right"
        return false;
    }

    if (left.dev().isEmpty() && (!right.dev().isEmpty()))
    {
        // Object "left" is smaller than object "right"
        return true;
    }

    if ((!left.dev().isEmpty()) && right.dev().isEmpty())
    {
        // Object "left" is greater than object "right"
        return false;
    }

    return (left.dev() < right.dev());
}

// -------------------------------------------------------------------------------------------------

bool operator<=(const VersionInfo &left, const VersionInfo &right)
{
    return ((left == right) || (left < right));
}

// -------------------------------------------------------------------------------------------------

bool operator>(const VersionInfo &left, const VersionInfo &right)
{
    return !(left <= right);
}

// -------------------------------------------------------------------------------------------------

bool operator>=(const VersionInfo &left, const VersionInfo &right)
{
    return !(left < right);
}

}
