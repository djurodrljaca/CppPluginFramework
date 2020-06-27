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
#include <CppPluginFramework/IPluginFactory.hpp>
#include <CppPluginFramework/PluginConfig.hpp>

// Qt includes

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppPluginFramework
{

//! This class enables loading of plugin instances from a library
class CPPPLUGINFRAMEWORK_LIBRARY_EXPORT Plugin
{
public:
    /*!
     * Loads plugin instances from the specified library
     *
     * \param   pluginConfig    Plugin config
     *
     * \return  Loaded plugin instances or an empty vector if loading failed
     */
    static std::vector<std::unique_ptr<IPlugin>> loadInstances(const PluginConfig &pluginConfig);

private:
    //! Construction of this class is disabled
    Plugin();

    /*!
     * Loads the plugin instance from the specified library and configures it
     *
     * \param   instanceConfig  Plugin instance config
     *
     * \return  Loaded plugin instance or nullptr if loading failed
     */
    static std::unique_ptr<IPlugin> loadInstance(const IPluginFactory &pluginFactory,
                                                 const PluginInstanceConfig &instanceConfig);

    /*!
     * Checks if the version matches the plugin config's version requirements
     *
     * \param   pluginVersion   Plugin version
     * \param   pluginConfig    Plugin config
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    static bool checkVersion(const VersionInfo &pluginVersion, const PluginConfig &pluginConfig);
};

} // namespace CppPluginFramework

#endif // CPPPLUGINFRAMEWORK_PLUGIN_HPP
