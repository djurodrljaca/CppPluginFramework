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

#pragma once

// C++ Plugin Framework includes
#include <CppPluginFramework/AbstractPlugin.hpp>
#include <CppPluginFramework/PluginFactoryTemplate.hpp>
#include <ITestPlugin1.hpp>

// Qt includes

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppPluginFramework
{
namespace TestPlugins
{

class TestPlugin1 : public CppPluginFramework::AbstractPlugin, public ITestPlugin1
{
public:
    TestPlugin1(const QString &name);
    ~TestPlugin1() = default;

    bool loadConfig(const CppConfigFramework::ConfigObjectNode &config) override;
    bool injectDependency(IPlugin *plugin) override;
    void ejectDependencies() override;

    virtual QString value() const override;

private:
    QString m_configuredValue;
};

// -------------------------------------------------------------------------------------------------

class Q_DECL_EXPORT PluginFactory : public QObject, public PluginFactoryTemplate<TestPlugin1>
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
