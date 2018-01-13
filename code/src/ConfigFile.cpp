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
 * Contains a convenience method for reading config files
 */

// C++ Plugin Framework includes
#include <CppPluginFramework/ConfigFile.hpp>
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
     * Parses custom environment variables from the config JSON object
     *
     * \param   config  Config JSON object that contains the custom environment variables
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    bool parseCustomEnvironmentVariables(const QJsonObject &config);

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
     * \param   config  Config JSON object that contains the plugin config
     *
     * \return  Plugin config
     */
    PluginConfig parsePluginConfig(const QJsonObject &config);

    /*!
     * Parses plugin configs from the config JSON array
     *
     * \param   config  Config JSON array that contains the plugin configs
     *
     * \return  Plugin instance configs
     */
    QList<PluginInstanceConfig> parsePluginInstanceConfigs(const QJsonArray &config);

    /*!
     * Parses plugin instance config
     *
     * \param   config  Config JSON object that contains the plugin instance config
     *
     * \return  Plugin instance config
     */
    PluginInstanceConfig parsePluginInstanceConfig(const QJsonObject &config);

    /*!
     * Parses plugin instance dependency
     *
     * \param   config  Config JSON object that contains the plugin instance dependency
     *
     * \return  Plugin instance dependency
     */
    QString parseDependency(const QJsonObject &config);

    /*!
     * Reads the specified environment variable
     *
     * \param   variableName    Name of the environment variable to read
     *
     * \return  Environment variable value or an empty string if the variable was not found
     *
     * Environment variable is read primarily from custom environment variables. If it is not found
     * there then it attempts to read it from the system environment variables.
     */
    QString readEnvironmentVariable(const QString &variableName) const;

    /*!
     * Expands the specified variable
     *
     * \param       name            Name of the variable to expand
     * \param       value           Variable value to expand
     * \param[out]  expandedValue   Output for the expanded variable
     *
     * \retval  true    Success
     * \retval  false   Failure
     *
     * \note    A variable is not allowed to reference itself
     */
    bool expandVariable(const QString &name, const QString &value, QString *expandedValue) const;

    /*!
     * Expands all references to environment variables in the text
     *
     * \param   text    Text to extend
     *
     * \return  Extended text
     */
    QString expandText(const QString &text) const;

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
    QHash<QString, QString> m_customEnvironmentVariables;

    /*!
     * Holds the plugin configs
     */
    QList<PluginConfig> m_pluginConfigs;
};

bool ConfigFile::Impl::setWorkingDirPath(const QString &workingDirPath)
{
    const QDir workingDir(workingDirPath);
    bool success = false;

    if (workingDir.exists())
    {
        m_workingDir = workingDir;
        success = true;
    }

    return success;
}

QString ConfigFile::Impl::getAbsoluteFilePath(const QString &filePath) const
{
    QString absoluteFilePath;

    // Check if path is already absolute
    if (QDir::isAbsolutePath(filePath))
    {
        absoluteFilePath = filePath;
    }
    else
    {
        // Make the path absolute relative to the working directory
        absoluteFilePath = m_workingDir.absoluteFilePath(filePath);
    }

    return absoluteFilePath;
}

bool ConfigFile::Impl::parseCustomEnvironmentVariables(const QJsonObject &config)
{
    // Iterate over all items
    bool success = true;
    m_customEnvironmentVariables.clear();

    for (auto it = config.constBegin(); it != config.constEnd(); it++)
    {
        // Parse environment variable name and value
        const QString &name = it.key();
        success = Validation::validateEnvironmentVariableName(name);

        if (success)
        {
            const QJsonValue &value = it.value();

            if (value.isString())
            {
                if (m_customEnvironmentVariables.contains(name))
                {
                    success = false;
                    qDebug() << "CppPluginFramework::ConfigFile::Impl::"
                                "parseCustomEnvironmentVariables: "
                                "Error: variable with the same name already exists:"
                             << endl << "- name:" << name
                             << endl << "- value:" << value;
                }
                else
                {
                    m_customEnvironmentVariables[name] = value.toString();
                }
            }
            else
            {
                success = false;
                qDebug() << "CppPluginFramework::ConfigFile::Impl::"
                            "parseCustomEnvironmentVariables: "
                            "Error: invalid value:"
                         << endl << "- name:" << name
                         << endl << "- value:" << value;
            }
        }
        else
        {
            success = false;
            qDebug() << "CppPluginFramework::ConfigFile::Impl::parseCustomEnvironmentVariables: "
                        "Error: invalid name:" << name;
        }

        if (!success)
        {
            break;
        }
    }

    // Expand all environment variables (max 100 cycles)
    if (success)
    {
        for (int i = 0; i < 100; i++)
        {
            bool atLeastOneExpanded = false;

            for (auto it = m_customEnvironmentVariables.begin();
                 it != m_customEnvironmentVariables.end();
                 it++)
            {
                const QString &name = it.key();
                QString &value = it.value();
                QString expandedValue;

                success = expandVariable(name, value, &expandedValue);

                if (success)
                {
                    // Check if value was changed
                    if (expandedValue != value)
                    {
                        value = expandedValue;
                        atLeastOneExpanded = true;
                    }
                }
                else
                {
                    qDebug() << "CppPluginFramework::ConfigFile::Impl::"
                                "parseCustomEnvironmentVariables: Error: invalid name:" << name;
                    break;
                }
            }

            if (!atLeastOneExpanded)
            {
                // Finished as none of the variables were expanded
                break;
            }
        }
    }

    return success;
}

