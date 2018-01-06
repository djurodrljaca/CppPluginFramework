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
 * Contains a class for reading config files
 */

#ifndef CPPPLUGINFRAMEWORK_CONFIGFILE_HPP
#define CPPPLUGINFRAMEWORK_CONFIGFILE_HPP

// C++ Plugin Framework includes
#include <CppPluginFramework/PluginConfig.hpp>

// Qt includes
#include <QtCore/QJsonObject>

// System includes
#include <memory>

// Forward declarations

// Macros

namespace CppPluginFramework
{

/*!
 * This class enables loading of a plugin from a library
 */
class ConfigFile
{
public:
    /*!
     * Constructor
     */
    ConfigFile();

    /*!
     * Clears the object
     */
    void clear();

    /*!
     * Reads the specified config file
     *
     * \param   configFilePath  Path to the config file
     *
     * \retval  true    Success
     * \retval  true    Failure
     */
    bool read(const QString &configFilePath);

    /*!
     * Reads the specified config object
     *
     * \param   config  Config object
     *
     * \retval  true    Success
     * \retval  true    Failure
     */
    bool read(const QJsonObject &config);

    /*!
     * Gets custom environment variables that were read from the config file
     *
     * \return  Custom environment variables
     */
    QHash<QString, QString> customEnvironmentVariables() const;

    /*!
     * Gets plugin configs that were read from the config file
     *
     * \return  Plugin configs
     */
    QList<PluginConfig> pluginConfigs() const;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

}

#endif // CPPPLUGINFRAMEWORK_CONFIGFILE_HPP
