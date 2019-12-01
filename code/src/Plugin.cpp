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
#include <CppPluginFramework/IPluginFactory.hpp>
#include <CppPluginFramework/Validation.hpp>

// Qt includes
#include <QtCore/QPluginLoader>
#include <QtCore/QtDebug>

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppPluginFramework
{

std::vector<std::unique_ptr<IPlugin>> Plugin::loadInstances(const PluginConfig &pluginConfig,
                                                            QString *error)
{
    // Check plugin config
    if (!pluginConfig.isValid())
    {
        if (error != nullptr)
        {
            *error = QStringLiteral("Error: plugin config is not valid!");
        }
        return {};
    }

    // Load plugin from the library and extract the plugin factory interface from it
    QPluginLoader loader(pluginConfig.filePath());
    auto *loaderInstance = loader.instance();

    if (loaderInstance == nullptr)
    {
        if (error != nullptr)
        {
            *error = QString("Failed to load plugin [%1]! Error: [%2]").arg(loader.fileName(),
                                                                            loader.errorString());
        }
        return {};
    }

    auto *pluginFactory = qobject_cast<IPluginFactory*>(loaderInstance);

    if (pluginFactory == nullptr)
    {
        if (error != nullptr)
        {
            *error = QString("Loaded plugin [%1] does not implement the plugin factory interface!")
                     .arg(loader.fileName());
        }
        return {};
    }

    // Create plugin instances
    std::vector<std::unique_ptr<IPlugin>> instances;

    for (const PluginInstanceConfig &instanceConfig : pluginConfig.instanceConfigs())
    {
        // Create plugin instance
        auto instance = loadInstance(*pluginFactory, instanceConfig, error);

        if (!instance)
        {
            if (error != nullptr)
            {
                *error = QString("Failed to load the plugin instance [%1] from the plugin [%2]! "
                                 "Error: [%3]").arg(instanceConfig.name(),
                                                    loader.fileName(),
                                                    *error);
            }
            return {};
        }

        // Check plugin's version
        if (!checkVersion(instance->version(), pluginConfig, error))
        {
            if (error != nullptr)
            {
                *error = QString("Plugin instance [%1] from the plugin [%2] has an unsupported "
                                 "version! Error: [%3]").arg(instanceConfig.name(),
                                                             loader.fileName(),
                                                             *error);
            }
            return {};
        }

        // Add the instance to the container of loaded instances
        instances.push_back(std::move(instance));
    }

    return instances;
}

// -------------------------------------------------------------------------------------------------

std::unique_ptr<IPlugin> Plugin::loadInstance(const IPluginFactory &pluginFactory,
                                              const PluginInstanceConfig &instanceConfig,
                                              QString *error)
{
    // Create plugin instance
    auto instance = pluginFactory.createInstance(instanceConfig.name());

    if (!instance)
    {
        if (error != nullptr)
        {
            *error = QStringLiteral("Failed to create the plugin instance!");
        }
        return {};
    }

    // Configure the plugin instance
    if (!instance->loadConfig(instanceConfig.config()))
    {
        if (error != nullptr)
        {
            *error = QStringLiteral("Failed to load the plugin instance's configuration!");
        }
        return {};
    }

    return instance;
}

// -------------------------------------------------------------------------------------------------

bool Plugin::checkVersion(const VersionInfo &pluginVersion,
                          const PluginConfig &pluginConfig,
                          QString *error)
{
    if (pluginConfig.isExactVersion())
    {
        // Plugin's version must match the exact version
        if (pluginVersion != pluginConfig.version())
        {
            if (error != nullptr)
            {
                *error = QString("Loaded plugin's version [%1] does not match the expected version "
                                 "[%2]!").arg(pluginVersion.toString(),
                                              pluginConfig.version().toString());
            }
            return false;
        }
    }
    else
    {
        // Plugin's version must be within the version range
        if (!VersionInfo::isVersionInRange(pluginVersion,
                                           pluginConfig.minVersion(),
                                           pluginConfig.maxVersion()))
        {
            if (error != nullptr)
            {
                *error = QString("Loaded plugin's version [%1] does not match the expected version "
                                 "range: min=[%2], max=[%3]!")
                         .arg(pluginVersion.toString(),
                              pluginConfig.minVersion().toString(),
                              pluginConfig.maxVersion().toString());
            }
            return false;
        }
    }

    return true;
}

} // namespace CppPluginFramework