bool ConfigFile::Impl::parsePluginConfigs(const QJsonArray &config)
{
    // Iterate over all items
    m_pluginConfigs.clear();

    for (const QJsonValue item : config)
    {
        // Make sure that the config item is a JSON object
        if (!item.isObject())
        {
            qDebug() << "CppPluginFramework::ConfigFile::Impl::parsePluginConfigs: "
                        "Error: invalid plugin config:" << item;
            m_pluginConfigs.clear();
            break;
        }

        // Parse plugin config and add it to the plugin config list
        const PluginConfig parsedPluginConfig = parsePluginConfig(item.toObject());

        if (!parsedPluginConfig.isValid())
        {
            qDebug() << "CppPluginFramework::ConfigFile::Impl::parsePluginConfigs: "
                        "Error: failed to parse plugin config:" << item;
            m_pluginConfigs.clear();
            break;
        }

        m_pluginConfigs.append(parsedPluginConfig);
    }

    // At least one plugin is required
    return (!m_pluginConfigs.isEmpty());
}

PluginConfig ConfigFile::Impl::parsePluginConfig(const QJsonObject &config)
{
    bool success = false;
    PluginConfig pluginConfig;

    // Parse file path
    if (config.contains("pluginFilePath"))
    {
        if (config["pluginFilePath"].isString())
        {
            const QString filePath = expandText(config["pluginFilePath"].toString());
            const QString absoluteFilePath = getAbsoluteFilePath(filePath);

            success = Validation::validateFilePath(absoluteFilePath);

            if (success)
            {
                pluginConfig.setFilePath(absoluteFilePath);
            }
        }

        if (!success)
        {
            qDebug() << "CppPluginFramework::ConfigFile::Impl::parsePluginConfig: "
                        "Error: invalid plugin file path:" << config["pluginFilePath"];
        }
    }
    else
    {
        qDebug() << "CppPluginFramework::ConfigFile::Impl::parsePluginConfig: "
                    "Error: invalid plugin file path is missing:" << config;
    }

    // Parse version info
    if (success)
    {
        if (config.contains("version") &&
            (!config.contains("minVersion")) &&
            (!config.contains("maxVersion")))
        {
            // Parse required version
            if (config["version"].isString())
            {
                const VersionInfo version(config["version"].toString());
                success = version.isValid();

                if (success)
                {
                    pluginConfig.setVersion(version);
                }
                else
                {
                    qDebug() << "CppPluginFramework::ConfigFile::Impl::parsePluginConfig: "
                                "Error: invalid plugin version:" << config["version"];
                }
            }
            else
            {
                success = false;
                qDebug() << "CppPluginFramework::ConfigFile::Impl::parsePluginConfig: "
                            "Error: invalid plugin version:" << config["version"];
            }
        }
        else if ((!config.contains("version")) &&
                 config.contains("minVersion") &&
                 config.contains("maxVersion"))
        {
            // Parse required version range
            if (config["minVersion"].isString() &&
                config["maxVersion"].isString())
            {
                const VersionInfo minVersion(config["minVersion"].toString());
                const VersionInfo maxVersion(config["maxVersion"].toString());

                success = VersionInfo::isRangeValid(minVersion, maxVersion);

                if (success)
                {
                    pluginConfig.setMinVersion(minVersion);
                    pluginConfig.setMaxVersion(maxVersion);
                }
                else
                {
                    qDebug() << "CppPluginFramework::ConfigFile::Impl::parsePluginConfig: "
                                "Error: invalid plugin version range: "
                                "[" << config["minVersion"] << "," << config["maxVersion"] << "]";
                }
            }
            else
            {
                success = false;
                qDebug() << "CppPluginFramework::ConfigFile::Impl::parsePluginConfig: "
                            "Error: invalid plugin version range: "
                            "[" << config["minVersion"] << "," << config["maxVersion"] << "]";
            }
        }
        else
        {
            success = false;
            qDebug() << "CppPluginFramework::ConfigFile::Impl::parsePluginConfig: "
                        "Error: version requirement needs to be specified (either only "
                        "'version' or both 'minVersion' and 'maxVersion'):"
                     << config;
        }
    }

    // Parse plugin instances
    if (success)
    {
        success = config.contains("instances");

        if (success)
        {
            if (config["instances"].isArray())
            {
                // Parse plugin instance configs
                const QList<PluginInstanceConfig> pluginInstanceConfigs =
                        parsePluginInstanceConfigs(config["instances"].toArray());

                if (!pluginInstanceConfigs.isEmpty())
                {
                    pluginConfig.setInstanceConfigs(pluginInstanceConfigs);
                }
                else
                {
                    success = false;
                    qDebug() << "CppPluginFramework::ConfigFile::Impl::parsePluginConfig: "
                                "Error: failed to parse the plugin instance configs:" << config;
                }
            }
            else
            {
                success = false;
                qDebug() << "CppPluginFramework::ConfigFile::Impl::parsePluginConfig: "
                            "Error: plugin configs have to be stored in a JSON array!";
            }
        }
    }

    return pluginConfig;
}

