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
 * This class reads and holds the plugin configuration and the environment variables
 */

// Own header
#include <CppPluginFramework/ConfigFile.hpp>

// C++ Plugin Framework includes
#include "LogHelper.hpp"
#include <CppPluginFramework/Validation.hpp>
#include <CppPluginFramework/VersionInfo.hpp>

// Qt includes
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>
#include <QtCore/QProcessEnvironment>
#include <QtCore/QRegularExpression>
#include <QtCore/QtDebug>

// System includes

// Forward declarations

// Macros
#define LOG_METHOD(METHOD)      CPPPLUGINFRAMEWORK_LOG_METHOD("ConfigFile::" METHOD)
#define LOG_METHOD_IMPL(METHOD) CPPPLUGINFRAMEWORK_LOG_METHOD("ConfigFile::Impl::" METHOD)

namespace CppPluginFramework
{

// -------------------------------------------------------------------------------------------------
// Implementation
// -------------------------------------------------------------------------------------------------

/*!
 * Private implementation struct
 */
struct ConfigFile::Impl
{
    /*!
     * Sets the working directory
     *
     * \param   workingDirPath  Working directory
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    bool setWorkingDirPath(const QString &workingDirPath);

    /*!
     * Gets an absolute path for the specified file path
     *
     * \param   filePath    File path
     *
     * \return  Absolute file path
     */
    QString getAbsoluteFilePath(const QString &filePath) const;

    /*!
     * Parses environment variables from the config JSON object
     *
     * \param   config  Config JSON object that contains the environment variables
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    bool parseEnvironmentVariables(const QJsonObject &config);

    /*!
     * Parses plugin configs from the config JSON array
     *
     * \param   config  Config JSON array that contains the plugin configs
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    bool parsePluginConfigs(const QJsonArray &config);

    /*!
     * Parses plugin config
     *
     * \param       config          Config JSON object that contains the plugin config
     * \param[out]  pluginConfig    Output for the parsed plugin config
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    bool parsePluginConfig(const QJsonObject &config, PluginConfig *pluginConfig);

    /*!
     * Parses the plugin version requirement (exact version or a version range)
     *
     * \param       config          JSON object to parse
     * \param[out]  pluginConfig    Plugin config where the version requirement needs to be stored
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    bool parsePluginVersionRequirement(const QJsonObject &config, PluginConfig *pluginConfig) const;

    /*!
     * Parses plugin configs from the config JSON array
     *
     * \param       config                  Config JSON array that contains the plugin configs
     * \param[out]  pluginInstanceConfigs   Output for the parsed plugin instance configs
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    bool parsePluginInstanceConfigs(const QJsonArray &config,
                                    QList<PluginInstanceConfig> *pluginInstanceConfigs);

    /*!
     * Parses plugin instance config
     *
     * \param       config                  Config JSON object that contains the plugin instance
     *                                      config
     * \param[out]  pluginInstanceConfig    Output for the parsed plugin instance config
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    bool parsePluginInstanceConfig(const QJsonObject &config,
                                   PluginInstanceConfig *pluginInstanceConfig);

    /*!
     * Parses plugin instance dependency
     *
     * \param       config      Config JSON object that contains the plugin instance dependency
     * \param[out]  dependency  Output for the parsed plugin instance dependency
     *
     * \retval  true    Success
     * \retval  false   Failure
     *
     * The plugin dependency from the JSON object is parsed and validated.
     */
    bool parseDependency(const QJsonObject &config, QString *dependency);

    /*!
     * Parses the file path from the JSON value
     *
     * \param       value               JSON value to parse
     * \param[out]  absoluteFilePath    Output for the parsed file path
     *
     * \retval  true    Success
     * \retval  false   Failure
     *
     * The file path from the JSON value is parsed, converted to an absolute file path if necessary,
     * and validated.
     */
    bool parseFilePath(const QJsonValue &value, QString *absoluteFilePath) const;

