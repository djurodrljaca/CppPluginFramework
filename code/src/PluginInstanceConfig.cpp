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
 * Contains a config class for a Plugin Instance
 */

// Own header
#include <CppPluginFramework/PluginInstanceConfig.hpp>

// C++ Plugin Framework includes
#include <CppPluginFramework/Validation.hpp>

// Qt includes
#include <QtCore/QStringBuilder>

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppPluginFramework
{

PluginInstanceConfig::PluginInstanceConfig(const QString &name,
                                           const CppConfigFramework::ConfigObjectNode &config,
                                           const QSet<QString> &dependencies)
    : CppConfigFramework::ConfigLoader(),
      m_name(name),
      m_config(std::move(config.clone()->toObject())),
      m_dependencies(dependencies)
{
}

// -------------------------------------------------------------------------------------------------

PluginInstanceConfig::PluginInstanceConfig(const PluginInstanceConfig &other)
    : CppConfigFramework::ConfigLoader(),
      m_name(other.m_name),
      m_config(std::move(other.m_config.clone()->toObject())),
      m_dependencies(other.m_dependencies)
{
}

// -------------------------------------------------------------------------------------------------

PluginInstanceConfig &PluginInstanceConfig::operator=(const PluginInstanceConfig &other)
{
    if (this == &other)
    {
        return *this;
    }

    m_name = other.m_name;
    m_config = std::move(other.m_config.clone()->toObject());
    m_dependencies = other.m_dependencies;
    return *this;
}

// -------------------------------------------------------------------------------------------------

bool PluginInstanceConfig::isValid() const
{
    return validateConfig().isEmpty();
}

// -------------------------------------------------------------------------------------------------

QString PluginInstanceConfig::name() const
{
    return m_name;
}

// -------------------------------------------------------------------------------------------------

void PluginInstanceConfig::setName(const QString &name)
{
    m_name = name;
}

// -------------------------------------------------------------------------------------------------

const CppConfigFramework::ConfigObjectNode &PluginInstanceConfig::config() const
{
    return m_config;
}

// -------------------------------------------------------------------------------------------------

void PluginInstanceConfig::setConfig(const CppConfigFramework::ConfigObjectNode &config)
{
    m_config = std::move(config.clone()->toObject());
}

// -------------------------------------------------------------------------------------------------

QSet<QString> PluginInstanceConfig::dependencies() const
{
    return m_dependencies;
}

// -------------------------------------------------------------------------------------------------

void PluginInstanceConfig::setDependencies(const QSet<QString> &dependencies)
{
    m_dependencies = dependencies;
}

// -------------------------------------------------------------------------------------------------

bool PluginInstanceConfig::loadConfigParameters(const CppConfigFramework::ConfigObjectNode &config,
                                                QString *error)
{
    // Load name
    if (!loadRequiredConfigParameter(&m_name, QStringLiteral("name"), config, error))
    {
        if (error != nullptr)
        {
            *error = QStringLiteral("Failed to load plugin instance's name. Error: ") % *error;
        }
        return false;
    }

    // Load config
    if (config.contains(QStringLiteral("config")))
    {
        // Extract the config
        const auto *node = config.member(QStringLiteral("config"));

        if (!node->isObject())
        {
            if (error != nullptr)
            {
                *error = QStringLiteral("Plugin instance's config is not an Object node");
            }
            return false;
        }

        m_config = std::move(node->clone()->toObject());
    }
    else
    {
        // No config
        m_config = CppConfigFramework::ConfigObjectNode();
    }

    // Load dependencies
    bool loaded = false;

    if (!loadOptionalConfigParameter(&m_dependencies,
                                     QStringLiteral("dependencies"),
                                     config,
                                     &loaded,
                                     error))
    {
        if (error != nullptr)
        {
            *error = QStringLiteral("Failed to load plugin instance's dependencies. Error: ") %
                     *error;
        }
        return false;
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

QString PluginInstanceConfig::validateConfig() const
{
    // Check name
    if (!Validation::validatePluginInstanceName(m_name))
    {
        return QStringLiteral("Name is not valid: ") % m_name;
    }

    // Config is optional

    // Check (optional) dependencies
    if (!m_dependencies.isEmpty())
    {
        // Check individual dependency if it is referencing a valid name
        for (const QString &dependency : m_dependencies)
        {
            if (dependency == m_name)
            {
                return QStringLiteral("Dependency name is the same as the plugin instance name: ") %
                        dependency;
            }

            if (!Validation::validatePluginInstanceName(dependency))
            {
                return QStringLiteral("Dependency's name is not valid: ") % dependency;
            }
        }
    }

    return QString();
}

} // namespace CppPluginFramework

// -------------------------------------------------------------------------------------------------

bool operator==(const CppPluginFramework::PluginInstanceConfig &left,
                const CppPluginFramework::PluginInstanceConfig &right)
{
    return ((left.name() == right.name()) &&
            (left.config() == right.config()) &&
            (left.dependencies() == right.dependencies()));
}

// -------------------------------------------------------------------------------------------------

bool operator!=(const CppPluginFramework::PluginInstanceConfig &left,
                const CppPluginFramework::PluginInstanceConfig &right)
{
    return !(left == right);
}
