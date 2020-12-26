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

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppPluginFramework
{

AbstractPlugin::AbstractPlugin(const QString &name,
                               const VersionInfo &version,
                               const QString &description,
                               const QSet<QString> &exportedInterfaces)
    : m_name(name),
      m_version(version),
      m_description(description),
      m_exportedInterfaces(exportedInterfaces),
      m_started(false)
{
}

// -------------------------------------------------------------------------------------------------

QString AbstractPlugin::name() const
{
    QMutexLocker locker(&m_mutex);
    return m_name;
}

// -------------------------------------------------------------------------------------------------

QString AbstractPlugin::description() const
{
    QMutexLocker locker(&m_mutex);

    return m_description;
}

// -------------------------------------------------------------------------------------------------

VersionInfo AbstractPlugin::version() const
{
    QMutexLocker locker(&m_mutex);
    return m_version;
}

// -------------------------------------------------------------------------------------------------

bool AbstractPlugin::isInterfaceExported(const QString &interface) const
{
    QMutexLocker locker(&m_mutex);
    return m_exportedInterfaces.contains(interface);
}

// -------------------------------------------------------------------------------------------------

QSet<QString> AbstractPlugin::exportedInterfaces() const
{
    QMutexLocker locker(&m_mutex);
    return m_exportedInterfaces;
}

// -------------------------------------------------------------------------------------------------

bool AbstractPlugin::isStarted() const
{
    QMutexLocker locker(&m_mutex);
    return m_started;
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

    QMutexLocker locker(&m_mutex);
    m_started = success;
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

    QMutexLocker locker(&m_mutex);
    m_started = false;
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

} // namespace CppPluginFramework
