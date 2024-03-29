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
#include <CppPluginFramework/LoggingCategories.hpp>
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
    : m_name(name),
      m_config(std::move(config.clone()->toObject())),
      m_dependencies(dependencies)
{
}

// -------------------------------------------------------------------------------------------------

PluginInstanceConfig::PluginInstanceConfig(const PluginInstanceConfig &other)
    : m_name(other.m_name),
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

bool PluginInstanceConfig::loadConfigParameters(const CppConfigFramework::ConfigObjectNode &config)
{
    // Load name
    if (!loadRequiredConfigParameter(&m_name, QStringLiteral("name"), config))
    {
        qCWarning(CppPluginFramework::LoggingCategory::Config)
                << "Failed to load plugin instance's name!";
        return false;
    }

    // Load config
    const auto *configMember = config.member(QStringLiteral("config"));

    if (configMember == nullptr)
    {
        m_config = CppConfigFramework::ConfigObjectNode();
    }
    else
    {
        if (!configMember->isObject())
        {
            qCWarning(CppPluginFramework::LoggingCategory::Config)
                    << "Plugin instance's config is not an Object! Type:"
                    << CppConfigFramework::ConfigNode::typeToString(configMember->type());
            return false;
        }

        m_config = std::move(configMember->clone()->toObject());
    }

    // Load dependencies
    m_dependencies.clear();

    if (!loadOptionalConfigParameter(&m_dependencies, QStringLiteral("dependencies"), config))
    {
        qCWarning(CppPluginFramework::LoggingCategory::Config)
                << "Failed to load plugin instance's dependencies!";
        return false;
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

bool PluginInstanceConfig::storeConfigParameters(CppConfigFramework::ConfigObjectNode *config)
{
    // Storing config parameters is currently not supported
    Q_UNUSED(config)
    return false;
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
