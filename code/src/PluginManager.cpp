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
 * Contains a plugin management class
 */

// C++ Plugin Framework includes
#include <CppPluginFramework/Plugin.hpp>
#include <CppPluginFramework/PluginManager.hpp>
#include <CppPluginFramework/Validation.hpp>

// Qt includes
#include <QtCore/QDir>
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
 * Private implementation struct
 */
struct PluginManager::Impl
{
    /*!
     * Loads all specified plugins
     *
     * \param   pluginConfigs   List of plugin configs
     *
     * \retval  true    All plugins were loaded
     * \retval  false   Loading of at least one plugin failed
     */
    bool loadPlugins(const QList<PluginConfig> &pluginConfigs);

    /*!
     * Unloads all loaded plugins
     */
    void unloadPlugins();

    /*!
     * Injects dependencies to all specified plugins
     *
     * \param   pluginConfigs   List of plugin configs
     *
     * \retval  true    All dependencies were injected
     * \retval  false   Injection of at least one dependency failed
     */
    bool injectDependencies(const QList<PluginConfig> &pluginConfigs);

    /*!
     * Injects dependencies to the specified instance
     *
     * \param   instanceName    Name of the plugin instance to inject dependencies to
     * \param   dependencies    Names of the needed dependencies
     *
     * \retval  true    All dependencies were injected
     * \retval  false   Injection of at least one dependency failed
     */
    bool injectDependencies(const QString &instanceName, const QSet<QString> &dependencies);

    /*!
     * Ejects all injected dependencies
     *
     * \retval  true    Success
     * \retval  false   Failure (at least one plugin instance was still running)
     */
    bool ejectDependencies();

    /*!
     * Starts all loaded plugins
     *
     * \retval  true    All plugins were started
     * \retval  false   At least one plugin failed to load
     */
    bool startPlugins();

    /*!
     * Stops all loaded plugins
     */
    void stopPlugins();

    /*!
     * Gets the specified plugin instance
     *
     * \param   pluginInstanceName  Plugin instance name
     *
     * \return  Pointer to plugin instance or nullptr
     */
    IPlugin *pluginInstance(const QString &pluginInstanceName);

    /*!
     * Holds all of the loaded plugins.
     *
     * \note    Key is mapped to plugin's name
     */
    QList<Plugin *> m_plugins;
};

bool PluginManager::Impl::loadPlugins(const QList<PluginConfig> &pluginConfigs)
{
    bool success = false;

    // Load all plugins
    m_plugins.clear();

    for (const PluginConfig &pluginConfig : pluginConfigs)
    {
        // Load plugin
        std::unique_ptr<Plugin> plugin = Plugin::load(pluginConfig);
        success = plugin->isValid();

        if (success)
        {
            for (IPlugin *item : plugin->instances())
            {
                // Check if a plugin instance with the same name already exists
                if (pluginInstance(item->name()) != nullptr)
                {
                    qDebug() << "CppPluginFramework::PluginManager::Impl::loadPlugins: "
                                "Error: A plugin instance with the same name is already loaded:"
                             << item->name();
                    success = false;
                    break;
                }
            }

            if (success)
            {
                m_plugins.append(plugin.release());
            }
        }

        if (!success)
        {
            qDebug() << "CppPluginFramework::PluginManager::Impl::loadPlugin:"
                        "Error: failed to load plugin:" << pluginConfig.filePath();
            break;
        }
    }

    return success;
}

void PluginManager::Impl::unloadPlugins()
{
    // Make sure that all plugins are stopped and that all of their dependencies are ejected
    stopPlugins();
    ejectDependencies();

    // Unload all plugins
    while (!m_plugins.isEmpty())
    {
        Plugin *plugin = m_plugins.takeFirst();

        if (plugin->isLoaded())
        {
            plugin->unload();
        }

        delete plugin;
        plugin = nullptr;
    }
}

bool PluginManager::Impl::injectDependencies(const QList<PluginConfig> &pluginConfigs)
{
    bool success = true;

    // Iterate over all plugin configs
    for (const PluginConfig &pluginConfig : pluginConfigs)
    {
        // Iterate over all plugin instance configs
        for (const PluginInstanceConfig &pluginInstanceConfig : pluginConfig.instanceConfigs())
        {
            // Inject dependencies to the selected plugin
            const QSet<QString> dependencies = pluginInstanceConfig.dependencies();

            if (!dependencies.isEmpty())
            {
                success = injectDependencies(pluginInstanceConfig.name(), dependencies);

                if (!success)
                {
                    break;
                }
            }
        }

        if (!success)
        {
            break;
        }
    }

    return success;
}

