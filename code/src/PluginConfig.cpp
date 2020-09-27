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
 * Contains a config class for a Plugin
 */

// Own header
#include <CppPluginFramework/PluginConfig.hpp>

// C++ Plugin Framework includes
#include <CppPluginFramework/LoggingCategories.hpp>
#include <CppPluginFramework/Validation.hpp>

// Qt includes
#include <QtCore/QStringBuilder>

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppPluginFramework
{

PluginConfig::PluginConfig(const QString &filePath,
                           const VersionInfo &version,
                           const QList<PluginInstanceConfig> &instanceConfigs)
    : m_filePath(filePath),
      m_version(version),
      m_minVersion(),
      m_maxVersion(),
      m_instanceConfigs(instanceConfigs)
{
}

// -------------------------------------------------------------------------------------------------

PluginConfig::PluginConfig(const QString &filePath,
                           const VersionInfo &minVersion,
                           const VersionInfo &maxVersion,
                           const QList<PluginInstanceConfig> &instanceConfigs)
    : m_filePath(filePath),
      m_version(),
      m_minVersion(minVersion),
      m_maxVersion(maxVersion),
      m_instanceConfigs(instanceConfigs)
{
}

// -------------------------------------------------------------------------------------------------

bool PluginConfig::isValid() const
{
    return validateConfig().isEmpty();
}

// -------------------------------------------------------------------------------------------------

bool PluginConfig::isExactVersion() const
{
    return ((!m_version.isNull()) && m_minVersion.isNull() && m_maxVersion.isNull());
}

// -------------------------------------------------------------------------------------------------

bool PluginConfig::isVersionRange() const
{
    return (m_version.isNull() && (!m_minVersion.isNull()) && (!m_maxVersion.isNull()));
}

// -------------------------------------------------------------------------------------------------

QString PluginConfig::filePath() const
{
    return m_filePath;
}

// -------------------------------------------------------------------------------------------------

void PluginConfig::setFilePath(const QString &filePath)
{
    m_filePath = filePath;
}

// -------------------------------------------------------------------------------------------------

VersionInfo PluginConfig::version() const
{
    return m_version;
}

// -------------------------------------------------------------------------------------------------

void PluginConfig::setVersion(const VersionInfo &version)
{
    m_version = version;
}

// -------------------------------------------------------------------------------------------------

VersionInfo PluginConfig::minVersion() const
{
    return m_minVersion;
}

// -------------------------------------------------------------------------------------------------

void PluginConfig::setMinVersion(const VersionInfo &minVersion)
{
    m_minVersion = minVersion;
}

// -------------------------------------------------------------------------------------------------

VersionInfo PluginConfig::maxVersion() const
{
    return m_maxVersion;
}

// -------------------------------------------------------------------------------------------------

void PluginConfig::setMaxVersion(const VersionInfo &maxVersion)
{
    m_maxVersion = maxVersion;
}

// -------------------------------------------------------------------------------------------------

const QList<PluginInstanceConfig> &PluginConfig::instanceConfigs() const
{
    return m_instanceConfigs;
}

// -------------------------------------------------------------------------------------------------

void PluginConfig::setInstanceConfigs(const QList<PluginInstanceConfig> &instanceConfigs)
{
    m_instanceConfigs = instanceConfigs;
}

// -------------------------------------------------------------------------------------------------

bool PluginConfig::loadConfigParameters(const CppConfigFramework::ConfigObjectNode &config)
{
    // Load file path
    if (!loadRequiredConfigParameter(&m_filePath, QStringLiteral("file_path"), config))
    {
        qCWarning(CppPluginFramework::LoggingCategory::Config)
                << "Failed to load plugin's file path!";
        return false;
    }

    // Load version
    m_version = VersionInfo();

    if (!loadOptionalConfigParameter(&m_version, QStringLiteral("version"), config))
    {
        qCWarning(CppPluginFramework::LoggingCategory::Config)
                << "Failed to load plugin's version!";
        return false;
    }

    // Load min version
    m_minVersion = VersionInfo();

    if (!loadOptionalConfigParameter(&m_minVersion, QStringLiteral("min_version"), config))
    {
        qCWarning(CppPluginFramework::LoggingCategory::Config)
                << "Failed to load plugin's min version!";
        return false;
    }

    // Load max version
    m_maxVersion = VersionInfo();

    if (!loadOptionalConfigParameter(&m_maxVersion, QStringLiteral("max_version"), config))
    {
        qCWarning(CppPluginFramework::LoggingCategory::Config)
                << "Failed to load plugin's max version";
        return false;
    }

    // Load instance configs
    if (!loadRequiredConfigContainer(&m_instanceConfigs, QStringLiteral("instances"), config))
    {
        qCWarning(CppPluginFramework::LoggingCategory::Config)
                << "Failed to load plugin's instances!";
        return false;
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

QString PluginConfig::validateConfig() const
{
    // Check file path
    if (!Validation::validateFilePath(m_filePath))
    {
        return QStringLiteral("File path is not valid: ") % m_filePath;
    }

    // Check version info
    if (isExactVersion())
    {
        if (!m_version.isValid())
        {
            return QStringLiteral("Version is not valid");
        }
    }
    else if (isVersionRange())
    {
        if (!VersionInfo::isRangeValid(m_minVersion, m_maxVersion))
        {
            return QStringLiteral("Version range is not valid");
        }
    }
    else
    {
        return QStringLiteral("Either just the version parameter needs to be set or "
                              "both min and max version parameters!");
    }

    // At least one plugin instance is required
    if (m_instanceConfigs.isEmpty())
    {
        return QStringLiteral("Plugin config does not define any plugin instances");
    }

    // Check individual instances are valid and if this plugin config contains duplicate instances
    // (instances with the same name)
    QStringList instanceNames;

    for (const auto &instanceConfig : m_instanceConfigs)
    {
        if (!instanceConfig.isValid())
        {
            return QStringLiteral("Plugin instance config is not valid: ") % instanceConfig.name();
        }

        if (instanceNames.contains(instanceConfig.name()))
        {
            return QStringLiteral("Duplicate plugin instance name: ") % instanceConfig.name();
        }

        instanceNames.append(instanceConfig.name());
    }

    return QString();
}

} // namespace CppPluginFramework

// -------------------------------------------------------------------------------------------------

bool operator==(const CppPluginFramework::PluginConfig &left,
                const CppPluginFramework::PluginConfig &right)
{
    if ((left.filePath() != right.filePath()) ||
        (left.version() != right.version()) ||
        (left.minVersion() != right.minVersion()) ||
        (left.maxVersion() != right.maxVersion()))
    {
        return false;
    }

    if (left.instanceConfigs().size() != right.instanceConfigs().size())
    {
        return false;
    }

    for (const auto &item : left.instanceConfigs())
    {
        if (!right.instanceConfigs().contains(item))
        {
            return false;
        }
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

bool operator!=(const CppPluginFramework::PluginConfig &left,
                const CppPluginFramework::PluginConfig &right)
{
    return !(left == right);
}
