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

bool PluginManagerConfig::loadConfigParameters(const CppConfigFramework::ConfigObjectNode &config,
                                               QString *error)
{

    // Load instance configs
    if (!loadRequiredConfigContainer(&m_pluginConfigs, QStringLiteral("plugins"), config, error))
    {
        if (error != nullptr)
        {
            *error = QStringLiteral("Failed to load plugin's instances. Error: ") % *error;
        }
        return false;
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

QString PluginManagerConfig::validateConfig() const
{
    // Check individual plugins are valid and extract all instance names
    QStringList plugins;
    QStringList instanceNames;

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

        plugins.append(pluginConfig.filePath());

        // Check for duplicate instance names
        for (const auto &instanceConfig : pluginConfig.instanceConfigs())
        {
            if (instanceNames.contains(instanceConfig.name()))
            {
                return QString("Plugin [%1] has an instance with a duplicated name [%2]!")
                        .arg(pluginConfig.filePath(),
                             instanceConfig.name());
            }

            instanceNames.append(instanceConfig.name());
        }
    }

    return QString();
}

} // namespace CppPluginFramework

// -------------------------------------------------------------------------------------------------

bool operator==(const CppPluginFramework::PluginManagerConfig &left,
                const CppPluginFramework::PluginManagerConfig &right)
{
    return (left.pluginConfigs() == right.pluginConfigs());
}

// -------------------------------------------------------------------------------------------------

bool operator!=(const CppPluginFramework::PluginManagerConfig &left,
                const CppPluginFramework::PluginManagerConfig &right)
{
    return !(left == right);
}
