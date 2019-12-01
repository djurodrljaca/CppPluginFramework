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
 * Contains the "test plugin 2"
 */

// C++ Plugin Framework includes
#include "TestPlugin2.hpp"

// Qt includes

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppPluginFramework
{
namespace TestPlugins
{

// -------------------------------------------------------------------------------------------------

static const VersionInfo s_version("1.0.0");
static const QString s_description("test plugin 2");
static const QSet<QString> s_exportedInterfaces
{
    "CppPluginFramework::TestPlugins::ITestPlugin2"
};

// -------------------------------------------------------------------------------------------------

TestPlugin2::TestPlugin2(const QString &name)
    : CppPluginFramework::AbstractPlugin(name, s_version, s_description, s_exportedInterfaces),
      ITestPlugin2(),
      m_configuredDelimiter(),
      m_dependencies()
{
}

// -------------------------------------------------------------------------------------------------

bool TestPlugin2::loadConfig(const CppConfigFramework::ConfigObjectNode &config)
{
    bool success = false;

    if (config.contains("delimiter"))
    {
        auto node = config.member("delimiter");

        if (node->isValue())
        {
            m_configuredDelimiter = node->toValue().value().toString();
            success = true;
        }
    }

    return success;
}

// -------------------------------------------------------------------------------------------------

bool TestPlugin2::injectDependency(IPlugin *plugin)
{
    if (isStarted())
    {
        return false;
    }

    if (!plugin->isInterfaceExported("CppPluginFramework::TestPlugins::ITestPlugin1"))
    {
        return false;
    }

    auto *interface = plugin->interface<CppPluginFramework::TestPlugins::ITestPlugin1>();

    if (interface == nullptr)
    {
        return false;
    }

    m_dependencies.append(plugin);
    return true;
}

// -------------------------------------------------------------------------------------------------

void TestPlugin2::ejectDependencies()
{
    if (!isStarted())
    {
        m_dependencies.clear();
    }
}

// -------------------------------------------------------------------------------------------------

QString TestPlugin2::joinedValues() const
{
    QStringList values;

    for (auto *dependency : m_dependencies)
    {
        auto *interface = dependency->interface<CppPluginFramework::TestPlugins::ITestPlugin1>();
        values.append(interface->value());
    }

    values.sort();

    return values.join(m_configuredDelimiter);
}

// -------------------------------------------------------------------------------------------------

bool TestPlugin2::onStart()
{
    if (m_dependencies.isEmpty())
    {
        return false;
    }

    for (auto *dependency : m_dependencies)
    {
        if (!dependency->isStarted())
        {
            return false;
        }
    }

    return true;
}

} // namespace TestPlugins
} // namespace CppPluginFramework
