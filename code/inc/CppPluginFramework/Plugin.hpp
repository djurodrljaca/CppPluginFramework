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

#ifndef CPPPLUGINFRAMEWORK_PLUGIN_HPP
#define CPPPLUGINFRAMEWORK_PLUGIN_HPP

// C++ Plugin Framework includes
#include <CppPluginFramework/IPlugin.hpp>
#include <CppPluginFramework/PluginConfig.hpp>

// Qt includes

// System includes
#include <memory>

// Forward declarations

// Macros

namespace CppPluginFramework
{

/*!
 * This class enables loading of a plugin from a library
 */
class CPPPLUGINFRAMEWORK_LIBRARY_EXPORT Plugin
{
public:
    /*!
     * Destructor
     */
    ~Plugin();

    /*!
     * Checks if the plugin is loaded
     *
     * \retval  true    Plugin is loaded
     * \retval  false   Plugin is not loaded
     */
    bool isLoaded() const;

    /*!
     * Checks if the plugin is valid
     *
     * \retval  true    Plugin is valid
     * \retval  false   Plugin is not valid
     */
    bool isValid() const;

    /*!
     * Gets the plugin's version
     *
     * \return  Plugin's version
     */
    VersionInfo version() const;

    /*!
     * Gets all instances of this plugin
     *
     * \return  All instances of the plugin
     */
    QList<IPlugin *> instances();

    /*!
     * Gets the specified plugin instance
     *
     * \param   instanceName    Name of the plugin instance
     *
     * \return  Plugin instance or nullptr
     */
    IPlugin *instance(const QString &instanceName);

    /*!
     * Loads a plugin from the specified library
     *
     * \param   pluginConfig    Plugin config
     *
     * \return  Loaded plugin or nullptr if loading failed
     */
    static std::unique_ptr<Plugin> loadPlugin(const PluginConfig &pluginConfig);

    /*!
     * Unloads the plugin
     */
    void unloadPlugin();

private:
    /*!
     * Private constructor
     */
    Plugin();

    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

}

#endif // CPPPLUGINFRAMEWORK_PLUGIN_HPP
