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
 * Contains a config class for the Plugin Manager
 */

// Own header
#include <CppPluginFramework/PluginManagerConfig.hpp>

// C++ Plugin Framework includes
#include <CppPluginFramework/LoggingCategories.hpp>

// Qt includes
#include <QtCore/QStringBuilder>

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppPluginFramework
{

bool PluginManagerConfig::isValid() const
{
    return validateConfig().isEmpty();
}

// -------------------------------------------------------------------------------------------------

const QList<PluginConfig> &PluginManagerConfig::pluginConfigs() const
{
    return m_pluginConfigs;
}

// -------------------------------------------------------------------------------------------------

void PluginManagerConfig::setPluginConfigs(const QList<PluginConfig> &pluginConfigs)
{
    m_pluginConfigs = pluginConfigs;
}

// -------------------------------------------------------------------------------------------------

const QStringList &PluginManagerConfig::pluginStartupPriorities() const
{
    return m_pluginStartupPriorities;
}

// -------------------------------------------------------------------------------------------------

void PluginManagerConfig::setPluginStartupPriorities(const QStringList &startupPriorities)
{
    m_pluginStartupPriorities = startupPriorities;
}

// -------------------------------------------------------------------------------------------------

bool PluginManagerConfig::loadConfigParameters(const CppConfigFramework::ConfigObjectNode &config)
{
    // Load plugin configs
    if (!loadRequiredConfigContainer(&m_pluginConfigs, QStringLiteral("plugins"), config))
    {
        qCWarning(CppPluginFramework::LoggingCategory::Config)
                << "Failed to load plugin configurations!";
        return false;
    }

    // Load plugin startup priorities
    m_pluginStartupPriorities.clear();

    if (!loadOptionalConfigParameter(&m_pluginStartupPriorities,
                                     QStringLiteral("plugin_startup_priorities"),
                                     config))
    {
        qCWarning(CppPluginFramework::LoggingCategory::Config)
                << "Failed to load plugin's startup priorities!";
        return false;
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

bool PluginManagerConfig::storeConfigParameters(CppConfigFramework::ConfigObjectNode *config)
{
    // Storing config parameters is currently not supported
    Q_UNUSED(config)
    return false;
}

// -------------------------------------------------------------------------------------------------

QString PluginManagerConfig::validateConfig() const
{
    // Check individual plugins are valid and extract all instance names
    QSet<QString> plugins;
    QSet<QString> instanceNames;
    QSet<QString> dependencies;

    for (const auto &pluginConfig : m_pluginConfigs)
    {
        // Check individual plugins are valid
        if (!pluginConfig.isValid())
        {
            return QStringLiteral("Plugin config is not valid: ") % pluginConfig.filePath();
        }

        // Check for duplicate plugins
        if (plugins.contains(pluginConfig.filePath()))
        {
            return QStringLiteral("Duplicated plugin: [%1]") % pluginConfig.filePath();
        }

        plugins.insert(pluginConfig.filePath());

        // Check for plugin instances of this plugin
        for (const auto &instanceConfig : pluginConfig.instanceConfigs())
        {
            // Check for duplicate instance name
            if (instanceNames.contains(instanceConfig.name()))
            {
                return QString("Plugin [%1] has an instance with a duplicated name [%2]!")
                        .arg(pluginConfig.filePath(),
                             instanceConfig.name());
            }

            instanceNames.insert(instanceConfig.name());

            // Keep track of all dependencies
            dependencies.unite(instanceConfig.dependencies());
        }
    }

    // Check if dependencies reference actual plugin instances
    for (const QString &dependency : dependencies)
    {
        if (!instanceNames.contains(dependency))
        {
            return QString("Dependency [%1] does not reference an actual plugin instance!")
                    .arg(dependency);
        }
    }

    // Check if the startup priorities reference actual plugin instances
    for (const QString &instanceName : m_pluginStartupPriorities)
    {
        if (!instanceNames.contains(instanceName))
        {
            return QString("Plugin instance [%1] referenced in the startup priorities does not"
                           "reference an actual plugin instance!")
                    .arg(instanceName);
        }

        if (m_pluginStartupPriorities.count(instanceName) != 1)
        {
            return QString("Duplicate plugin instance [%1] in the startup priorities!")
                    .arg(instanceName);
        }
    }

    return QString();
}

} // namespace CppPluginFramework

// -------------------------------------------------------------------------------------------------

bool operator==(const CppPluginFramework::PluginManagerConfig &left,
                const CppPluginFramework::PluginManagerConfig &right)
{
    return ((left.pluginConfigs() == right.pluginConfigs()) &&
            (left.pluginStartupPriorities() == right.pluginStartupPriorities()));
}

// -------------------------------------------------------------------------------------------------

bool operator!=(const CppPluginFramework::PluginManagerConfig &left,
                const CppPluginFramework::PluginManagerConfig &right)
{
    return !(left == right);
}
