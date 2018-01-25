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
#include <CppPluginFramework/EnvironmentVariables.hpp>
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
 * This class reads and holds the plugin configuration and the environment variables
 */
class CPPPLUGINFRAMEWORK_LIBRARY_EXPORT ConfigFile
{
public:
    /*!
     * Constructor
     */
    ConfigFile();

    /*!
     * Copy constructor
     *
     * \param   other   Object to copy
     */
    ConfigFile(const ConfigFile &other);

    /*!
     * Destructor
     */
    ~ConfigFile();

    /*!
     * Assignment operator
     *
     * \param   rhs     Object to copy
     */
    ConfigFile &operator=(ConfigFile rhs);

    /*!
     * Clears the object
     */
    void clear();

    /*!
     * Reads the specified config object
     *
     * \param   config          Config object
     * \param   workingDirPath  Working directory path
     *
     * \retval  true    Success
     * \retval  true    Failure
     *
     * If the working directory path is set then it is used instead of the application's working
     * directory.
     *
     * \note    All of the relative paths referenced in the config are relative to the set working
     *          directory
     */
    bool read(const QJsonObject &config, const QString &workingDirPath = QString());

    /*!
     * Reads the specified config file
     *
     * \param   configFilePath  Path to the config file
     * \param   workingDirPath  Working directory path
     *
     * \retval  true    Success
     * \retval  true    Failure
     *
     * \note    This is a convenience method which first reads the config file and then processes
     *          the extracted config JSON object.
     */
    bool read(const QString &configFilePath, const QString &workingDirPath = QString());

    /*!
     * Gets custom environment variables that were read from the config file
     *
     * \return  Custom environment variables
     */
    EnvironmentVariables environmentVariables() const;

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
