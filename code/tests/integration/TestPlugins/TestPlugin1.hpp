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

#ifndef CPPPLUGINFRAMEWORK_TESTPLUGINS_TESTPLUGIN1_HPP
#define CPPPLUGINFRAMEWORK_TESTPLUGINS_TESTPLUGIN1_HPP

// C++ Plugin Framework includes
#include <CppPluginFramework/AbstractPlugin.hpp>
#include <ITestPlugin1.hpp>

// Qt includes

// System includes

// Forward declarations

// Macros

namespace CppPluginFramework
{
namespace TestPlugins
{

class CPPPLUGINFRAMEWORK_PLUGIN_EXPORT TestPlugin1 :
        public CppPluginFramework::AbstractPlugin,
        public ITestPlugin1
{
public:
    TestPlugin1(const QString &name);
    virtual ~TestPlugin1() = default;

    virtual bool loadConfig(const QJsonObject &config,
                            const EnvironmentVariables &environmentVariables) override;
    virtual bool injectDependency(IPlugin *plugin) override;
    virtual void ejectDependencies() override;

    virtual QString value() const override;

private:
    QString m_configuredValue;
};

}
}

#endif // CPPPLUGINFRAMEWORK_TESTPLUGINS_TESTPLUGIN1_HPP
