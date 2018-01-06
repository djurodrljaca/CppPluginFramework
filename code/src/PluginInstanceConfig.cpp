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

// C++ Plugin Framework includes
#include <CppPluginFramework/PluginInstanceConfig.hpp>
#include <CppPluginFramework/Validation.hpp>

// Qt includes
#include <QtCore/QFileInfo>

// System includes

// Forward declarations

// Macros

namespace CppPluginFramework
{

PluginInstanceConfig::PluginInstanceConfig(const QString &name,
                                           const QJsonObject &config,
                                           const QSet<QString> &dependencies)
    : m_name(name),
      m_config(config),
      m_dependencies(dependencies)
{
}

bool PluginInstanceConfig::isValid() const
{
    // Check name
    bool valid = Validation::validatePluginInstanceName(m_name);

    // No need to check the config because it is optional

    // Check (optional) dependencies
    if (valid && (!m_dependencies.isEmpty()))
    {
        // Check individual dependency if it is referencing a valid name
        for (const QString &dependency : m_dependencies)
        {
            if ((dependency == m_name) ||
                (!Validation::validatePluginInstanceName(dependency)))
            {
                valid = false;
                break;
            }
        }
    }

    return valid;
}

QString PluginInstanceConfig::name() const
{
    return m_name;
}

void PluginInstanceConfig::setName(const QString &name)
{
    m_name = name;
}

QJsonObject PluginInstanceConfig::config() const
{
    return m_config;
}

void PluginInstanceConfig::setConfig(const QJsonObject &config)
{
    m_config = config;
}

QSet<QString> PluginInstanceConfig::dependencies() const
{
    return m_dependencies;
}

void PluginInstanceConfig::setDependencies(const QSet<QString> &dependencies)
{
    m_dependencies = dependencies;
}

}