    /*!
     * Parses the plugin instance name from the JSON value
     *
     * \param       value               JSON value to parse
     * \param[out]  pluginInstanceName  Output for the parsed plugin instance name
     *
     * \retval  true    Success
     * \retval  false   Failure
     *
     * The plugin instance name from the JSON value is parsed and validated.
     */
    bool parsePluginInstanceName(const QJsonValue &value, QString *pluginInstanceName) const;

    /*!
     * Reads a JSON object from a file
     *
     * \param       filePath    Path to the JSON file
     * \param[out]  fileContent Output for the file content
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    static bool readFile(const QString &filePath, QByteArray *fileContent);

    /*!
     * Parses a JSON object from raw JSON data
     *
     * \param       json    Raw JSON data
     * \param[out]  object  Output for the JSON object
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    static bool readJsonObject(const QByteArray &json, QJsonObject *object);

    /*!
     * Reads a JSON object from a file
     *
     * \param       filePath    Path to the JSON file
     * \param[out]  object      Output for the JSON object
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    static bool readJsonObjectFromFile(const QString &filePath, QJsonObject *object);

    /*!
     * Holds the working directory
     */
    QDir m_workingDir;

    /*!
     * Holds the custom environment variables
     */
    EnvironmentVariables m_environmentVariables;

