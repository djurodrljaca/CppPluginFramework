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

// C++ Plugin Framework includes
#include <CppPluginFramework/Plugin.hpp>
#include <CppPluginFramework/Validation.hpp>

// Qt includes
#include <QtCore/QLibrary>
#include <QtCore/QtDebug>

// System includes

// Forward declarations

// Macros

namespace CppPluginFramework
{

// -------------------------------------------------------------------------------------------------
// Implementation
// -------------------------------------------------------------------------------------------------

/*!
 * Pointer to a function for reading the plugin version
 */
typedef const char *(*ReadPluginVersionFunction)();

/*!
 * Pointer to a function for creation of a plugin instance
 */
typedef IPlugin *(*CreatePluginInstanceFunction)(const QString &instanceName);

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
     * \param   pluginConfig    Plugin config
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    bool createPluginInstances(const PluginConfig &pluginConfig);

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

bool Plugin::Impl::readPluginVersion()
{
    bool success = false;
    auto readPluginVersionFunction =
            reinterpret_cast<ReadPluginVersionFunction>(m_library.resolve("readPluginVersion"));

    if (readPluginVersionFunction != nullptr)
    {
        const QString versionString = QString::fromUtf8(readPluginVersionFunction());
        const VersionInfo version(versionString);

        if (version.isValid())
        {
            m_version = version;
            success = true;
        }
        else
        {
            qDebug() << "CppPluginFramework::Plugin::Impl::readPluginVersion: "
                        "Error: Invalid plugin version:" << versionString;
        }
    }
    else
    {
        qDebug() << "CppPluginFramework::Plugin::Impl::createPluginInstances: "
                    "Error: Failed to load the function for reading the plugin version";
    }

    return success;
}

bool Plugin::Impl::checkPluginVersion(const PluginConfig &pluginConfig) const
{
    bool success = false;

    if (pluginConfig.isExactVersion())
    {
        success = (m_version == pluginConfig.version());
    }
    else
    {
        success = VersionInfo::isVersionInRange(m_version,
                                                pluginConfig.minVersion(),
                                                pluginConfig.maxVersion());
    }

    return success;
}

bool Plugin::Impl::createPluginInstances(const PluginConfig &pluginConfig)
{
    bool success = false;
    auto createPluginInstanceFunction = reinterpret_cast<CreatePluginInstanceFunction>(
                                            m_library.resolve("createPluginInstance"));

    // Create plugin instances
    if (createPluginInstanceFunction != nullptr)
    {
        success = true;
        QList<IPlugin *> pluginInstances;

        for (const PluginInstanceConfig &pluginInstanceConfig : pluginConfig.instanceConfigs())
        {
            IPlugin *pluginInstance = createPluginInstanceFunction(pluginInstanceConfig.name());

            if (pluginInstance != nullptr)
            {
                qDebug() << "CppPluginFramework::Plugin::Impl::createPluginInstances: "
                            "Created plugin instance:"
                         << endl << "- Name:" << pluginInstance->name()
                         << endl << "- Description:" << pluginInstance->description()
                         << endl << "- Version:" << pluginInstance->version().toString()
                         << endl << "- Exported interfaces:"
                         << pluginInstance->exportedInterfaces();

                // Configure the plugin instance
                if (pluginInstance->loadConfig(pluginInstanceConfig.config()))
                {
                    pluginInstances.append(pluginInstance);
                }
                else
                {
                    qDebug() << "CppPluginFramework::Plugin::Impl::createPluginInstances: "
                                "Error: Failed to load the plugin instance config:"
                             << pluginInstanceConfig.config();
                    success = false;
                    break;
                }
            }
            else
            {
                qDebug() << "CppPluginFramework::Plugin::Impl::createPluginInstances: "
                            "Error: Failed to create plugin instance:"
                         << pluginInstanceConfig.name();
                success = false;
                break;
            }
        }

        if (success)
        {
            if (!pluginInstances.isEmpty())
            {
                m_pluginInstances = pluginInstances;
            }
            else
            {
                success = false;
                qDebug() << "CppPluginFramework::Plugin::Impl::createPluginInstances: "
                            "Error: No plugin instances were created!";
            }
        }
    }
    else
    {
        qDebug() << "CppPluginFramework::Plugin::Impl::createPluginInstances: "
                    "Error: Failed to load plugin instance creation function:"
                 << endl << "- File path:" << pluginConfig.filePath()
                 << endl << "- Error string:" << m_library.errorString();
    }

    return success;
}

// -------------------------------------------------------------------------------------------------
// API
// -------------------------------------------------------------------------------------------------

Plugin::Plugin()
    : m_impl(std::make_unique<Plugin::Impl>())
{
}

Plugin::~Plugin()
{
    unload();
}

bool Plugin::isLoaded() const
{
    return (!m_impl->m_pluginInstances.isEmpty());
}

bool Plugin::isValid() const
{
    bool valid = false;

    // Plugin is valid if all of its instances are loaded and if the instance's name, version, and
    // list of exported interfaces are all valid
    if (isLoaded())
    {
        for (IPlugin *item : m_impl->m_pluginInstances)
        {
            valid = Validation::validatePluginInstanceName(item->name()) &&
                    item->version().isValid() &&
                    Validation::validateExportedInterfaces(item->exportedInterfaces());

            if (!valid)
            {
                break;
            }
        }
    }

    return valid;
}

VersionInfo Plugin::version() const
{
    return m_impl->m_version;
}

QList<IPlugin *> Plugin::instances()
{
    return m_impl->m_pluginInstances;
}

IPlugin *Plugin::instance(const QString &instanceName)
{
    IPlugin *pluginInstance = nullptr;

    for (IPlugin *item : m_impl->m_pluginInstances)
    {
        if (item->name() == instanceName)
        {
            pluginInstance = item;
            break;
        }
    }

    return pluginInstance;
}

std::unique_ptr<Plugin> Plugin::load(const PluginConfig &pluginConfig)
{
    bool success = false;
    std::unique_ptr<Plugin> plugin;

    if (pluginConfig.isValid())
    {
        qDebug() << "CppPluginFramework::Plugin::loadPlugin: path:" << pluginConfig.filePath();

        // Load library file and then load plugin instance creation function from it
        plugin = std::unique_ptr<Plugin>(new Plugin());
        plugin->m_impl->m_library.setFileName(pluginConfig.filePath());

        // Read plugin version
        if (plugin->m_impl->readPluginVersion())
        {
            success = true;
            qDebug() << "CppPluginFramework::Plugin::loadPlugin: version:"
                     << plugin->m_impl->m_version.toString();
        }
        else
        {
            qDebug() << "CppPluginFramework::Plugin::loadPlugin: "
                        "Error: Failed to read the plugin's version";
        }

        // Check if plugin's version matches the version requirements
        if (success)
        {
            if (!plugin->m_impl->checkPluginVersion(pluginConfig))
            {
                success = false;
                qDebug() << "CppPluginFramework::Plugin::loadPlugin: "
                            "Error: plugin's version does not match the version requirements";
            }
        }

        // Create plugin instances
        if (success)
        {
            if (!plugin->m_impl->createPluginInstances(pluginConfig))
            {
                success = false;
                qDebug() << "CppPluginFramework::Plugin::loadPlugin: "
                            "Error: failed to create all needed plugin instances";
            }
        }
    }

    // In case of failure destroy the plugin
    if (!success)
    {
        plugin.reset();
    }

    return plugin;
}

void Plugin::unload()
{
    if (isLoaded())
    {
        // Unload plugin instance
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
