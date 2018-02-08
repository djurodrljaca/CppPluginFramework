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

// Qt includes

// System includes

// Forward declarations

// Macros

namespace CppPluginFramework
{
namespace TestPlugins
{

static const char s_version_string[] = "1.0.0";

TestPlugin1::TestPlugin1(const QString &name)
    : CppPluginFramework::AbstractPlugin(name),
      ITestPlugin1(),
      m_started(false),
      m_configuredValue()
{
    setDescription("test plugin 1");
    setVersion(CppPluginFramework::VersionInfo(QLatin1String(s_version_string)));

    const QSet<QString> interfaces =
    {
        CPPPLUGINFRAMEWORK_INTERFACE_NAME(CppPluginFramework::TestPlugins::ITestPlugin1)
    };

    setExportedInterfaces(interfaces);
}

bool TestPlugin1::loadConfig(const QJsonObject &config,
                             const EnvironmentVariables &environmentVariables)
{
    Q_UNUSED(environmentVariables)
    bool success = false;

    if (config.contains("value"))
    {
        auto value = config["value"];

        if (value.isString())
        {
            m_configuredValue = value.toString();
            success = true;
        }
    }

    return success;
}

bool TestPlugin1::injectDependency(IPlugin *plugin)
{
    // This plugin doesn't support dependencies!
    Q_UNUSED(plugin)
    return false;
}

void TestPlugin1::ejectDependencies()
{
}

bool TestPlugin1::isStarted()
{
    return m_started;
}

bool TestPlugin1::start()
{
    bool success = false;

    if (!m_configuredValue.isEmpty())
    {
        m_started = true;
        success = true;
    }

    return success;
}

void TestPlugin1::stop()
{
    m_started = false;
}

QString TestPlugin1::value() const
{
    return m_configuredValue;
}

}
}

const char *readPluginVersion()
{
    return CppPluginFramework::TestPlugins::s_version_string;
}

CppPluginFramework::IPlugin *createPluginInstance(const QString &instanceName)
{
    return new CppPluginFramework::TestPlugins::TestPlugin1(instanceName);
}
