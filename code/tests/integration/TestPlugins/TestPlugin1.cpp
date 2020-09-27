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
 * Contains the "test plugin 1"
 */

// C++ Plugin Framework includes
#include "TestPlugin1.hpp"

// C++ Config Framework includes
#include <CppConfigFramework/ConfigValueNode.hpp>

// Cedar Framework includes
#include <CedarFramework/Deserialization.hpp>

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
static const QString s_description("test plugin 1");
static const QSet<QString> s_exportedInterfaces
{
    "CppPluginFramework::TestPlugins::ITestPlugin1"
};

// -------------------------------------------------------------------------------------------------

TestPlugin1::TestPlugin1(const QString &name)
    : CppPluginFramework::AbstractPlugin(name, s_version, s_description, s_exportedInterfaces),
      ITestPlugin1(),
      m_configuredValue()
{
}

// -------------------------------------------------------------------------------------------------

bool TestPlugin1::loadConfig(const QJsonObject &config)
{
    return CedarFramework::deserializeNode(config, "value", &m_configuredValue);
}

// -------------------------------------------------------------------------------------------------

bool TestPlugin1::injectDependency(IPlugin *plugin)
{
    // This plugin doesn't support dependencies!
    Q_UNUSED(plugin)
    return false;
}

// -------------------------------------------------------------------------------------------------

void TestPlugin1::ejectDependencies()
{
}

// -------------------------------------------------------------------------------------------------

QString TestPlugin1::value() const
{
    return m_configuredValue;
}

} // namespace TestPlugins
} // namespace CppPluginFramework
