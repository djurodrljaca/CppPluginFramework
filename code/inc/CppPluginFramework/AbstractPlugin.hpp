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

#pragma once

// C++ Plugin Framework includes
#include <CppPluginFramework/IPlugin.hpp>

// Qt includes
#include <QtCore/QMutex>

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppPluginFramework
{

/*!
 * This is a convenience base class for creating plugins. It provides a common implementation a part
 * of the plugin interface.
 *
 * Derived classes still need to implement the following:
 * - Loading of a configuration
 * - Injection and ejection of dependencies
 *
 * Derived classes can also hook to events:
 * - Starting plugin
 * - Stopping plugin
 */
class CPPPLUGINFRAMEWORK_EXPORT AbstractPlugin : public IPlugin
{
public:
    /*!
     * Constructor
     *
     * \param   name    Plugin instance name
     * \param   version Plugin version
     */
    AbstractPlugin(const QString &name,
                   const VersionInfo &version,
                   const QString &description,
                   const QSet<QString> &exportedInterfaces);

    //! Destructor
    ~AbstractPlugin() override = default;

    //! \copydoc CppPluginFramework::IPlugin::name()
    QString name() const override final;

    //! \copydoc CppPluginFramework::IPlugin::version()
    VersionInfo version() const override final;

    //! \copydoc CppPluginFramework::IPlugin::description()
    QString description() const override final;

    //! \copydoc CppPluginFramework::IPlugin::isInterfaceExported()
    bool isInterfaceExported(const QString &interface) const override final;

    //! \copydoc CppPluginFramework::IPlugin::exportedInterfaces()
    QSet<QString> exportedInterfaces() const override final;

    //! \copydoc CppPluginFramework::IPlugin::isStarted()
    bool isStarted() const override final;

    //! \copydoc CppPluginFramework::IPlugin::start()
    bool start() override final;

    //! \copydoc CppPluginFramework::IPlugin::stop()
    void stop() override final;

private:
    /*!
     * Executes the startup procedure
     *
     * \retval  true    Success
     * \retval  false    Failure
     *
     * This method is called inside start() method if the plugin is not started yet. Default
     * implementation doesn't do anything. This method needs to be overridden if anything needs
     * to be done during startup.
     */
    virtual bool onStart();

    /*!
     * Executes the shutdown procedure
     *
     * This method is called inside stop() method if the plugin is not started yet. Default
     * implementation doesn't do anything. This method needs to be overridden if anything needs
     * to be done during startup.
     */
    virtual void onStop();

private:
    //! Enables thread-safe access to the plugin data
    mutable QMutex m_mutex;

    //! Holds the name of the plugin instance
    QString m_name;

    //! Holds the version of the plugin
    VersionInfo m_version;

    //! Holds the description of the plugin
    QString m_description;

    //! Holds the list of exported interfaces of the plugin
    QSet<QString> m_exportedInterfaces;

    //! Holds the "started" flag
    bool m_started;
};

}
