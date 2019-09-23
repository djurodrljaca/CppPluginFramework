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

#ifndef CPPPLUGINFRAMEWORK_VERSIONINFO_HPP
#define CPPPLUGINFRAMEWORK_VERSIONINFO_HPP

// C++ Plugin Framework includes
#include <CppPluginFramework/LibraryExport.hpp>

// Qt includes
#include <QtCore/QString>

// System includes

// Forward declarations

// Macros

// These two symbols need to be undefined to avoid conflict with macros from <sys/sysmacros.h>
#undef major
#undef minor

namespace CppPluginFramework
{

/*!
 * This class holds version information
 */
class CPPPLUGINFRAMEWORK_LIBRARY_EXPORT VersionInfo
{
public:
    /*!
     * Constructor
     */
    VersionInfo();

    /*!
     * Constructor
     *
     * \param   major   Major version number
     * \param   minor   Minor version number
     * \param   patch   Patch version number
     * \param   dev     Optional development version string
     */
    VersionInfo(const int major, const int minor, const int patch, const QString &dev = QString());

    /*!
     * Constructor
     *
     * \param   version     String representation of the version
     */
    VersionInfo(const QString &version);

    /*!
     * Checks if version information is null (default constructed value)
     *
     * \retval  true    Null
     * \retval  false   Not null
     */
    bool isNull() const;

    /*!
     * Checks if version information is valid
     *
     * \retval  true    Valid
     * \retval  false   Invalid
     */
    bool isValid() const;

    /*!
     * Returns major version number
     *
     * \return  Major version number
     */
    int major() const;

    /*!
     * Sets major version number
     *
     * \param   major   Major version number
     */
    void setMajor(const int &major);

    /*!
     * Returns minor version number
     *
     * \return  Minor version number
     */
    int minor() const;

    /*!
     * Sets minor version number
     *
     * \param   minor   Minor version number
     */
    void setMinor(const int &minor);

    /*!
     * Returns patch version number
     *
     * \return  Patch version number
     */
    int patch() const;

    /*!
     * Sets patch version number
     *
     * \param   patch   Patch version number
     */
    void setPatch(const int &patch);

    /*!
     * Returns development version string
     *
     * \return  Development version string
     */
    QString dev() const;

    /*!
     * Sets development version string
     *
     * \param   dev     Development version string
     */
    void setDev(const QString &dev);

    /*!
     * Generates a string representation of the version
     *
     * \return  String representation of the version
     */
    QString toString() const;

    /*!
     * Checks if version range is valid
     *
     * \param   minVersion  Minimum version
     * \param   maxVersion  Maximum version
     *
     * \retval  true    Valid
     * \retval  false   Invalid
     */
    static bool isRangeValid(const VersionInfo &minVersion, const VersionInfo &maxVersion);

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
    static bool isVersionInRange(const VersionInfo &version,
                                 const VersionInfo &minVersion,
                                 const VersionInfo &maxVersion);

private:
    /*!
     * Major version number
     */
    int m_major;

    /*!
     * Minor version number
     */
    int m_minor;

    /*!
     * Patch version number
     */
    int m_patch;

    /*!
     * Development version number
     */
    QString m_dev;
};

/*!
 * Checks if the two specified versions are equal
 *
 * \param   left    Version
 * \param   right   Version
 *
 * \retval  true    Versions are equal
 * \retval  false   Versions are not equal
 *
 * \note    Result of the comparison is valid only if both versions are valid!
 */
CPPPLUGINFRAMEWORK_LIBRARY_EXPORT bool operator==(const VersionInfo &left,
                                                  const VersionInfo &right);

/*!
 * Checks if the one version is smaller than the other
 *
 * \param   left    Version
 * \param   right   Version
 *
 * \retval  true    Version "left" is smaller than version "right"
 * \retval  false   Version "left" is not smaller than version "right"
 *
 * \note    Result of the comparison is valid only if both versions are valid!
 */
CPPPLUGINFRAMEWORK_LIBRARY_EXPORT bool operator<(const VersionInfo &left, const VersionInfo &right);

/*!
 * Checks if the one version is smaller than or equal to the other
 *
 * \param   left    Version
 * \param   right   Version
 *
 * \retval  true    Version "left" is smaller than or equal to version "right"
 * \retval  false   Version "left" is not smaller than or equal to version "right"
 *
 * \note    Result of the comparison is valid only if both versions are valid!
 */
CPPPLUGINFRAMEWORK_LIBRARY_EXPORT bool operator<=(const VersionInfo &left,
                                                  const VersionInfo &right);

/*!
 * Checks if the one version is bigger than the other
 *
 * \param   left    Version
 * \param   right   Version
 *
 * \retval  true    Version "left" is bigger than version "right"
 * \retval  false   Version "left" is not bigger than version "right"
 *
 * \note    Result of the comparison is valid only if both versions are valid!
 */
CPPPLUGINFRAMEWORK_LIBRARY_EXPORT bool operator>(const VersionInfo &left,
                                                 const VersionInfo &right);

/*!
 * Checks if the one version is bigger than or equal to the other
 *
 * \param   left    Version
 * \param   right   Version
 *
 * \retval  true    Version "left" is bigger than or equal to version "right"
 * \retval  false   Version "left" is not bigger than or equal to version "right"
 *
 * \note    Result of the comparison is valid only if both versions are valid!
 */
CPPPLUGINFRAMEWORK_LIBRARY_EXPORT bool operator>=(const VersionInfo &left,
                                                  const VersionInfo &right);

}

#endif // CPPPLUGINFRAMEWORK_VERSIONINFO_HPP
