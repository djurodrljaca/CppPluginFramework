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

#ifndef CPPPLUGINFRAMEWORK_TESTPLUGINS_TESTPLUGIN2_HPP
#define CPPPLUGINFRAMEWORK_TESTPLUGINS_TESTPLUGIN2_HPP

// C++ Plugin Framework includes
#include <CppPluginFramework/AbstractPlugin.hpp>
#include <CppPluginFramework/PluginFactoryTemplate.hpp>
#include <ITestPlugin1.hpp>
#include <ITestPlugin2.hpp>

// Qt includes

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppPluginFramework
{
namespace TestPlugins
{

class CPPPLUGINFRAMEWORK_PLUGIN_EXPORT TestPlugin2 :
        public CppPluginFramework::AbstractPlugin,
        public ITestPlugin2
{
public:
    TestPlugin2(const QString &name);
    virtual ~TestPlugin2() = default;

    virtual bool loadConfig(const QJsonObject &config) override;
    virtual bool injectDependency(IPlugin *plugin) override;
    virtual void ejectDependencies() override;

    virtual QString joinedValues() const override;

private:
    bool onStart() override;

private:
    QString m_configuredDelimiter;
    QList<IPlugin*> m_dependencies;
};

// -------------------------------------------------------------------------------------------------

class CPPPLUGINFRAMEWORK_PLUGIN_EXPORT PluginFactory :
        public QObject,
        public PluginFactoryTemplate<TestPlugin2>
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "CppPluginFramework::IPluginFactory")
    Q_INTERFACES(CppPluginFramework::IPluginFactory)

public:
    //! Destructor
    ~PluginFactory() override = default;
};

} // namespace TestPlugins
} // namespace CppPluginFramework

#endif // CPPPLUGINFRAMEWORK_TESTPLUGINS_TESTPLUGIN2_HPP
