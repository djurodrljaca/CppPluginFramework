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

// Own header
#include <CppPluginFramework/PluginManager.hpp>

// C++ Plugin Framework includes
#include "LogHelper.hpp"
#include <CppPluginFramework/Plugin.hpp>
#include <CppPluginFramework/Validation.hpp>

// Qt includes
#include <QtCore/QDir>
#include <QtCore/QLibrary>
#include <QtCore/QtDebug>

// System includes

// Forward declarations

// Macros
#define LOG_METHOD(METHOD)      CPPPLUGINFRAMEWORK_LOG_METHOD("PluginManager::" METHOD)
#define LOG_METHOD_IMPL(METHOD) CPPPLUGINFRAMEWORK_LOG_METHOD("PluginManager::Impl::" METHOD)

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
    bool injectAllDependencies(const QList<PluginConfig> &pluginConfigs);

    /*!
     * Injects dependencies to the specified instance
     *
     * \param   instanceName    Name of the plugin instance to inject dependencies to
     * \param   dependencies    Names of the needed dependencies
     *
     * \retval  true    Plugin's dependencies were injected
     * \retval  false   Injection of at least one dependency failed
     */
    bool injectPluginDependencies(const QString &instanceName, const QSet<QString> &dependencies);

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

// -------------------------------------------------------------------------------------------------

bool PluginManager::Impl::loadPlugins(const QList<PluginConfig> &pluginConfigs)
{
    if (!m_plugins.isEmpty())
    {
        qDebug() << LOG_METHOD_IMPL("loadPlugins")
                 << "Error: plugins are already loaded";
        return false;
    }

    // Load all plugins
    for (const PluginConfig &pluginConfig : pluginConfigs)
    {
        // Load plugin
        std::unique_ptr<Plugin> plugin = Plugin::load(pluginConfig);

        if (!plugin)
        {
            qDebug() << LOG_METHOD_IMPL("loadPlugins")
                     << "Error: failed to load plugin:" << pluginConfig.filePath();
            return false;
        }

        if (!plugin->isValid())
        {
            qDebug() << LOG_METHOD_IMPL("loadPlugins")
                     << "Error: load plugin is invalid:" << pluginConfig.filePath();
            return false;
        }

        // Check if a plugin instance with the same name already exists
        for (IPlugin *item : plugin->instances())
        {
            if (pluginInstance(item->name()) != nullptr)
            {
                qDebug() << LOG_METHOD_IMPL("loadPlugins")
                         << "Error: A plugin instance with the same name is already loaded:"
                         << item->name();
                return false;
            }
        }

        m_plugins.append(plugin.release());
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

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

// -------------------------------------------------------------------------------------------------

bool PluginManager::Impl::injectAllDependencies(const QList<PluginConfig> &pluginConfigs)
{
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
                if (!injectPluginDependencies(pluginInstanceConfig.name(), dependencies))
                {
                    qDebug() << LOG_METHOD_IMPL("injectAllDependencies")
                             << "Error: failed to inject dependencies into plugin:"
                             << pluginInstanceConfig.name();
                    return false;
                }
            }
        }
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

bool PluginManager::Impl::injectPluginDependencies(const QString &instanceName,
                                                   const QSet<QString> &dependencies)
{
    // Find plugin instance
    IPlugin *instance = pluginInstance(instanceName);

    if (instance == nullptr)
    {
        qDebug() << LOG_METHOD_IMPL("injectPluginDependencies")
                 << "Error: failed to find plugin instance:" << instanceName;
        return false;
    }

    // Inject plugin's dependencies
    for (const QString &dependencyName : dependencies)
    {
        // Find dependency
        IPlugin *dependency = pluginInstance(dependencyName);

        if (dependency == nullptr)
        {
            qDebug() << LOG_METHOD_IMPL("injectPluginDependencies")
                     << "Error: failed to find dependency:" << dependencyName;
            return false;
        }

        if (!instance->injectDependency(dependency))
        {
            qDebug() << LOG_METHOD_IMPL("injectPluginDependencies")
                     << "Error: failed to inject dependency " << dependencyName
                     << "into plugin" << instanceName;
            return false;
        }
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

bool PluginManager::Impl::ejectDependencies()
{
    for (Plugin *plugin : m_plugins)
    {
        if (plugin->isLoaded())
        {
            for (IPlugin *pluginInstance : plugin->instances())
            {
                if (!pluginInstance->isStarted())
                {
                    qDebug() << LOG_METHOD_IMPL("ejectDependencies")
                             << "Error: plugin instance is not stopped:" << pluginInstance->name();
                    return false;
                }

                pluginInstance->ejectDependencies();
            }
        }
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

bool PluginManager::Impl::startPlugins()
{
    // Iterate over all plugins and start their instances
    for (Plugin *plugin : m_plugins)
    {
        // Check if plugin is loaded
        if (!plugin->isLoaded())
        {
            qDebug() << LOG_METHOD_IMPL("startPlugins")
                     << "Error: plugin is not loaded:" << plugin->filePath();
            return false;
        }

        // Start all plugin's instances
        for (IPlugin *pluginInstance : plugin->instances())
        {
            // Check if plugin instance is already started
            if (pluginInstance->isStarted())
            {
                qDebug() << LOG_METHOD_IMPL("startPlugins")
                         << "Error: plugin instance is already started:" << pluginInstance->name();
                return false;
            }

            // Start all plugin instance
            if (!pluginInstance->start())
            {
                qDebug() << LOG_METHOD_IMPL("startPlugins")
                         << "Error: failed to start plugin:" << pluginInstance->name();
                return false;
            }
        }
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

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

// -------------------------------------------------------------------------------------------------

IPlugin *PluginManager::Impl::pluginInstance(const QString &pluginInstanceName)
{
    for (auto plugin : m_plugins)
    {
        IPlugin *foundPluginInterface = plugin->instance(pluginInstanceName);

        if (foundPluginInterface != nullptr)
        {
            return foundPluginInterface;
        }
    }

    return nullptr;
}

// -------------------------------------------------------------------------------------------------
// API
// -------------------------------------------------------------------------------------------------

PluginManager::PluginManager()
    : m_impl(std::make_unique<PluginManager::Impl>())
{
}

// -------------------------------------------------------------------------------------------------

PluginManager::~PluginManager()
{
    unloadPlugins();
}

// -------------------------------------------------------------------------------------------------

bool PluginManager::loadPlugins(const PluginManagerConfig &pluginManagerConfig)
{
    // Load plugins
    if (!m_impl->loadPlugins(pluginManagerConfig.pluginConfigs()))
    {
        qDebug() << LOG_METHOD("loadPlugins")
                 << "Error: failed to load plugins!";
        return false;
    }

    // Inject dependencies
    if (!m_impl->injectAllDependencies(pluginManagerConfig.pluginConfigs()))
    {
        qDebug() << LOG_METHOD("loadPlugins")
                 << "Error: failed to inject dependencies!";
        return false;
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

bool PluginManager::startPlugins()
{
    return m_impl->startPlugins();
}

// -------------------------------------------------------------------------------------------------

void PluginManager::stopPlugins()
{
    m_impl->stopPlugins();
}

// -------------------------------------------------------------------------------------------------

void PluginManager::unloadPlugins()
{
    m_impl->unloadPlugins();
}

// -------------------------------------------------------------------------------------------------

IPlugin *PluginManager::pluginInstance(const QString &pluginInstanceName)
{
    return m_impl->pluginInstance(pluginInstanceName);
}

// -------------------------------------------------------------------------------------------------

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
