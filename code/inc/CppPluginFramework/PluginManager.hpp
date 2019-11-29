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

#ifndef CPPPLUGINFRAMEWORK_PLUGINMANAGER_HPP
#define CPPPLUGINFRAMEWORK_PLUGINMANAGER_HPP

// C++ Plugin Framework includes
#include <CppPluginFramework/IPlugin.hpp>
#include <CppPluginFramework/PluginManagerConfig.hpp>

// Qt includes
#include <QtCore/QHash>

// System includes
#include <memory>

// Forward declarations

// Macros

namespace CppPluginFramework
{

/*!
 * This class can be used to manage plugins
 */
class CPPPLUGINFRAMEWORK_LIBRARY_EXPORT PluginManager
{
public:
    /*!
     * Constructor
     */
    PluginManager();

    /*!
     * Destructor
     */
    ~PluginManager();

    /*!
     * Loads all plugins specified in the config
     *
     * \param   pluginManagerConfig     Plugin manager configs
     *
     * \retval  true    Success
     * \retval  false   Failure
     *
     * After each plugin is loaded all of its instances get created and configured. When all the
     * plugins are loaded the dependencies of each plugin instance are injected into it.
     */
    bool loadPlugins(const PluginManagerConfig &pluginManagerConfig);

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
     * Unloads all loaded plugins
     */
    void unloadPlugins();

    /*!
     * Gets the specified plugin instance
     *
     * \param   pluginInstanceName  Plugin instance name
     *
     * \return  Pointer to plugin instance or nullptr
     */
    IPlugin *pluginInstance(const QString &pluginInstanceName);

    /*!
     * Gets names of all loaded plugin instances
     *
     * \return  Names of all loaded plugin instances
     */
    QStringList pluginInstanceNames() const;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

}

#endif // CPPPLUGINFRAMEWORK_PLUGINMANAGER_HPP