QList<PluginInstanceConfig> ConfigFile::Impl::parsePluginInstanceConfigs(const QJsonArray &config)
{
    QList<PluginInstanceConfig> pluginInstanceConfigs;

    for (const QJsonValue item : config)
    {
        // Make sure that the config item is a JSON object
        if (!item.isObject())
        {
            qDebug() << "CppPluginFramework::ConfigFile::Impl::parsePluginInstanceConfigs: "
                        "Error: invalid plugin instance config:" << item;
            pluginInstanceConfigs.clear();
            break;
        }

        // Parse plugin instance config
        const PluginInstanceConfig pluginInstanceConfig =
                parsePluginInstanceConfig(item.toObject());

        if (!pluginInstanceConfig.isValid())
        {
            qDebug() << "CppPluginFramework::ConfigFile::Impl::parsePluginInstanceConfigs: "
                        "Error: failed to parse plugin instance config:" << item;
            pluginInstanceConfigs.clear();
            break;
        }

        pluginInstanceConfigs.append(pluginInstanceConfig);
    }

    return pluginInstanceConfigs;
}

PluginInstanceConfig ConfigFile::Impl::parsePluginInstanceConfig(const QJsonObject &config)
{
    bool success = false;
    PluginInstanceConfig pluginInstanceConfig;

    // Parse plugin instance name
    if (config.contains("instanceName"))
    {
        if (config["instanceName"].isString())
        {
            const QString instanceName = config["instanceName"].toString();

            success = Validation::validatePluginInstanceName(instanceName);

            if (success)
            {
                pluginInstanceConfig.setName(instanceName);
            }
        }

        if (!success)
        {
            qDebug() << "CppPluginFramework::ConfigFile::Impl::parsePluginInstanceConfig: "
                        "Error: invalid plugin instance name:" << config["instanceName"];
        }
    }
    else
    {
        qDebug() << "CppPluginFramework::ConfigFile::Impl::parsePluginInstanceConfig: "
                    "Error: invalid plugin instance name is missing:" << config;
    }

    // Parse config file path (optional parameter)
    if (success)
    {
        if (config.contains("configFilePath") && (!config.contains("config")))
        {
            if (config["configFilePath"].isString())
            {
                const QString filePath = expandText(config["configFilePath"].toString());
                const QString absoluteFilePath = getAbsoluteFilePath(filePath);

                success = Validation::validateFilePath(absoluteFilePath);

                if (success)
                {
                    // Read config from file
                    QJsonObject configObject;

                    if (readJsonObjectFromFile(absoluteFilePath, &configObject))
                    {
                        pluginInstanceConfig.setConfig(configObject);
                    }
                    else
                    {
                        success = false;
                        qDebug() << "CppPluginFramework::ConfigFile::Impl::parsePluginInstanceConfig: "
                                    "Error: failed to parse the config parameter:" << config;
                    }
                }
            }

            if (!success)
            {
                qDebug() << "CppPluginFramework::ConfigFile::Impl::parsePluginInstanceConfig: "
                            "Error: invalid config file path:" << config["configFilePath"];
            }
        }
        else if (config.contains("config") && (!config.contains("configFilePath")))
        {
            if (config["config"].isObject())
            {
                pluginInstanceConfig.setConfig(config["config"].toObject());
            }
            else
            {
                success = false;
                qDebug() << "CppPluginFramework::ConfigFile::Impl::parsePluginInstanceConfig: "
                            "Error: config parameter must me a JSON object:" << config;
            }
        }
        else if (config.contains("config") && config.contains("configFilePath"))
        {
            success = false;
            qDebug() << "CppPluginFramework::ConfigFile::Impl::parsePluginInstanceConfig: "
                        "Error: both config and config file path are set:" << config;
        }
        else
        {
            // Plugin instance does not have a config
        }
    }

    // Parse dependencies (optional parameter)
    if (success)
    {
        if (config.contains("dependencies"))
        {
            if (config["dependencies"].isArray())
            {
                QSet<QString> dependencies;

                for (const QJsonValue item : config["dependencies"].toArray())
                {
                    // Make sure that the config item is a JSON object
                    if (!item.isObject())
                    {
                        qDebug() << "CppPluginFramework::ConfigFile::Impl::parsePluginInstanceConfig: "
                                    "Error: invalid dependency:" << item;
                        success = false;
                        break;
                    }

                    // Parse dependency
                    const QString dependency = parseDependency(item.toObject());

                    if (dependency.isEmpty())
                    {
                        qDebug() << "CppPluginFramework::ConfigFile::Impl::parsePluginInstanceConfig: "
                                    "Error: invalid dependency:" << item;
                        success = false;
                        break;
                    }

                    dependencies.insert(dependency);
                }

                if (success)
                {
                    pluginInstanceConfig.setDependencies(dependencies);
                }
            }
            else
            {
                success = false;
                qDebug() << "CppPluginFramework::ConfigFile::Impl::parsePluginInstanceConfig: "
                            "Error: invalid config file path:" << config["configFilePath"];
            }
        }
    }

    // On error clear the instance config
    if (!success)
    {
        pluginInstanceConfig = PluginInstanceConfig();
    }

    return pluginInstanceConfig;
}

