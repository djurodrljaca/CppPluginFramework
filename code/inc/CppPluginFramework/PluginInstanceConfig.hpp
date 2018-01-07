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
 * Contains a class that holds a plugin's instance config
 */

#ifndef CPPPLUGINFRAMEWORK_PLUGININSTANCECONFIG_HPP
#define CPPPLUGINFRAMEWORK_PLUGININSTANCECONFIG_HPP

// C++ Plugin Framework includes
#include <CppPluginFramework/LibraryExport.hpp>

// Qt includes
#include <QtCore/QJsonObject>
#include <QtCore/QSet>

// System includes

// Forward declarations

// Macros

namespace CppPluginFramework
{

/*!
 * This class holds a plugin's instance config
 */
class CPPPLUGINFRAMEWORK_LIBRARY_EXPORT PluginInstanceConfig
{
public:
    /*!
     * Constructor
     */
    PluginInstanceConfig() = default;

    /*!
     * Constructor
     *
     * \param   name            Name of the plugin instance
     * \param   config          Plugin's instance config
     * \param   dependencies    List of plugin's dependencies
     */
    PluginInstanceConfig(const QString &name,
                         const QJsonObject &config = QJsonObject(),
                         const QSet<QString> &dependencies = QSet<QString>());

    /*!
     * Checks if plugin instance config is valid
     *
     * \retval  true    Plugin instance config is valid
     * \retval  false   Plugin instance config is not valid
     */
    bool isValid() const;

    /*!
     * Returns plugin's instance name
     *
     * \return  Plugin's instance name
     */
    QString name() const;

    /*!
     * Sets plugin's instance name
     *
     * \param   name    Plugin's instance name
     */
    void setName(const QString &name);

    /*!
     * Returns the plugin's instance config
     *
     * \return  Plugin's instance config
     */
    QJsonObject config() const;

    /*!
     * Sets the plugin's instance config
     *
     * \param   config  Plugin's instance config
     */
    void setConfig(const QJsonObject &config);

    /*!
     * Returns list of plugin's instance dependencies
     *
     * \return  List of plugin's instance dependencies
     */
    QSet<QString> dependencies() const;

    /*!
     * Sets list of plugin's instance dependencies
     *
     * \param   dependencies    List of plugin's instance dependencies
     */
    void setDependencies(const QSet<QString> &dependencies);

private:
    /*!
     * Holds the plugin's instance name
     */
    QString m_name;

    /*!
     * Holds the path to the plugin's instance config
     */
    QJsonObject m_config;

    /*!
     * Holds the list of plugin's instance dependencies
     */
    QSet<QString> m_dependencies;
};

}

#endif // CPPPLUGINFRAMEWORK_PLUGININSTANCECONFIG_HPP
