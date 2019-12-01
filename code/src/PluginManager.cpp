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
#include <CppPluginFramework/Plugin.hpp>
#include <CppPluginFramework/Validation.hpp>

// Qt includes
#include <QtCore/QDir>
#include <QtCore/QLibrary>
#include <QtCore/QtDebug>

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppPluginFramework
{

PluginManager::~PluginManager()
{
    unload();
}

// -------------------------------------------------------------------------------------------------

bool PluginManager::load(const PluginManagerConfig &pluginManagerConfig, QString *error)
{
    // Check if plugins are already loaded
    if (!m_pluginInstances.empty())
    {
        if (error != nullptr)
        {
            *error = QStringLiteral("Plugins are already loaded!");
        }
        return false;
    }

    // Check if config is valid
    if (!pluginManagerConfig.isValid())
    {
        if (error != nullptr)
        {
            *error = QStringLiteral("Plugin manager config is not valid!");
        }
        return false;
    }

    // Load all plugin instances
    for (const auto &pluginConfig : pluginManagerConfig.pluginConfigs())
    {
        // Load plugin instances
        auto instances = Plugin::loadInstances(pluginConfig, error);

        if (instances.empty())
        {
            if (error != nullptr)
            {
                *error = QString("Failed to load plugin [%1]. Error: [%2]")
                         .arg(pluginConfig.filePath(),
                              *error);
            }
            return false;
        }

        // Store the loaded instances
        for (auto &instance : instances)
        {
            // Make sure that an instance with the same name is not already in the container
            if (pluginInstance(instance->name()) != nullptr)
            {
                if (error != nullptr)
                {
                    *error = QString("A plugin instance with the same name [%1] was already "
                                     "loaded!").arg(instance->name());
                }
                return false;
            }

            // Store the instance in the container
            m_pluginInstances.emplace(instance->name(), std::move(instance));
        }
    }

    // Inject dependencies
    if (!injectAllDependencies(pluginManagerConfig.pluginConfigs(), error))
    {
        if (error != nullptr)
        {
            *error = QString("Failed to inject dependencies! Error: [%1]").arg(*error);
        }
        return false;
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

bool PluginManager::unload(QString *error)
{
    // Make sure that all plugins are stopped
    stop();

    // Make sure that all of their dependencies are ejected
    if (!ejectDependencies(error))
    {
        if (error != nullptr)
        {
            *error = QString("Failed to unload eject dependencies! Error: [%1]").arg(*error);
        }
        return false;
    }

    // Unload all plugin instances
    m_pluginInstances.clear();
    return true;
}

// -------------------------------------------------------------------------------------------------

bool PluginManager::start(QString *error)
{
    // TODO: make it possible to start the plugin instances in a specific order!

    // Iterate over all plugin instances and start them
    for (auto &item : m_pluginInstances)
    {
        auto *instance = item.second.get();

        // Check if plugin instance is loaded
        if (instance == nullptr)
        {
            if (error != nullptr)
            {
                *error = QString("Plugin instance [%1] is null!").arg(item.first);
            }
            return false;
        }

        // Start plugin instance
        if (instance->isStarted())
        {
            if (error != nullptr)
            {
                *error = QString("Plugin instance [%1] is already started!").arg(instance->name());
            }
            return false;
        }

        if (!instance->start())
        {
            if (error != nullptr)
            {
                *error = QString("Failed to start plugin instance [%1]!").arg(instance->name());
            }
            return false;
        }
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

void PluginManager::stop()
{
    // TODO: make it possible to stop the plugin instances in a specific order!

    // Iterate over all plugin instances and stop them
    for (auto &item : m_pluginInstances)
    {
        auto *instance = item.second.get();

        // Stop plugin instance
        if ((instance != nullptr) && (instance->isStarted()))
        {
            instance->stop();
        }
    }
}

// -------------------------------------------------------------------------------------------------

IPlugin *PluginManager::pluginInstance(const QString &instanceName)
{
    auto it = m_pluginInstances.find(instanceName);

    if (it == m_pluginInstances.end())
    {
        return nullptr;
    }

    return it->second.get();
}

// -------------------------------------------------------------------------------------------------

QStringList PluginManager::pluginInstanceNames() const
{
    QStringList instanceNames;

    for (const auto &item : m_pluginInstances)
    {
        instanceNames.append(item.first);
    }

    return instanceNames;
}

// -------------------------------------------------------------------------------------------------

bool PluginManager::injectAllDependencies(const QList<PluginConfig> &pluginConfigs, QString *error)
{
    // Iterate over all plugin configs
    for (const PluginConfig &pluginConfig : pluginConfigs)
    {
        // Iterate over all plugin instance configs
        for (const PluginInstanceConfig &instanceConfig : pluginConfig.instanceConfigs())
        {
            // Inject dependencies to the selected plugin
            const QSet<QString> dependencies = instanceConfig.dependencies();

            if (!dependencies.isEmpty())
            {
                if (!injectDependencies(instanceConfig.name(), dependencies, error))
                {
                    if (error != nullptr)
                    {
                        *error = QString("Failed to inject dependencies to plugin instance [%1]. "
                                         "Error: [%2]").arg(instanceConfig.name(),
                                                            *error);
                    }
                    return false;
                }
            }
        }
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

bool PluginManager::injectDependencies(const QString &instanceName,
                                       const QSet<QString> &dependencies,
                                       QString *error)
{
    // Find plugin instance
    auto *instance = pluginInstance(instanceName);

    if (instance == nullptr)
    {
        if (error != nullptr)
        {
            *error = QString("Plugin instance [%1] was not found!").arg(instanceName);
        }
        return false;
    }

    // Inject plugin's dependencies
    for (const QString &dependencyName : dependencies)
    {
        // Find dependency
        auto *dependency = pluginInstance(dependencyName);

        if (dependency == nullptr)
        {
            if (error != nullptr)
            {
                *error = QString("Dependency [%1] was not found!").arg(dependencyName);
            }
            return false;
        }

        if (!instance->injectDependency(dependency))
        {
            if (error != nullptr)
            {
                *error = QString("Failed to inject dependency [%1] into plugin instance [%2]!")
                         .arg(dependencyName,
                              instanceName);
            }
            return false;
        }
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

bool PluginManager::ejectDependencies(QString *error)
{
    // Iterate over all plugin instances and eject their dependencies
    for (auto &item : m_pluginInstances)
    {
        auto *instance = item.second.get();

        // Check if plugin instance is loaded
        if (instance == nullptr)
        {
            if (error != nullptr)
            {
                *error = QString("Plugin instance [%1] is null!").arg(item.first);
            }
            return false;
        }

        // Check if plugin instance is started
        if (instance->isStarted())
        {
            if (error != nullptr)
            {
                *error = QString("Plugin instance [%1] is not stopped!").arg(instance->name());
            }
            return false;
        }

        // Eject dependencies
        instance->ejectDependencies();
    }

    return true;
}

} // namespace CppPluginFramework