QString ConfigFile::Impl::parseDependency(const QJsonObject &config)
{
    QString dependency;

    if (config.contains("instanceName"))
    {
        if (config["instanceName"].isString())
        {
            dependency = config["instanceName"].toString();

            if (!Validation::validatePluginInstanceName(dependency))
            {
                dependency.clear();
            }
        }
    }

    if (dependency.isEmpty())
    {
        qDebug() << "CppPluginFramework::ConfigFile::Impl::parseDependency: "
                    "Error: invalid dependency:" << config;
    }

    return dependency;
}

QString ConfigFile::Impl::readEnvironmentVariable(const QString &variableName) const
{
    QString variableValue;

    if (m_customEnvironmentVariables.contains(variableName))
    {
        variableValue = m_customEnvironmentVariables[variableName];
    }
    else
    {
        variableValue = QProcessEnvironment::systemEnvironment().value(variableName);
    }

    return variableValue;
}

bool ConfigFile::Impl::expandVariable(const QString &name,
                                      const QString &value,
                                      QString *expandedValue) const
{
    bool success = (expandedValue != nullptr);

    if (success)
    {
        // Set the initial value
        *expandedValue = value;

        // Expand all referenced environment variables (max 100 cycles)
        QRegularExpression regex("\\${([a-zA-Z0-9_]+)}");

        for (int i = 0; i < 100; i++)
        {
            auto match = regex.match(*expandedValue);

            if (match.hasMatch())
            {
                const QString referenceName = match.captured(1);

                if (referenceName != name)
                {
                    const QString reference = match.captured(0);
                    const QString expandedReferenceValue = readEnvironmentVariable(referenceName);

                    expandedValue->replace(reference, expandedReferenceValue);
                }
                else
                {
                    success = false;
                    qDebug() << "CppPluginFramework::ConfigFile::Impl::expandVariable: "
                                "Error: variable value contains a reverence to itself:"
                             << endl << "- name:" << name
                             << endl << "- value:" << value;
                    break;
                }
            }
            else
            {
                // No references were found, exit loop
                break;
            }
        }
    }

    return success;
}

