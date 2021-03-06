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
#include <CppPluginFramework/LoggingCategories.hpp>
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

bool PluginManager::load(const PluginManagerConfig &pluginManagerConfig)
{
    // Check if plugins are already loaded
    if (!m_pluginInstances.empty())
    {
        qCWarning(CppPluginFramework::LoggingCategory::PluginManager)
                << "Plugins are already loaded!";
        return false;
    }

    // Check if config is valid
    if (!pluginManagerConfig.isValid())
    {
        qCWarning(CppPluginFramework::LoggingCategory::PluginManager)
                << "Plugin manager config is not valid!";
        return false;
    }

    // Load all plugin instances
    for (const auto &pluginConfig : pluginManagerConfig.pluginConfigs())
    {
        // Load plugin instances
        auto instances = Plugin::loadInstances(pluginConfig);

        if (instances.empty())
        {
            qCWarning(CppPluginFramework::LoggingCategory::PluginManager)
                    << "Failed to load plugin:" << pluginConfig.filePath();
            return false;
        }

        // Store the loaded instances
        for (auto &instance : instances)
        {
            // Make sure that an instance with the same name is not already in the container
            if (pluginInstance(instance->name()) != nullptr)
            {
                qCWarning(CppPluginFramework::LoggingCategory::PluginManager)
                        << QString("A plugin instance with the same name [%1] was already loaded!")
                           .arg(instance->name());
                return false;
            }

            // Store the instance in the container
            m_pluginInstances.emplace(instance->name(), std::move(instance));
        }
    }

    // Inject dependencies
    if (!injectAllDependencies(pluginManagerConfig.pluginConfigs()))
    {
        qCWarning(CppPluginFramework::LoggingCategory::PluginManager)
                << "Failed to inject dependencies!";
        return false;
    }

    // Startup order (instances from startup priorities first, then all others)
    m_pluginStartupOrder = pluginManagerConfig.pluginStartupPriorities();

    for (const QString &instanceName : pluginInstanceNames())
    {
        if (!m_pluginStartupOrder.contains(instanceName))
        {
            m_pluginStartupOrder.append(instanceName);
        }
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

bool PluginManager::unload()
{
    // Make sure that all plugins are stopped
    stop();

    // Make sure that all of their dependencies are ejected
    if (!ejectDependencies())
    {
        qCWarning(CppPluginFramework::LoggingCategory::PluginManager)
                << "Failed to unload eject dependencies!";
        return false;
    }

    // Unload all plugin instances
    m_pluginInstances.clear();
    return true;
}

// -------------------------------------------------------------------------------------------------

bool PluginManager::start()
{
    // Start plugin instances in the defined startup order
    for (const QString &instanceName : qAsConst(m_pluginStartupOrder))
    {
        auto *instance = pluginInstance(instanceName);

        // Check if plugin instance is loaded
        if (instance == nullptr)
        {
            qCWarning(CppPluginFramework::LoggingCategory::PluginManager)
                    << "Plugin instance is null:" << instanceName;
            return false;
        }

        // Start plugin instance
        if (instance->isStarted())
        {
            qCWarning(CppPluginFramework::LoggingCategory::PluginManager)
                    << "Plugin instance is already started:" << instanceName;
            return false;
        }

        if (!instance->start())
        {
            qCWarning(CppPluginFramework::LoggingCategory::PluginManager)
                    << "Failed to start plugin instance:" << instanceName;
            return false;
        }
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

void PluginManager::stop()
{
    // Stop plugin instances in the reverse order as they were started
    for (auto it = m_pluginStartupOrder.rbegin(); it != m_pluginStartupOrder.rend(); it++)
    {
        auto *instance = pluginInstance(*it);

        // Stop plugin instance
        if ((instance != nullptr) && (instance->isStarted()))
        {
            instance->stop();
        }
    }
}

// -------------------------------------------------------------------------------------------------

bool PluginManager::hasPluginInstance(const QString &instanceName) const
{
    auto it = m_pluginInstances.find(instanceName);

    return (it != m_pluginInstances.end());
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

bool PluginManager::injectAllDependencies(const QList<PluginConfig> &pluginConfigs)
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
                if (!injectDependencies(instanceConfig.name(), dependencies))
                {
                    qCWarning(CppPluginFramework::LoggingCategory::PluginManager)
                            << "Failed to inject dependencies to plugin instance:"
                            << instanceConfig.name();
                    return false;
                }
            }
        }
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

bool PluginManager::injectDependencies(const QString &instanceName,
                                       const QSet<QString> &dependencies)
{
    // Find plugin instance
    auto *instance = pluginInstance(instanceName);

    if (instance == nullptr)
    {
        qCWarning(CppPluginFramework::LoggingCategory::PluginManager)
                << "Plugin instance was not found:" << instanceName;
        return false;
    }

    // Inject plugin's dependencies
    for (const QString &dependencyName : dependencies)
    {
        // Find dependency
        auto *dependency = pluginInstance(dependencyName);

        if (dependency == nullptr)
        {
            qCWarning(CppPluginFramework::LoggingCategory::PluginManager)
                    << "Dependency was not found:" << dependencyName;
            return false;
        }

        if (!instance->injectDependency(dependency))
        {
            qCWarning(CppPluginFramework::LoggingCategory::PluginManager)
                    << QString("Failed to inject dependency [%1] into plugin instance [%2]!")
                       .arg(dependencyName, instanceName);
            return false;
        }
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

bool PluginManager::ejectDependencies()
{
    // Iterate over all plugin instances and eject their dependencies
    for (auto &item : m_pluginInstances)
    {
        auto *instance = item.second.get();

        // Check if plugin instance is loaded
        if (instance == nullptr)
        {
            qCWarning(CppPluginFramework::LoggingCategory::PluginManager)
                    << "Plugin instance is null:" << item.first;
            return false;
        }

        // Check if plugin instance is started
        if (instance->isStarted())
        {
            qCWarning(CppPluginFramework::LoggingCategory::PluginManager)
                    << "Plugin instance is not stopped:" << instance->name();
            return false;
        }

        // Eject dependencies
        instance->ejectDependencies();
    }

    return true;
}

} // namespace CppPluginFramework