    /*!
     * Holds the plugin configs
     */
    QList<PluginConfig> m_pluginConfigs;
};

// -------------------------------------------------------------------------------------------------

bool ConfigFile::Impl::setWorkingDirPath(const QString &workingDirPath)
{
    const QDir workingDir(workingDirPath);

    if (!workingDir.exists())
    {
        qDebug() << LOG_METHOD_IMPL("setWorkingDirPath")
                 << "Error: working directory path doesn't exist:" << workingDirPath;
        return false;
    }

    m_workingDir = workingDir;
    return true;
}

// -------------------------------------------------------------------------------------------------

QString ConfigFile::Impl::getAbsoluteFilePath(const QString &filePath) const
{
    // Check if path is already absolute
    if (QDir::isAbsolutePath(filePath))
    {
        return filePath;
    }

    // Make the absolute path that is relative to the working directory
    return m_workingDir.absoluteFilePath(filePath);
}

// -------------------------------------------------------------------------------------------------

bool ConfigFile::Impl::parseEnvironmentVariables(const QJsonObject &config)
{
    // Iterate over all items
    for (auto it = config.constBegin(); it != config.constEnd(); it++)
    {
        // Parse environment variable name
        const QString &name = it.key();

        if(!Validation::validateEnvironmentVariableName(name))
        {
            qDebug() << LOG_METHOD_IMPL("parseEnvironmentVariables")
                     << "Error: invalid name:" << name;
            return false;
        }

        // Parse environment variable value
        const QJsonValue &value = it.value();

        if (!value.isString())
        {
            qDebug() << LOG_METHOD_IMPL("parseEnvironmentVariables")
                     << "Error: invalid value! Only string values are allowed:" << endl
                     << "- name:" << name << endl
                     << "- value:" << value;
            return false;
        }

        // Store parsed environment variable
        m_environmentVariables.setValue(name, value.toString());
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

bool ConfigFile::Impl::parsePluginConfigs(const QJsonArray &config)
{
    // Expand all text inside the config JSON object
    const QByteArray json = QJsonDocument(config).toJson(QJsonDocument::Compact);
    const QByteArray expandedJson = m_environmentVariables.expandText(json);
    const QJsonArray expandedConfig = QJsonDocument::fromJson(expandedJson).array();

    // Iterate over all items
    m_pluginConfigs.clear();

    for (const QJsonValue item : expandedConfig)
    {
        // Make sure that the config item is a JSON object
        if (!item.isObject())
        {
            qDebug() << LOG_METHOD_IMPL("parsePluginConfigs")
                     << "Error: invalid plugin config item! JSON object is expected:" << item;
            return false;
        }

        // Parse plugin config and add it to the plugin config list
        PluginConfig pluginConfig;

        if (!parsePluginConfig(item.toObject(), &pluginConfig))
        {
            qDebug() << LOG_METHOD_IMPL("parsePluginConfigs")
                     << "Error: failed to parse plugin config:" << item;
            return false;
        }

        m_pluginConfigs.append(pluginConfig);
    }

    // At least one plugin is required
    if (m_pluginConfigs.isEmpty())
    {
        qDebug() << LOG_METHOD_IMPL("parsePluginConfigs")
                 << "Error: at least one plugin config is needed!";
        return false;
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

bool ConfigFile::Impl::parsePluginConfig(const QJsonObject &config, PluginConfig *pluginConfig)
{
    Q_ASSERT(pluginConfig != nullptr);

    // Parse file path
    if (!config.contains("pluginFilePath"))
    {
        qDebug() << LOG_METHOD_IMPL("parsePluginConfig")
                 << "Error: invalid plugin file path is missing:" << config;
        return false;
    }

    QString absoluteFilePath;

    if (!parseFilePath(config["pluginFilePath"], &absoluteFilePath))
    {
        qDebug() << LOG_METHOD_IMPL("parsePluginConfig")
                 << "Error: failed to parse plugin file path:" << config;
        return false;
    }

    pluginConfig->setFilePath(absoluteFilePath);

    // Parse version requirement
    if (!parsePluginVersionRequirement(config, pluginConfig))
    {
        qDebug() << LOG_METHOD_IMPL("parsePluginConfig")
                 << "Error: failed to parse plugin version requirement:" << config;
        return false;
    }

    // Parse plugin instances
    if (!config.contains("instances"))
    {
        qDebug() << LOG_METHOD_IMPL("parsePluginConfig")
                 << "Error: no plugin instances!";
        return false;
    }

    if (!config["instances"].isArray())
    {
        qDebug() << LOG_METHOD_IMPL("parsePluginConfig")
                 << "Error: plugin instance configs have to be stored in a JSON array!";
        return false;
    }

    // Parse plugin instance configs
    QList<PluginInstanceConfig> pluginInstanceConfigs;

    if (!parsePluginInstanceConfigs(config["instances"].toArray(), &pluginInstanceConfigs))
    {
        qDebug() << LOG_METHOD_IMPL("parsePluginConfig")
                 << "Error: failed to parse the plugin instance configs:" << config;
        return false;
    }

    pluginConfig->setInstanceConfigs(pluginInstanceConfigs);
    return true;
}

// -------------------------------------------------------------------------------------------------

bool ConfigFile::Impl::parsePluginVersionRequirement(const QJsonObject &config,
                                                     PluginConfig *pluginConfig) const
{
    Q_ASSERT(pluginConfig != nullptr);

    if (config.contains("version") &&
        (!config.contains("minVersion")) &&
        (!config.contains("maxVersion")))
    {
        // Parse required version
        if (!config["version"].isString())
        {
            qDebug() << LOG_METHOD_IMPL("parsePluginVersionRequirement")
                     << "Error: plugin version must be a string:" << config["version"];
            return false;
        }

        const VersionInfo version(config["version"].toString());

        if (!version.isValid())
        {
            qDebug() << LOG_METHOD_IMPL("parsePluginVersionRequirement")
                     << "Error: invalid plugin version:" << config["version"];
            return false;
        }

        pluginConfig->setVersion(version);
    }
    else if ((!config.contains("version")) &&
             config.contains("minVersion") &&
             config.contains("maxVersion"))
    {
        // Parse required version range
        if ((!config["minVersion"].isString()) || (!config["maxVersion"].isString()))
        {
            qDebug() << LOG_METHOD_IMPL("parsePluginVersionRequirement")
                     << "Error: plugin min and max versions must be strings: "
                        "[" << config["minVersion"] << "," << config["maxVersion"] << "]";
            return false;
        }

        const VersionInfo minVersion(config["minVersion"].toString());
        const VersionInfo maxVersion(config["maxVersion"].toString());

        if (!VersionInfo::isRangeValid(minVersion, maxVersion))
        {
            qDebug() << LOG_METHOD_IMPL("parsePluginVersionRequirement")
                     << "Error: invalid plugin version range: "
                        "[" << config["minVersion"] << "," << config["maxVersion"] << "]";
            return false;
        }

        pluginConfig->setMinVersion(minVersion);
        pluginConfig->setMaxVersion(maxVersion);
    }
    else
    {
        qDebug() << LOG_METHOD_IMPL("parsePluginVersionRequirement")
                 << "Error: version requirement needs to be specified (either only 'version' or "
                    "both 'minVersion' and 'maxVersion'):" << config;
        return false;
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

bool ConfigFile::Impl::parsePluginInstanceConfigs(
        const QJsonArray &config, QList<PluginInstanceConfig> *pluginInstanceConfigs)
{
    Q_ASSERT(pluginInstanceConfigs != nullptr);

    // Parse all plugin instance configs
    for (const QJsonValue item : config)
    {
        // Make sure that the config item is a JSON object
        if (!item.isObject())
        {
            qDebug() << LOG_METHOD_IMPL("parsePluginInstanceConfigs")
                     << "Error: plugin instance config must be a JSON object:" << item;
            return false;
        }

        // Parse plugin instance config
        PluginInstanceConfig pluginInstanceConfig;

        if (!parsePluginInstanceConfig(item.toObject(), &pluginInstanceConfig))
        {
            qDebug() << LOG_METHOD_IMPL("parsePluginInstanceConfigs")
                     << "Error: failed to parse plugin instance config:" << item;
            return false;
        }

        pluginInstanceConfigs->append(pluginInstanceConfig);
    }

    // Check if at least one plugin instance config was found
    if (pluginInstanceConfigs->isEmpty())
    {
        qDebug() << LOG_METHOD_IMPL("parsePluginInstanceConfigs")
                 << "Error: no plugin instance configs were found:" << config;
        return false;
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

bool ConfigFile::Impl::parsePluginInstanceConfig(const QJsonObject &config,
                                                 PluginInstanceConfig *pluginInstanceConfig)
{
    Q_ASSERT(pluginInstanceConfig != nullptr);

    // Parse plugin instance name
    if (!config.contains("instanceName"))
    {
        qDebug() << LOG_METHOD_IMPL("parsePluginInstanceConfig")
                 << "Error: plugin instance name is missing:" << config;
        return false;
    }

    QString instanceName;

    if (!parsePluginInstanceName(config["instanceName"], &instanceName))
    {
        qDebug() << LOG_METHOD_IMPL("parsePluginInstanceConfig")
                 << "Error: failed to parse plugin instance name:" << config["instanceName"];
        return false;
    }

    pluginInstanceConfig->setName(instanceName);

    // Parse plugin config (optional parameter)
    if (config.contains("configFilePath") && (!config.contains("config")))
    {
        // Parse config file path
        QString configFilePath;

        if (!parseFilePath(config["configFilePath"], &configFilePath))
        {
            qDebug() << LOG_METHOD_IMPL("parsePluginInstanceConfig")
                     << "Error: failed to parse config file path:" << config;
            return false;
        }

        // Read config from file
        QByteArray fileContent;

        if (!readFile(configFilePath, &fileContent))
        {
            qDebug() << LOG_METHOD_IMPL("parsePluginInstanceConfig")
                     << "Error: failed to read the config file:" << configFilePath;
            return false;
        }

        // Parse JSON object from the expand file content
        QJsonObject configObject;

        if (!readJsonObject(m_environmentVariables.expandText(fileContent), &configObject))
        {
            qDebug() << LOG_METHOD_IMPL("parsePluginInstanceConfig")
                     << "Error: failed to read the config file:" << configFilePath;
            return false;
        }

        pluginInstanceConfig->setConfig(configObject);
    }
    else if (config.contains("config") && (!config.contains("configFilePath")))
    {
        if (!config["config"].isObject())
        {
            qDebug() << LOG_METHOD_IMPL("parsePluginInstanceConfig")
                     << "Error: config parameter must me a JSON object:" << config;
            return false;
        }

        pluginInstanceConfig->setConfig(config["config"].toObject());
    }
    else if (config.contains("config") && config.contains("configFilePath"))
    {
        qDebug() << LOG_METHOD_IMPL("parsePluginInstanceConfig")
                 << "Error: both config and config file path are set:" << config;
        return false;
    }
    else
    {
        // Plugin instance does not have a config
    }

    // Parse dependencies (optional parameter)
    if (config.contains("dependencies"))
    {
        if (!config["dependencies"].isArray())
        {
            qDebug() << LOG_METHOD_IMPL("parsePluginInstanceConfig")
                     << "Error: dependencies must be a JSON array:" << config;
            return false;
        }

        QSet<QString> dependencies;

        for (const QJsonValue item : config["dependencies"].toArray())
        {
            // Make sure that the config item is a JSON object
            if (!item.isObject())
            {
                qDebug() << LOG_METHOD_IMPL("parsePluginInstanceConfig")
                         << "Error: dependency must be a JSON object:" << item;
                return false;
            }

            // Parse dependency
            QString dependency;

            if (!parseDependency(item.toObject(), &dependency))
            {
                qDebug() << LOG_METHOD_IMPL("parsePluginInstanceConfig")
                         << "Error: failed to parse dependency:" << item;
                return false;
            }

            if (dependencies.contains(dependency))
            {
                qDebug() << LOG_METHOD_IMPL("parsePluginInstanceConfig")
                         << "Error: dependency to this plugin instance was already added:"
                         << dependency;
                return false;
            }

            dependencies.insert(dependency);
        }

        pluginInstanceConfig->setDependencies(dependencies);
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

bool ConfigFile::Impl::parseDependency(const QJsonObject &config, QString *dependency)
{
    Q_ASSERT(dependency != nullptr);

    if (!config.contains("instanceName"))
    {
        qDebug() << LOG_METHOD_IMPL("parseDependency")
                 << "Error: instance name is missing:" << config;
        return false;
    }

    if (!parsePluginInstanceName(config["instanceName"], dependency))
    {
        qDebug() << LOG_METHOD_IMPL("parseDependency")
                 << "Error: failed to parse instance name:" << config;
        return false;
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

bool ConfigFile::Impl::parseFilePath(const QJsonValue &value, QString *absoluteFilePath) const
{
    Q_ASSERT(absoluteFilePath != nullptr);

    if (!value.isString())
    {
        qDebug() << LOG_METHOD_IMPL("parseFilePath")
                 << "Error: file path must be a string:" << value;
        return false;
    }

    *absoluteFilePath = getAbsoluteFilePath(value.toString());

    if (!Validation::validateFilePath(*absoluteFilePath))
    {
        qDebug() << LOG_METHOD_IMPL("parseFilePath")
                 << "Error: invalid file path:" << value;
        return false;
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

bool ConfigFile::Impl::parsePluginInstanceName(const QJsonValue &value,
                                               QString *pluginInstanceName) const
{
    Q_ASSERT(pluginInstanceName != nullptr);

    if (!value.isString())
    {
        qDebug() << LOG_METHOD_IMPL("parsePluginInstanceName")
                 << "Error: plugin instance name must be a string:" << value;
        return false;
    }

    *pluginInstanceName = value.toString();

    if (!Validation::validateInterfaceName(*pluginInstanceName))
    {
        qDebug() << LOG_METHOD_IMPL("parsePluginInstanceName")
                 << "Error: invalid plugin instance name:" << value;
        return false;
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

bool ConfigFile::Impl::readFile(const QString &filePath, QByteArray *fileContent)
{
    Q_ASSERT(fileContent != nullptr);
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << LOG_METHOD_IMPL("readFile")
                 << "Error: Failed to open file:" << filePath;
        return false;
    }

    *fileContent = file.readAll();
    return true;
}

// -------------------------------------------------------------------------------------------------

bool ConfigFile::Impl::readJsonObject(const QByteArray &json, QJsonObject *object)
{
    if (json.isEmpty())
    {
        *object = QJsonObject();
    }
    else
    {
        const QJsonDocument doc = QJsonDocument::fromJson(json);

        if (!doc.isObject())
        {
            qDebug() << LOG_METHOD_IMPL("readJsonObject")
                     << "Error: raw JSON data does not contain a JSON object:" << json;
            return false;
        }

        *object = doc.object();
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

bool ConfigFile::Impl::readJsonObjectFromFile(const QString &filePath, QJsonObject *object)
{
    // Read config file
    QByteArray fileContent;

    if (!readFile(filePath, &fileContent))
    {
        qDebug() << LOG_METHOD_IMPL("readJsonObjectFromFile")
                 << "Error: Failed to open file:" << filePath;
        return false;
    }

    // Parse JSON file content
    if (!readJsonObject(fileContent, object))
    {
        qDebug() << LOG_METHOD_IMPL("readJsonObjectFromFile")
                 << "Error: failed to parse a JSON object from file:" << filePath;
        return false;
    }

    return true;
}

// -------------------------------------------------------------------------------------------------
// API
// -------------------------------------------------------------------------------------------------

ConfigFile::ConfigFile()
    : m_impl(std::make_unique<ConfigFile::Impl>())
{
}

// -------------------------------------------------------------------------------------------------

ConfigFile::ConfigFile(const ConfigFile &other)
    : m_impl(std::make_unique<ConfigFile::Impl>(*(other.m_impl)))
{
}

// -------------------------------------------------------------------------------------------------

ConfigFile::~ConfigFile()
{
}

// -------------------------------------------------------------------------------------------------

ConfigFile &ConfigFile::operator=(ConfigFile rhs)
{
    m_impl = std::move(rhs.m_impl);

    return *this;
}

// -------------------------------------------------------------------------------------------------

void ConfigFile::clear()
{
    m_impl->m_environmentVariables.reset();
    m_impl->m_pluginConfigs.clear();
}

// -------------------------------------------------------------------------------------------------

bool ConfigFile::read(const QJsonObject &config, const QString &workingDirPath)
{
    clear();

    // Apply the specified working directory
    if (!m_impl->setWorkingDirPath(workingDirPath))
    {
        qDebug() << LOG_METHOD("read")
                 << "Error: failed to set the working directory path!";
        return false;
    }

    // Read custom environment variables
    if (config.contains("customEnvironmentVariables"))
    {
        if (!config["customEnvironmentVariables"].isObject())
        {
            qDebug() << LOG_METHOD("read")
                     << "Error: custom environment variables have to be stored in a JSON object!";
            return false;
        }

        if (!m_impl->parseEnvironmentVariables(config["customEnvironmentVariables"].toObject()))
        {
            qDebug() << LOG_METHOD("read")
                     << "Error: failed to parse custom environment variables!";
            return false;
        }
    }

    // Read plugin configs
    if (!config.contains("plugins"))
    {
        qDebug() << LOG_METHOD("read")
                 << "Error: plugin configs are missing!";
        return false;
    }

    if (!config["plugins"].isArray())
    {
        qDebug() << LOG_METHOD("read")
                 << "Error: plugin configs have to be stored in a JSON array!";
        return false;
    }

    if (!m_impl->parsePluginConfigs(config["plugins"].toArray()))
    {
        qDebug() << LOG_METHOD("read")
                 << "Error: failed to parse plugin configs!";
        return false;
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

bool ConfigFile::read(const QString &configFilePath, const QString &workingDirPath)
{
    // Read config from file
    QJsonObject config;

    if (!Impl::readJsonObjectFromFile(configFilePath, &config))
    {
        qDebug() << LOG_METHOD("read")
                 << "Error: failed to read config file:" << configFilePath;
        return false;
    }

    if (!read(config, workingDirPath))
    {
        qDebug() << LOG_METHOD("read")
                 << "Error: failed to read config file:" << configFilePath;
        return false;
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

const EnvironmentVariables &ConfigFile::environmentVariables() const
{
    return m_impl->m_environmentVariables;
}

// -------------------------------------------------------------------------------------------------

const QList<PluginConfig> &ConfigFile::pluginConfigs() const
{
    return m_impl->m_pluginConfigs;
}

}
