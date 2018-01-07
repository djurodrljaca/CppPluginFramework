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
 * Contains a convenience base class for creating plugins
 */

#ifndef CPPPLUGINFRAMEWORK_ABSTRACTPLUGIN_HPP
#define CPPPLUGINFRAMEWORK_ABSTRACTPLUGIN_HPP

// C++ Plugin Framework includes
#include <CppPluginFramework/IPlugin.hpp>

// Qt includes

// System includes
#include <memory>

// Forward declarations

// Macros

namespace CppPluginFramework
{

/*!
 * This is a convenience base class for creating plugins. It provides a common implementation one
 * part of the plugin interface.
 *
 * Derived classes still need to implement the following:
 * - Loading of a configuration
 * - Injection and ejection of dependencies
 * - Started state of the plugin (is started or not)
 * - Starting and stopping of the plugin
 */
class CPPPLUGINFRAMEWORK_LIBRARY_EXPORT AbstractPlugin : public IPlugin
{
public:
    /*!
     * Constructor
     *
     * \param   name    Plugin instance name
     */
    AbstractPlugin(const QString &name);

    /*!
     * Destructor
     */
    virtual ~AbstractPlugin() = default;

    /*!
     * \copydoc CppPluginFramework::IPlugin::name()
     */
    virtual QString name() const override;

    /*!
     * \copydoc CppPluginFramework::IPlugin::description()
     */
    virtual QString description() const override;

    /*!
     * \copydoc CppPluginFramework::IPlugin::version()
     */
    virtual VersionInfo version() const override;

    /*!
     * \copydoc CppPluginFramework::IPlugin::isInterfaceExported()
     */
    virtual bool isInterfaceExported(const QString &interface) const override;

    /*!
     * \copydoc CppPluginFramework::IPlugin::exportedInterfaces()
     */
    virtual QSet<QString> exportedInterfaces() const override;

protected:
    /*!
     * Sets the plugin's description
     *
     * \param   description     Plugin description
     */
    void setDescription(const QString &description);

    /*!
     * Sets the plugin's version
     *
     * \param   version     Plugin version
     */
    void setVersion(const VersionInfo &version);

    /*!
     * Sets the plugin's list of exported interfaces
     *
     * \param   interfaces  List of exported interfaces
     */
    void setExportedInterfaces(const QSet<QString> &interfaces);

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

}

#endif // CPPPLUGINFRAMEWORK_ABSTRACTPLUGIN_HPP
