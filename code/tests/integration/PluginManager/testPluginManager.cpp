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
 * Contains unit tests for PluginManager class
 */

// C++ Plugin Framework includes
#include <CppPluginFramework/PluginManager.hpp>
#include <CppPluginFramework/PluginManagerConfig.hpp>
#include "../TestPlugins/ITestPlugin1.hpp"
#include "../TestPlugins/ITestPlugin2.hpp"

// C++ Config Framework includes
#include <CppConfigFramework/ConfigReader.hpp>

// Qt includes
#include <QtCore/QDebug>
#include <QtTest/QTest>

// System includes

// Forward declarations

// Macros

// Test class declaration --------------------------------------------------------------------------

using namespace CppConfigFramework;
using namespace CppPluginFramework;

class TestPluginManager : public QObject
{
    Q_OBJECT

private slots:
    // Functions executed by QtTest before and after test suite
    void initTestCase();
    void cleanupTestCase();

    // Functions executed by QtTest before and after each test
    void init();
    void cleanup();

    // Test functions
    void testLoad();
    void testLoadAfterStart();
    void testLoadPluginsWithInvalidConfig();
    void testLoadPluginsWithUnsupportedDependency();
    void testLoadPluginsWithInvalidStartupOrder();
};

// Test Case init/cleanup methods ------------------------------------------------------------------

void TestPluginManager::initTestCase()
{
}

void TestPluginManager::cleanupTestCase()
{
}

// Test init/cleanup methods -----------------------------------------------------------------------

void TestPluginManager::init()
{
}

void TestPluginManager::cleanup()
{
}

// Test: loading of plugins ------------------------------------------------------------------------

void TestPluginManager::testLoad()
{
    // First load the config
    ConfigReader configReader;
    EnvironmentVariables environmentVariables;

    auto config = configReader.read(":/TestData/AppConfig.json",
                                    QDir(QCoreApplication::applicationDirPath()),
                                    ConfigNodePath::ROOT_PATH,
                                    ConfigNodePath::ROOT_PATH,
                                    {},
                                    &environmentVariables);
    QVERIFY(config);

    PluginManagerConfig pluginManagerConfig;
    QVERIFY(pluginManagerConfig.loadConfig(*config));

    // Load plugins
    PluginManager pluginManager;
    QVERIFY(pluginManager.load(pluginManagerConfig));

    // Check all instances
    const QStringList instanceNames = pluginManager.pluginInstanceNames();
    QCOMPARE(instanceNames.count(), 3);

    QVERIFY(instanceNames.contains("instance1"));
    QVERIFY(instanceNames.contains("instance2"));
    QVERIFY(instanceNames.contains("instance3"));

    QVERIFY(pluginManager.hasPluginInstance("instance1"));
    QVERIFY(pluginManager.hasPluginInstance("instance2"));
    QVERIFY(pluginManager.hasPluginInstance("instance3"));

    auto instance1 = pluginManager.pluginInstance("instance1");
    auto instance2 = pluginManager.pluginInstance("instance2");
    auto instance3 = pluginManager.pluginInstance("instance3");

    QVERIFY(instance1 != nullptr);
    QVERIFY(instance2 != nullptr);
    QVERIFY(instance3 != nullptr);

    // Start plugins
    QVERIFY(pluginManager.start());

    // Check if all instances return expected values
    QCOMPARE(instance1->interface<TestPlugins::ITestPlugin1>()->value(), QStringLiteral("value1"));
    QCOMPARE(instance2->interface<TestPlugins::ITestPlugin1>()->value(), QStringLiteral("value2"));
    QCOMPARE(instance3->interface<TestPlugins::ITestPlugin2>()->joinedValues(),
             QStringLiteral("value1;value2"));

    // Stop plugins
    pluginManager.stop();

    // Unload plugins
    QVERIFY(pluginManager.unload());
}

// Test: loading of plugins after the plugins were already started ---------------------------------

void TestPluginManager::testLoadAfterStart()
{
    // First load the config
    ConfigReader configReader;
    EnvironmentVariables environmentVariables;

    auto config = configReader.read(":/TestData/AppConfig.json",
                                    QDir(QCoreApplication::applicationDirPath()),
                                    ConfigNodePath::ROOT_PATH,
                                    ConfigNodePath::ROOT_PATH,
                                    std::vector<const ConfigObjectNode *>(),
                                    &environmentVariables);
    QVERIFY(config);

    PluginManagerConfig pluginManagerConfig;
    QVERIFY(pluginManagerConfig.loadConfig(*config));

    // Load plugins
    PluginManager pluginManager;
    QVERIFY(pluginManager.load(pluginManagerConfig));

    // Start plugins
    QVERIFY(pluginManager.start());

    // Load plugins again (must fail)
    QVERIFY(!pluginManager.load(pluginManagerConfig));

    // Stop plugins
    pluginManager.stop();

    // Unload plugins
    QVERIFY(pluginManager.unload());
}

// Test: loading of plugins with invalid config ----------------------------------------------------

void TestPluginManager::testLoadPluginsWithInvalidConfig()
{
    PluginManagerConfig pluginManagerConfig;
    pluginManagerConfig.setPluginConfigs({ PluginConfig() });

    // Load plugins with invalid config
    PluginManager pluginManager;
    QVERIFY(!pluginManager.load(pluginManagerConfig));
}

// Test: loading of plugins with an unsupported dependency -----------------------------------------

void TestPluginManager::testLoadPluginsWithUnsupportedDependency()
{
    // First load the config
    ConfigReader configReader;
    EnvironmentVariables environmentVariables;

    auto config = configReader.read(":/TestData/InvalidConfigWithUnsupportedDependency.json",
                                    QDir(QCoreApplication::applicationDirPath()),
                                    ConfigNodePath::ROOT_PATH,
                                    ConfigNodePath::ROOT_PATH,
                                    std::vector<const ConfigObjectNode *>(),
                                    &environmentVariables);
    QVERIFY(config);

    PluginManagerConfig pluginManagerConfig;
    QVERIFY(pluginManagerConfig.loadConfig(*config));

    PluginManager pluginManager;
    QVERIFY(!pluginManager.load(pluginManagerConfig));
}

// Test: loading of plugins with invalid startup order ---------------------------------------------

void TestPluginManager::testLoadPluginsWithInvalidStartupOrder()
{
    // First load the config
    ConfigReader configReader;
    EnvironmentVariables environmentVariables;

    auto config = configReader.read(":/TestData/AppConfigWithInvalidStartupOrder.json",
                                    QDir(QCoreApplication::applicationDirPath()),
                                    ConfigNodePath::ROOT_PATH,
                                    ConfigNodePath::ROOT_PATH,
                                    std::vector<const ConfigObjectNode *>(),
                                    &environmentVariables);
    QVERIFY(config);

    PluginManagerConfig pluginManagerConfig;
    QVERIFY(pluginManagerConfig.loadConfig(*config));

    // Load plugins
    PluginManager pluginManager;
    QVERIFY(pluginManager.load(pluginManagerConfig));

    // Start plugins (must fail)
    QVERIFY(!pluginManager.start());
}

// Main function -----------------------------------------------------------------------------------

QTEST_MAIN(TestPluginManager)
#include "testPluginManager.moc"
