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

// Own header
#include <CppPluginFramework/AbstractPlugin.hpp>

// C++ Plugin Framework includes

// Qt includes
#include <QtCore/QMutex>
#include <QtCore/QtDebug>

// System includes

// Forward declarations

// Macros

namespace CppPluginFramework
{

// -------------------------------------------------------------------------------------------------
// Implementation
// -------------------------------------------------------------------------------------------------

/*!
 * Private implementation struct
 */
struct AbstractPlugin::Impl
{
    /*!
     * Holds the name of the plugin instance
     */
    QString m_name;

    /*!
     * Holds the description of the plugin
     */
    QString m_description;

    /*!
     * Holds the version of the plugin
     */
    VersionInfo m_version;

    /*!
     * Holds the list of exported interfaces of the plugin
     */
    QSet<QString> m_exportedInterfaces;

    /*!
     * Holds the "started" flag
     */
    bool m_started;

    /*!
     * Enables access serialization between threads
     */
    QMutex m_mutex;
};

// -------------------------------------------------------------------------------------------------
// API
// -------------------------------------------------------------------------------------------------

AbstractPlugin::AbstractPlugin(const QString &name)
    : IPlugin(),
      m_impl(std::make_unique<AbstractPlugin::Impl>())
{
    m_impl->m_name = name;
}

// -------------------------------------------------------------------------------------------------

AbstractPlugin::~AbstractPlugin()
{
}

// -------------------------------------------------------------------------------------------------

QString AbstractPlugin::name() const
{
    QMutexLocker locker(&m_impl->m_mutex);

    return m_impl->m_name;
}

// -------------------------------------------------------------------------------------------------

QString AbstractPlugin::description() const
{
    QMutexLocker locker(&m_impl->m_mutex);

    return m_impl->m_description;
}

// -------------------------------------------------------------------------------------------------

VersionInfo AbstractPlugin::version() const
{
    QMutexLocker locker(&m_impl->m_mutex);

    return m_impl->m_version;
}

// -------------------------------------------------------------------------------------------------

bool AbstractPlugin::isInterfaceExported(const QString &interface) const
{
    QMutexLocker locker(&m_impl->m_mutex);

    return m_impl->m_exportedInterfaces.contains(interface);
}

// -------------------------------------------------------------------------------------------------

QSet<QString> AbstractPlugin::exportedInterfaces() const
{
    QMutexLocker locker(&m_impl->m_mutex);

    return m_impl->m_exportedInterfaces;
}

// -------------------------------------------------------------------------------------------------

bool AbstractPlugin::isStarted() const
{
    QMutexLocker locker(&m_impl->m_mutex);

    return m_impl->m_started;
}

// -------------------------------------------------------------------------------------------------

bool AbstractPlugin::start()
{
    // Check if plugin is already started
    if (isStarted())
    {
        // Error, plugin is already started
        return false;
    }

    // Start the plugin
    const bool success = onStart();

    QMutexLocker locker(&m_impl->m_mutex);
    m_impl->m_started = success;
    return success;
}

// -------------------------------------------------------------------------------------------------

void AbstractPlugin::stop()
{
    // Check if plugin is actually started
    if (!isStarted())
    {
        // Plugin is already stopped
        return;
    }

    // Stop the plugin
    onStop();

    QMutexLocker locker(&m_impl->m_mutex);
    m_impl->m_started = false;
}

// -------------------------------------------------------------------------------------------------

void AbstractPlugin::setDescription(const QString &description)
{
    QMutexLocker locker(&m_impl->m_mutex);

    m_impl->m_description = description;
}

// -------------------------------------------------------------------------------------------------

void AbstractPlugin::setVersion(const VersionInfo &version)
{
    QMutexLocker locker(&m_impl->m_mutex);

    m_impl->m_version = version;
}

// -------------------------------------------------------------------------------------------------

void AbstractPlugin::setExportedInterfaces(const QSet<QString> &interfaces)
{
    QMutexLocker locker(&m_impl->m_mutex);

    m_impl->m_exportedInterfaces = interfaces;
}

// -------------------------------------------------------------------------------------------------

bool AbstractPlugin::onStart()
{
    return true;
}

// -------------------------------------------------------------------------------------------------

void AbstractPlugin::onStop()
{
}

}
