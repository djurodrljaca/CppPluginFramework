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

// System includes
#include <memory>

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppPluginFramework
{

//! This class manages plugins
class CPPPLUGINFRAMEWORK_LIBRARY_EXPORT PluginManager
{
public:
    //! Destructor
    ~PluginManager();

    /*!
     * Loads all plugin instances specified in the config
     *
     * \param   pluginManagerConfig     Plugin manager configs
     *
     * \param[out]  error   Optional output for the error string
     *
     * \retval  true    Success
     * \retval  false   Failure
     *
     * After each plugin is loaded all of its instances get created and configured. When all the
     * plugins are loaded the dependencies of each plugin instance are injected into it.
     */
    bool load(const PluginManagerConfig &pluginManagerConfig, QString *error = nullptr);

    /*!
     *  Unloads all loaded plugin instances
     *
     * \param[out]  error   Optional output for the error string
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    bool unload(QString *error = nullptr);

    /*!
     * Starts all loaded plugin instances
     *
     * \param[out]  error   Optional output for the error string
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    bool start(QString *error = nullptr);

    //! Stops all loaded plugin instances
    void stop();

    /*!
     * Gets the specified plugin instance
     *
     * \param   instanceName    Plugin instance name
     *
     * \return  Plugin instance or nullptr if there is no plugin instance with that name
     */
    IPlugin *pluginInstance(const QString &instanceName);

    /*!
     * Gets names of all loaded plugin instances
     *
     * \return  Names of all loaded plugin instances
     */
    QStringList pluginInstanceNames() const;

private:
    /*!
     * Injects dependencies to all specified plugins
     *
     * \param   pluginConfigs   List of plugin configs
     *
     * \param[out]  error   Optional output for the error string
     *
     * \retval  true    All dependencies were injected
     * \retval  false   Injection of at least one dependency failed
     */
    bool injectAllDependencies(const QList<PluginConfig> &pluginConfigs, QString *error);

    /*!
     * Injects dependencies to the specified instance
     *
     * \param   instanceName    Name of the plugin instance to inject dependencies to
     * \param   dependencies    Names of the needed dependencies to inject
     *
     * \param[out]  error   Optional output for the error string
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    bool injectDependencies(const QString &instanceName,
                            const QSet<QString> &dependencies,
                            QString *error);

    /*!
     * Ejects all injected dependencies
     *
     * \param[out]  error   Optional output for the error string
     *
     * \retval  true    Success
     * \retval  false   Failure (at least one plugin instance was still running)
     */
    bool ejectDependencies(QString *error);

private:
    //! Holds all of the loaded plugins
    std::map<QString, std::unique_ptr<IPlugin>> m_pluginInstances;
};

} // namespace CppPluginFramework

#endif // CPPPLUGINFRAMEWORK_PLUGINMANAGER_HPP