QString ConfigFile::Impl::expandText(const QString &text) const
{
    // Set the initial value
    QString expandedText = text;

    // Expand all referenced environment variables
    QRegularExpression regex("\\${([a-zA-Z0-9_]+)}");

    auto it = regex.globalMatch(expandedText);

    while (it.hasNext())
    {
        auto match = it.next();

        const QString reference = match.captured(0);
        const QString referenceName = match.captured(1);
        const QString expandedReferenceValue = readEnvironmentVariable(referenceName);

        expandedText.replace(reference, expandedReferenceValue);
    }

    return expandedText;
}

bool ConfigFile::Impl::readJsonObjectFromFile(const QString &filePath, QJsonObject *object)
{
    bool success = false;

    // Read config file
    QByteArray fileContent;

    if (object != nullptr)
    {
        QFile file(filePath);

        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            fileContent = file.readAll();
            file.close();
            success = true;
        }
        else
        {
            qDebug() << "CppPluginFramework::ConfigFile::Impl::readJsonObjectFromFile: "
                        "Error: Failed to open file:" << filePath;
        }
    }

    // Parse JSON file content
    if (success)
    {
        if (fileContent.isEmpty())
        {
            *object = QJsonObject();
        }
        else
        {
            const QJsonDocument doc = QJsonDocument::fromJson(fileContent);
            success = doc.isObject();

            if (success)
            {
                *object = doc.object();
            }
            else
            {
                qDebug() << "CppPluginFramework::ConfigFile::Impl::readJsonObjectFromFile: "
                            "Error: File does not contain a JSON object:" << filePath;
            }
        }
    }

    return success;
}

// -------------------------------------------------------------------------------------------------
// API
// -------------------------------------------------------------------------------------------------

ConfigFile::ConfigFile()
    : m_impl(std::make_unique<ConfigFile::Impl>())
{
}

ConfigFile::ConfigFile(const ConfigFile &other)
    : m_impl(std::make_unique<ConfigFile::Impl>(*(other.m_impl)))
{
}

ConfigFile::~ConfigFile()
{
}

ConfigFile &ConfigFile::operator=(ConfigFile rhs)
{
    m_impl = std::move(rhs.m_impl);

    return *this;
}

void ConfigFile::clear()
{
    m_impl->m_customEnvironmentVariables.clear();
    m_impl->m_pluginConfigs.clear();
}

bool ConfigFile::read(const QJsonObject &config, const QString &workingDirPath)
{
    clear();

    // Apply the specified working directory
    bool success = m_impl->setWorkingDirPath(workingDirPath);

    // Read custom environment variables
    if (config.contains("customEnvironmentVariables"))
    {
        if (config["customEnvironmentVariables"].isObject())
        {
            success = m_impl->parseCustomEnvironmentVariables(
                          config["customEnvironmentVariables"].toObject());
        }
        else
        {
            if (!config["customEnvironmentVariables"].isNull())
            {
                success = false;
                qDebug() << "CppPluginFramework::ConfigFile::read: "
                            "Error: if custom environment variables are needed then they have "
                            "to be stored in a JSON object!";
            }
        }
    }

    // Read plugin configs
    if (success)
    {
        success = config.contains("plugins");

        if (success)
        {
            if (config["plugins"].isArray())
            {
                success = m_impl->parsePluginConfigs(config["plugins"].toArray());
            }
            else
            {
                success = false;
                qDebug() << "CppPluginFramework::ConfigFile::read: "
                            "Error: plugin configs have to be stored in a JSON array!";
            }
        }
    }

    return success;
}

bool ConfigFile::read(const QString &configFilePath, const QString &workingDirPath)
{
    // Read config from file
    QJsonObject config;
    bool success = Impl::readJsonObjectFromFile(configFilePath, &config);

    if (success)
    {
        success = read(config, workingDirPath);
    }
    else
    {
        qDebug() << "CppPluginFramework::ConfigFile::read: Error: failed to read config file:"
                 << configFilePath;
    }

    return success;
}

QHash<QString, QString> ConfigFile::customEnvironmentVariables() const
{
    return m_impl->m_customEnvironmentVariables;
}

QList<PluginConfig> ConfigFile::pluginConfigs() const
{
    return m_impl->m_pluginConfigs;
}

}
