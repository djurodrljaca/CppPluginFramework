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
#include <CppPluginFramework/LoggingCategories.hpp>
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

std::vector<std::unique_ptr<IPlugin>> Plugin::loadInstances(const PluginConfig &pluginConfig)
{
    // Check plugin config
    if (!pluginConfig.isValid())
    {
        qCWarning(CppPluginFramework::LoggingCategory::Plugin) << "Plugin config is not valid!";
        return {};
    }

    // Load plugin from the library and extract the plugin factory interface from it
    QPluginLoader loader(pluginConfig.filePath());
    auto *loaderInstance = loader.instance();

    if (loaderInstance == nullptr)
    {
        qCWarning(CppPluginFramework::LoggingCategory::Plugin)
                << "Failed to load plugin:" << loader.fileName();
        return {};
    }

    auto *pluginFactory = qobject_cast<IPluginFactory*>(loaderInstance);

    if (pluginFactory == nullptr)
    {
        qCWarning(CppPluginFramework::LoggingCategory::Plugin)
                << QString("Loaded plugin [%1] does not implement the plugin factory interface!")
                   .arg(loader.fileName());
        return {};
    }

    // Create plugin instances
    std::vector<std::unique_ptr<IPlugin>> instances;

    for (const PluginInstanceConfig &instanceConfig : pluginConfig.instanceConfigs())
    {
        // Create plugin instance
        auto instance = loadInstance(*pluginFactory, instanceConfig);

        if (!instance)
        {
            qCWarning(CppPluginFramework::LoggingCategory::Plugin)
                    << QString("Failed to load the plugin instance [%1] from the plugin [%2]!")
                       .arg(instanceConfig.name(), loader.fileName());
            return {};
        }

        // Check plugin's version
        if (!checkVersion(instance->version(), pluginConfig))
        {
            qCWarning(CppPluginFramework::LoggingCategory::Plugin)
                    << QString("Plugin instance [%1] from the plugin [%2] has an unsupported "
                               "version!").arg(instanceConfig.name(), loader.fileName());
            return {};
        }

        // Add the instance to the container of loaded instances
        instances.push_back(std::move(instance));
    }

    return instances;
}

// -------------------------------------------------------------------------------------------------

std::unique_ptr<IPlugin> Plugin::loadInstance(const IPluginFactory &pluginFactory,
                                              const PluginInstanceConfig &instanceConfig)
{
    // Create plugin instance
    auto instance = pluginFactory.createInstance(instanceConfig.name());

    if (!instance)
    {
        qCWarning(CppPluginFramework::LoggingCategory::Plugin)
                << "Failed to create the plugin instance!";
        return {};
    }

    // Configure the plugin instance
    if (!instance->loadConfig(instanceConfig.config()))
    {
        qCWarning(CppPluginFramework::LoggingCategory::Plugin)
                << "Failed to load the plugin instance's configuration!";
        return {};
    }

    return instance;
}

// -------------------------------------------------------------------------------------------------

bool Plugin::checkVersion(const VersionInfo &pluginVersion, const PluginConfig &pluginConfig)
{
    if (pluginConfig.isExactVersion())
    {
        // Plugin's version must match the exact version
        if (pluginVersion != pluginConfig.version())
        {
            qCWarning(CppPluginFramework::LoggingCategory::Plugin)
                    << QString("Loaded plugin's version [%1] does not match the expected version "
                               "[%2]!").arg(pluginVersion.toString(),
                                            pluginConfig.version().toString());
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
            qCWarning(CppPluginFramework::LoggingCategory::Plugin)
                    << QString("Loaded plugin's version [%1] does not match the expected version "
                               "range: min=[%2], max=[%3]!")
                       .arg(pluginVersion.toString(),
                            pluginConfig.minVersion().toString(),
                            pluginConfig.maxVersion().toString());
            return false;
        }
    }

    return true;
}

} // namespace CppPluginFramework
