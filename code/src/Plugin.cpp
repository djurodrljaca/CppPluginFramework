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
 * Contains a class that is able to load a plugin from a library
 */

// Own header
#include <CppPluginFramework/Plugin.hpp>

// C++ Plugin Framework includes
#include "LogHelper.hpp"
#include <CppPluginFramework/Validation.hpp>

// Qt includes
#include <QtCore/QLibrary>
#include <QtCore/QtDebug>

// System includes

// Forward declarations

// Macros
#define LOG_METHOD(METHOD)      CPPPLUGINFRAMEWORK_LOG_METHOD("Plugin::" METHOD)
#define LOG_METHOD_IMPL(METHOD) CPPPLUGINFRAMEWORK_LOG_METHOD("Plugin::Impl::" METHOD)

namespace CppPluginFramework
{

// -------------------------------------------------------------------------------------------------
// Implementation
// -------------------------------------------------------------------------------------------------

/*!
 * Pointer to a function for reading the plugin version
 */
typedef const char *(*ReadPluginVersionFunction)();

// -------------------------------------------------------------------------------------------------

/*!
 * Pointer to a function for creation of a plugin instance
 */
typedef IPlugin *(*CreatePluginInstanceFunction)(const QString &instanceName);

// -------------------------------------------------------------------------------------------------

/*!
 * Private implementation struct
 */
struct Plugin::Impl
{
    /*!
     * Reads the plugin version
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    bool readPluginVersion();

    /*!
     * Checks if plugin's version matches the version requirements
     *
     * \param   pluginConfig    Plugin config
     *
     * \retval  true    Plugin matches the version requirements
     * \retval  false   Plugin does not match the version requirements
     */
    bool checkPluginVersion(const PluginConfig &pluginConfig) const;

    /*!
     * Creates all needed plugin instances
     *
     * \param   pluginConfig            Plugin config
     * \param   environmentVariables    Environment variables
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    bool createPluginInstances(const PluginConfig &pluginConfig,
                               const EnvironmentVariables &environmentVariables);

    /*!
     * Holds the library object
     */
    QLibrary m_library;

    /*!
     * Holds the plugin version
     */
    VersionInfo m_version;