bool PluginManager::Impl::injectDependencies(const QString &instanceName,
                                             const QSet<QString> &dependencies)
{
    bool success = false;
    IPlugin *instance = pluginInstance(instanceName);

    if (instance != nullptr)
    {
        for (const QString &dependencyName : dependencies)
        {
            IPlugin *dependency = pluginInstance(dependencyName);

            if (dependency != nullptr)
            {
                success = instance->injectDependency(dependency);

                if (!success)
                {
                    qDebug() << "CppPluginFramework::PluginManager::Impl::injectDependencies: "
                                "Error: failed to inject dependency:"
                             << dependencyName << "-->" << instanceName;
                }
            }
            else
            {
                qDebug() << "CppPluginFramework::PluginManager::Impl::injectDependencies: "
                            "Error: failed to find dependency:" << dependencyName;
                success = false;
            }

            if (!success)
            {
                break;
            }
        }
    }
    else
    {
        qDebug() << "CppPluginFramework::PluginManager::Impl::injectDependencies:"
                    "Error: failed to find plugin instance:" << instanceName;
    }

    return success;
}

bool PluginManager::Impl::ejectDependencies()
{
    bool success = true;

    for (Plugin *plugin : m_plugins)
    {
        if (plugin->isLoaded())
        {
            for (IPlugin *pluginInstance : plugin->instances())
            {
                if (!pluginInstance->isStarted())
                {
                    pluginInstance->ejectDependencies();
                }
                else
                {
                    success = false;
                    qDebug() << "CppPluginFramework::PluginManager::Impl::ejectDependencies:"
                                "Error: plugin instance is not stopped:" << pluginInstance->name();
                }
            }
        }
    }

    return success;
}

bool PluginManager::Impl::startPlugins()
{
    bool success = false;

    for (Plugin *plugin : m_plugins)
    {
        if (plugin->isLoaded())
        {
            for (IPlugin *pluginInstance : plugin->instances())
            {
                if (!pluginInstance->isStarted())
                {
                    success = pluginInstance->start();

                    if (!success)
                    {
                        qDebug() << "CppPluginFramework::PluginManager::Impl::startPlugins:"
                                    "Error: failed to start plugin:" << pluginInstance->name();
                        break;
                    }
                }
                else
                {
                    success = false;
                    qDebug() << "CppPluginFramework::PluginManager::Impl::startPlugins:"
                                "Error: plugin instance is not stopped:" << pluginInstance->name();
                }
            }
        }
    }

    if (!success)
    {
        stopPlugins();
    }

    return success;
}

void PluginManager::Impl::stopPlugins()
{
    // Stop all plugin instances (in the reverse order)
    for (auto itPlugin = m_plugins.rbegin(); itPlugin != m_plugins.rend(); itPlugin++)
    {
        Plugin *plugin = *itPlugin;

        if (plugin->isLoaded())
        {
            QList<IPlugin *> pluginInstances = plugin->instances();

            for (auto itPluginInstance = pluginInstances.rbegin();
                 itPluginInstance != pluginInstances.rend();
                 itPluginInstance++)
            {
                IPlugin *pluginInstance = *itPluginInstance;

                if (pluginInstance->isStarted())
                {
                    pluginInstance->stop();
                }
            }
        }
    }
}

IPlugin *PluginManager::Impl::pluginInstance(const QString &pluginInstanceName)
{
    IPlugin *foundPluginInterface = nullptr;

    for (auto plugin : m_plugins)
    {
        foundPluginInterface = plugin->instance(pluginInstanceName);

        if (foundPluginInterface != nullptr)
        {
            break;
        }
    }

    return foundPluginInterface;
}

// -------------------------------------------------------------------------------------------------
// API
// -------------------------------------------------------------------------------------------------

PluginManager::PluginManager()
    : m_impl(std::make_unique<PluginManager::Impl>())
{
}

PluginManager::~PluginManager()
{
    unloadPlugins();
}

bool PluginManager::loadPlugins(const QList<PluginConfig> &pluginConfigs)
{
    bool success = false;

    // Load plugins
    success = m_impl->loadPlugins(pluginConfigs);

    // Inject dependencies
    if (success)
    {
        success = m_impl->injectDependencies(pluginConfigs);
    }

    return success;
}

bool PluginManager::startPlugins()
{
    return m_impl->startPlugins();
}

void PluginManager::stopPlugins()
{
    m_impl->stopPlugins();
}

void PluginManager::unloadPlugins()
{
    m_impl->unloadPlugins();
}

IPlugin *PluginManager::pluginInstance(const QString &pluginInstanceName)
{
    return m_impl->pluginInstance(pluginInstanceName);
}

QStringList PluginManager::pluginInstanceNames() const
{
    QStringList instanceNames;

    for (auto plugin : m_impl->m_plugins)
    {
        for (auto instance : plugin->instances())
        {
            instanceNames.append(instance->name());
        }
    }

    return instanceNames;
}

}
