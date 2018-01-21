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
#include <CppPluginFramework/ConfigFile.hpp>
#include <CppPluginFramework/PluginManager.hpp>
#include "../TestPlugins/ITestPlugin1.hpp"
#include "../TestPlugins/ITestPlugin2.hpp"

// Qt includes
#include <QtCore/QDebug>
#include <QtTest/QTest>

// System includes

// Forward declarations

// Macros

// Test class declaration --------------------------------------------------------------------------

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
    void testLoadPlugins();

    void testLoadPluginsWithInvalidConfig();
    void testLoadPluginsWithInvalidConfig_data();

private:
    // Holds the path to the data directory
    QDir m_testDataDirPath;
};

// Test Case init/cleanup methods ------------------------------------------------------------------

void TestPluginManager::initTestCase()
{
    m_testDataDirPath = QDir(TEST_DATA_DIR_PATH);
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

void TestPluginManager::testLoadPlugins()
{
    // First load the config
    ConfigFile configFile;
    QVERIFY(configFile.read(m_testDataDirPath.absoluteFilePath("AppConfig.json")));

    // Load plugins
    PluginManager pluginManager;
    QVERIFY(pluginManager.loadPlugins(configFile.pluginConfigs()));

    // Check all instances
    const QStringList instanceNames = pluginManager.pluginInstanceNames();
    QCOMPARE(instanceNames.count(), 3);

    QVERIFY(instanceNames.contains("instance1"));
    QVERIFY(instanceNames.contains("instance2"));
    QVERIFY(instanceNames.contains("instance3"));

    auto instance1 = pluginManager.pluginInstance("instance1");
    auto instance2 = pluginManager.pluginInstance("instance2");
    auto instance3 = pluginManager.pluginInstance("instance3");

    QVERIFY(instance1 != nullptr);
    QVERIFY(instance2 != nullptr);
    QVERIFY(instance3 != nullptr);

    // Start plugins
    QVERIFY(pluginManager.startPlugins());

    // Check if all instances return expected values
    QCOMPARE(instance1->interface<TestPlugins::ITestPlugin1>()->value(), "value1");
    QCOMPARE(instance2->interface<TestPlugins::ITestPlugin1>()->value(), "value2");
    QCOMPARE(instance3->interface<TestPlugins::ITestPlugin2>()->joinedValues(), "value1;value2");

    // Stop plugins
    pluginManager.stopPlugins();

    // Unload plugins
    pluginManager.unloadPlugins();
}

// Test: loading of plugins with invalid config ----------------------------------------------------

void TestPluginManager::testLoadPluginsWithInvalidConfig()
{
    QFETCH(QString, fileName);

    // First load the config
    ConfigFile configFile;
    QVERIFY(configFile.read(m_testDataDirPath.absoluteFilePath(fileName)));

    // Load plugins with invalid config
    PluginManager pluginManager;
    QVERIFY(!pluginManager.loadPlugins(configFile.pluginConfigs()));
}

void TestPluginManager::testLoadPluginsWithInvalidConfig_data()
{
    QTest::addColumn<QString>("fileName");

    QTest::newRow("duplicate instance names") << "InvalidAppConfig1.json";
    QTest::newRow("unknown dependency") << "InvalidAppConfig2.json";
    QTest::newRow("unsupported dependency") << "InvalidAppConfig3.json";
}

// Main function -----------------------------------------------------------------------------------

QTEST_MAIN(TestPluginManager)
#include "testPluginManager.moc"
