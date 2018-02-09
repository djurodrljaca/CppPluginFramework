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

namespace CppPluginFramework
{
namespace TestPlugins
{

static const char s_version_string[] = "1.0.0";

TestPlugin2::TestPlugin2(const QString &name)
    : CppPluginFramework::AbstractPlugin(name),
      ITestPlugin2(),
      m_configuredDelimiter(),
      m_dependencies()
{
    setDescription("test plugin 2");
    setVersion(CppPluginFramework::VersionInfo(QLatin1String(s_version_string)));

    const QSet<QString> interfaces =
    {
        CPPPLUGINFRAMEWORK_INTERFACE_NAME(CppPluginFramework::TestPlugins::ITestPlugin2)
    };

    setExportedInterfaces(interfaces);
}

bool TestPlugin2::loadConfig(const QJsonObject &config,
                             const EnvironmentVariables &environmentVariables)
{
    Q_UNUSED(environmentVariables)
    bool success = false;

    if (config.contains("delimiter"))
    {
        auto delimiter = config["delimiter"];

        if (delimiter.isString())
        {
            m_configuredDelimiter = delimiter.toString();
            success = true;
        }
    }

    return success;
}

bool TestPlugin2::injectDependency(IPlugin *plugin)
{
    bool success = false;

    if (!isStarted())
    {
        if (plugin->isInterfaceExported(
                CPPPLUGINFRAMEWORK_INTERFACE_NAME(CppPluginFramework::TestPlugins::ITestPlugin1)))
        {
            CppPluginFramework::TestPlugins::ITestPlugin1 *interface =
                    plugin->interface<CppPluginFramework::TestPlugins::ITestPlugin1>();

            if (interface != nullptr)
            {
                m_dependencies.append(interface);
                success = true;
            }
        }
    }

    return success;
}

void TestPlugin2::ejectDependencies()
{
    if (!isStarted())
    {
        m_dependencies.clear();
    }
}

QString TestPlugin2::joinedValues() const
{
    QStringList values;

    for (auto *item : m_dependencies)
    {
        values.append(item->value());
    }

    values.sort();

    return values.join(m_configuredDelimiter);
}

}
}

const char *readPluginVersion()
{
    return CppPluginFramework::TestPlugins::s_version_string;
}

CppPluginFramework::IPlugin *createPluginInstance(const QString &instanceName)
{
    return new CppPluginFramework::TestPlugins::TestPlugin2(instanceName);
}