    /*!
     * Holds the plugin instances
     */
    QList<IPlugin *> m_pluginInstances;
};

// -------------------------------------------------------------------------------------------------

bool Plugin::Impl::readPluginVersion()
{
    // Load the function for reading the plugin version
    auto readPluginVersionFunction =
            reinterpret_cast<ReadPluginVersionFunction>(m_library.resolve("readPluginVersion"));

    if (readPluginVersionFunction == nullptr)
    {
        qDebug() << LOG_METHOD_IMPL("readPluginVersion")
                 << "Error: Failed to load the function for reading the plugin version";
        return false;
    }

    // Read version from plugin
    const QString versionString = QString::fromUtf8(readPluginVersionFunction());
    const VersionInfo version(versionString);

    if (!version.isValid())
    {
        qDebug() << LOG_METHOD_IMPL("readPluginVersion")
                 << "Error: Invalid plugin version:" << versionString;
        return false;
    }

    m_version = version;
    return true;
}

// -------------------------------------------------------------------------------------------------

bool Plugin::Impl::checkPluginVersion(const PluginConfig &pluginConfig) const
{
    if (pluginConfig.isExactVersion())
    {
        return (m_version == pluginConfig.version());
    }
    else
    {
        return VersionInfo::isVersionInRange(m_version,
                                             pluginConfig.minVersion(),
                                             pluginConfig.maxVersion());
    }
}

// -------------------------------------------------------------------------------------------------

bool Plugin::Impl::createPluginInstances(const PluginConfig &pluginConfig,
                                         const EnvironmentVariables &environmentVariables)
{
    // Check if instances are already created
    if (!m_pluginInstances.isEmpty())
    {
        qDebug() << LOG_METHOD_IMPL("createPluginInstances")
                 << "Error: plugin instance are already created";
        return false;
    }

    // Load the function for creating the plugin instances
    auto createPluginInstanceFunction = reinterpret_cast<CreatePluginInstanceFunction>(
                                            m_library.resolve("createPluginInstance"));

    if (createPluginInstanceFunction == nullptr)
    {
        qDebug() << LOG_METHOD_IMPL("createPluginInstances")
                 << "Error: Failed to load plugin instance creation function:" << endl
                 << "- File path:" << pluginConfig.filePath() << endl
                 << "- Error string:" << m_library.errorString();
        return false;
    }

    // Create plugin instances
    for (const PluginInstanceConfig &pluginInstanceConfig : pluginConfig.instanceConfigs())
    {
        // Create plugin instance
        IPlugin *pluginInstance = createPluginInstanceFunction(pluginInstanceConfig.name());

        if (pluginInstance == nullptr)
        {
            qDebug() << LOG_METHOD_IMPL("createPluginInstances")
                     << "Error: Failed to create plugin instance:" << pluginInstanceConfig.name();
            return false;
        }

        qDebug() << LOG_METHOD_IMPL("createPluginInstances")
                 << "Created plugin instance:"
                 << "- Name:" << pluginInstance->name() << endl
                 << "- Description:" << pluginInstance->description() << endl
                 << "- Version:" << pluginInstance->version().toString() << endl
                 << "- Exported interfaces:" << pluginInstance->exportedInterfaces();

        // Configure the plugin instance
        if (!pluginInstance->loadConfig(pluginInstanceConfig.config(), environmentVariables))
        {
            qDebug() << LOG_METHOD_IMPL("createPluginInstances")
                     << "Error: Failed to load the plugin instance config:"
                     << pluginInstanceConfig.config();
            return false;
        }

        m_pluginInstances.append(pluginInstance);
    }

    if (m_pluginInstances.isEmpty())
    {
        qDebug() << LOG_METHOD_IMPL("createPluginInstances")
                 << "Error: No plugin instances were created!";
        return false;
    }

    return true;
}

// -------------------------------------------------------------------------------------------------
// API
// -------------------------------------------------------------------------------------------------

Plugin::Plugin()
    : m_impl(std::make_unique<Plugin::Impl>())
{
}

// -------------------------------------------------------------------------------------------------

Plugin::~Plugin()
{
    unload();
}

// -------------------------------------------------------------------------------------------------

bool Plugin::isLoaded() const
{
    return (!m_impl->m_pluginInstances.isEmpty());
}

// -------------------------------------------------------------------------------------------------

bool Plugin::isValid() const
{
    // Plugin is valid if all of its instances are loaded and if the instance's name, version, and
    // list of exported interfaces are all valid
    if (isLoaded())
    {
        for (IPlugin *item : m_impl->m_pluginInstances)
        {
            if ((!Validation::validatePluginInstanceName(item->name())) ||
                (!item->version().isValid()) ||
                (!Validation::validateExportedInterfaces(item->exportedInterfaces())))
            {
                return false;
            }
        }
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

VersionInfo Plugin::version() const
{
    return m_impl->m_version;
}

// -------------------------------------------------------------------------------------------------

QString Plugin::filePath() const
{
    return m_impl->m_library.fileName();
}

// -------------------------------------------------------------------------------------------------

QList<IPlugin *> Plugin::instances()
{
    return m_impl->m_pluginInstances;
}

// -------------------------------------------------------------------------------------------------

IPlugin *Plugin::instance(const QString &instanceName)
{
    for (IPlugin *item : m_impl->m_pluginInstances)
    {
        if (item->name() == instanceName)
        {
            return item;
        }
    }

    return nullptr;
}

// -------------------------------------------------------------------------------------------------

std::unique_ptr<Plugin> Plugin::load(const PluginConfig &pluginConfig,
                                     const EnvironmentVariables &environmentVariables)
{
    // Check plugin config
    if (!pluginConfig.isValid())
    {
        qDebug() << LOG_METHOD("load")
                 << "Error: plugin config is not valid!";
        return nullptr;
    }

    // Load library file and then load plugin instance creation function from it
    qDebug() << LOG_METHOD("load")
             << "plugin file path:" << pluginConfig.filePath();

    std::unique_ptr<Plugin> plugin = std::unique_ptr<Plugin>(new Plugin());
    plugin->m_impl->m_library.setFileName(pluginConfig.filePath());

    // Read plugin version
    if (!plugin->m_impl->readPluginVersion())
    {
        qDebug() << "CppPluginFramework::Plugin::loadPlugin: "
                    "Error: Failed to read the plugin's version";
    }

    qDebug() << LOG_METHOD("load")
             << "plugin version:" << plugin->m_impl->m_version.toString();

    // Check if plugin's version matches the version requirements
    if (!plugin->m_impl->checkPluginVersion(pluginConfig))
    {
        qDebug() << LOG_METHOD("load")
                 << "Error: plugin's version does not match the version requirements";
        return nullptr;
    }

    // Create plugin instances
    if (!plugin->m_impl->createPluginInstances(pluginConfig, environmentVariables))
    {
        qDebug() << LOG_METHOD("load")
                 << "Error: failed to create all needed plugin instances";
        return nullptr;
    }

    return plugin;
}

// -------------------------------------------------------------------------------------------------

void Plugin::unload()
{
    if (isLoaded())
    {
        // Destroy plugin instance
        for (IPlugin *pluginInstance : m_impl->m_pluginInstances)
        {
            if (pluginInstance->isStarted())
            {
                pluginInstance->stop();
            }

            pluginInstance->ejectDependencies();
        }

        qDeleteAll(m_impl->m_pluginInstances);
        m_impl->m_pluginInstances.clear();

        // Unload plugin library
        m_impl->m_library.unload();
    }
}

}
