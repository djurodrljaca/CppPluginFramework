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
 * Contains unit tests for Plugin class
 */

// C++ Plugin Framework includes
#include <CppPluginFramework/Plugin.hpp>

// Qt includes
#include <QtCore/QDebug>
#include <QtTest/QTest>

// System includes

// Forward declarations

// Macros

// Test class declaration --------------------------------------------------------------------------

using namespace CppConfigFramework;
using namespace CppPluginFramework;

Q_DECLARE_METATYPE(PluginConfig)
Q_DECLARE_METATYPE(VersionInfo)

class TestPlugin : public QObject
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
    void testLoadPlugin();
    void testLoadPlugin_data();
};

// Test Case init/cleanup methods ------------------------------------------------------------------

void TestPlugin::initTestCase()
{
}

void TestPlugin::cleanupTestCase()
{
}

// Test init/cleanup methods -----------------------------------------------------------------------

void TestPlugin::init()
{
}

void TestPlugin::cleanup()
{
}

// Test: loading of plugins ------------------------------------------------------------------------

void TestPlugin::testLoadPlugin()
{
    QFETCH(PluginConfig, config);
    QFETCH(VersionInfo, version);
    QFETCH(int, instanceCount);
    QFETCH(bool, result);

    // Load plugin
    std::unique_ptr<Plugin> plugin = Plugin::load(config);
    QCOMPARE(plugin != nullptr, result);

    // Check plugin
    if (result)
    {
        QVERIFY(plugin->isLoaded());
        QVERIFY(plugin->isValid());
        QCOMPARE(plugin->version(), version);
        QCOMPARE(plugin->instances().count(), instanceCount);
        plugin->unload();
    }
}

void TestPlugin::testLoadPlugin_data()
{
    QTest::addColumn<PluginConfig>("config");
    QTest::addColumn<VersionInfo>("version");
    QTest::addColumn<int>("instanceCount");
    QTest::addColumn<bool>("result");

    QDir testPluginsDir(QCoreApplication::applicationDirPath());
    testPluginsDir.cd("../TestPlugins");

    // Loading of test plugin 1
    {
        ConfigObjectNode instance1Config;
        instance1Config.setMember("value", ConfigValueNode("value1"));

        ConfigObjectNode instance2Config;
        instance2Config.setMember("value", ConfigValueNode("value2"));

        QList<PluginInstanceConfig> instanceConfigs;
        instanceConfigs << PluginInstanceConfig("instance1", instance1Config)
                       << PluginInstanceConfig("instance2", instance2Config);

        PluginConfig config(testPluginsDir.filePath("libTestPlugin1.so"),
                            VersionInfo(1, 0, 0),
                            instanceConfigs);

        QTest::newRow("valid: test plugin 1") << config << VersionInfo(1, 0, 0) << 2 << true;
    }

    // Loading of test plugin 2
    {
        ConfigObjectNode instance3Config;
        instance3Config.setMember("delimiter", ConfigValueNode(";"));

        QList<PluginInstanceConfig> instanceConfigs;
        instanceConfigs << PluginInstanceConfig("instance3",
                                               instance3Config,
                                               QSet<QString> { "instance1", "instance2"});

        PluginConfig config(testPluginsDir.filePath("libTestPlugin2.so"),
                            VersionInfo(1, 0, 0),
                            VersionInfo(1, 0, 1),
                            instanceConfigs);

        QTest::newRow("valid: test plugin 2") << config << VersionInfo(1, 0, 0) << 1 << true;
    }

    // Loading of plugin with invalid config
    {
        ConfigObjectNode instance1Config;
        instance1Config.setMember("invalid", ConfigValueNode("x"));

        QList<PluginInstanceConfig> instanceConfigs;
        instanceConfigs << PluginInstanceConfig("instance1", instance1Config);

        PluginConfig config(testPluginsDir.filePath("libTestPlugin1.so"),
                            VersionInfo(1, 0, 0),
                            instanceConfigs);

        QTest::newRow("invalid: config") << config << VersionInfo(1, 0, 0) << 1 << false;
    }

    // Loading of plugin with invalid version
    {
        ConfigObjectNode instance1Config;
        instance1Config.setMember("value", ConfigValueNode("value1"));

        QList<PluginInstanceConfig> instanceConfigs;
        instanceConfigs << PluginInstanceConfig("instance1", instance1Config);

        PluginConfig config(testPluginsDir.filePath("libTestPlugin1.so"),
                            VersionInfo(1, 0, 1),
                            instanceConfigs);

        QTest::newRow("invalid: version") << config << VersionInfo(1, 0, 0) << 1 << false;
    }
}

// Main function -----------------------------------------------------------------------------------

QTEST_MAIN(TestPlugin)
#include "testPlugin.moc"
