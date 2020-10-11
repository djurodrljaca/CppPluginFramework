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
 * Contains a base class for all plugins
 */

#pragma once

// C++ Plugin Framework includes
#include <CppPluginFramework/VersionInfo.hpp>

// C++ Config Framework includes
#include <CppConfigFramework/ConfigObjectNode.hpp>

// Qt includes

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppPluginFramework
{

//! This is a base class for all plugins
class CPPPLUGINFRAMEWORK_EXPORT IPlugin
{
public:
    //! Destructor
    virtual ~IPlugin() = default;

    /*!
     * Returns plugin's name
     *
     * \return  Plugin's name
     */
    virtual QString name() const = 0;

    /*!
     * Returns plugin's version
     *
     * \return  Plugin's version
     */
    virtual VersionInfo version() const = 0;

    /*!
     * Returns plugin's description
     *
     * \return  Plugin's description
     */
    virtual QString description() const = 0;

    /*!
     * Checks if the specified interface is exported by this plugin
     *
     * \param   interface   Name of the interface to check
     *
     * \retval  true    The specified interface is exported
     * \retval  false   The specified interface is not exported
     */
    virtual bool isInterfaceExported(const QString &interface) const = 0;

    /*!
     * Returns a list of all exported interfaces
     *
     * \return  List of all exported interfaces
     */
    virtual QSet<QString> exportedInterfaces() const = 0;

    /*!
     * Loads the specified config
     *
     * \param   config  Config node
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    virtual bool loadConfig(const CppConfigFramework::ConfigObjectNode &config) = 0;

    /*!
     * Injects all dependencies (interfaces) that can be taken out of the specified plugin instance
     *
     * \param   plugin  Plugin
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    virtual bool injectDependency(IPlugin *plugin) = 0;

    //! Ejects all injected dependencies (interfaces)
    virtual void ejectDependencies() = 0;

    /*!
     * Checks if plugin is started
     *
     * \retval  true    Plugin is started
     * \retval  false   Plugin is not started
     */
    virtual bool isStarted() const = 0;

    /*!
     * Starts the plugin
     *
     * \retval  true    Plugin was started
     * \retval  false   Plugin was not started
     */
    virtual bool start() = 0;

    //! Stops the plugin
    virtual void stop() = 0;

    /*!
     * Convenience method for casting this plugin to the specified interface
     *
     * \return  Pointer to the specified interface or a null pointer if this plugin does not
     *          implement the specified interface
     */
    template <typename T>
    T* interface()
    {
        return dynamic_cast<T *>(this);
    }
};

} // namespace CppPluginFramework
