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
#include <CppPluginFramework/IPluginFactory.hpp>
#include <CppPluginFramework/Plugin.hpp>

// C++ Config Framework includes
#include <CppConfigFramework/ConfigValueNode.hpp>

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
    QFETCH(QStringList, instanceNames);
    QFETCH(VersionInfo, version);
    QFETCH(QString, description);
    QFETCH(QStringList, exportedInterfaces);
    QFETCH(bool, result);

    // Load plugin
    auto instances = Plugin::loadInstances(config);
    QCOMPARE(instances.size(), static_cast<size_t>(instanceNames.size()));

    // Check plugin
    if (result)
    {
        QStringList checkedInstances;

        for (const auto &instance : instances)
        {
            QVERIFY(instance);

            QVERIFY(instanceNames.contains(instance->name()));
            QVERIFY(!checkedInstances.contains(instance->name()));
            checkedInstances.append(instance->name());

            QCOMPARE(instance->version(), version);
            QCOMPARE(instance->description(), description);
            QCOMPARE(instance->exportedInterfaces(), QSet<QString>(exportedInterfaces.begin(),
                                                                   exportedInterfaces.end()));
        }

        QCOMPARE(checkedInstances.size(), instanceNames.size());
        QCOMPARE(QSet<QString>(checkedInstances.begin(), checkedInstances.end()),
                 QSet<QString>(instanceNames.begin(), instanceNames.end()));
    }
}

void TestPlugin::testLoadPlugin_data()
{
    QTest::addColumn<PluginConfig>("config");
    QTest::addColumn<QStringList>("instanceNames");
    QTest::addColumn<VersionInfo>("version");
    QTest::addColumn<QString>("description");
    QTest::addColumn<QStringList>("exportedInterfaces");
    QTest::addColumn<bool>("result");

    QDir testPluginsDir(QCoreApplication::applicationDirPath());
    testPluginsDir.cd("../TestPlugins");

    // Loading of test plugin 1
    {
        QJsonObject instance1Config
        {
            { "value", "value1" }
        };

        QJsonObject instance2Config
        {
            { "value", "value2" }
        };

        QList<PluginInstanceConfig> instanceConfigs;
        instanceConfigs << PluginInstanceConfig("instance1", instance1Config)
                        << PluginInstanceConfig("instance2", instance2Config);

        PluginConfig config(testPluginsDir.filePath("TestPlugin1.plugin"),
                            VersionInfo(1, 0, 0),
                            instanceConfigs);

        QTest::newRow("valid: test plugin 1")
                << config
                << QStringList { "instance1", "instance2" }
                << VersionInfo(1, 0, 0)
                << QString("test plugin 1")
                << QStringList { "CppPluginFramework::TestPlugins::ITestPlugin1" }
                << true;
    }

    // Loading of test plugin 2
    {
        QJsonObject instance3Config
        {
            { "delimiter", ";" }
        };

        QList<PluginInstanceConfig> instanceConfigs;
        instanceConfigs << PluginInstanceConfig("instance3",
                                                instance3Config,
                                                QSet<QString> { "instance1", "instance2"});

        PluginConfig config(testPluginsDir.filePath("TestPlugin2.plugin"),
                            VersionInfo(1, 0, 0),
                            VersionInfo(1, 0, 1),
                            instanceConfigs);

        QTest::newRow("valid: test plugin 2")
                << config
                << QStringList { "instance3" }
                << VersionInfo(1, 0, 0)
                << QString("test plugin 2")
                << QStringList { "CppPluginFramework::TestPlugins::ITestPlugin2" }
                << true;
    }

    // Loading of plugin with invalid config
    {
        QJsonObject instance1Config
        {
            { "invalid", "x" }
        };

        QList<PluginInstanceConfig> instanceConfigs;
        instanceConfigs << PluginInstanceConfig("instance1", instance1Config);

        PluginConfig config(testPluginsDir.filePath("TestPlugin1.plugin"),
                            VersionInfo(1, 0, 0),
                            instanceConfigs);

        QTest::newRow("invalid: config") << config
                                         << QStringList()
                                         << VersionInfo(1, 0, 0)
                                         << QString()
                                         << QStringList()
                                         << false;
    }

    // Loading of plugin with invalid plugin config
    {
        QTest::newRow("invalid: plugin config") << PluginConfig()
                << QStringList()
                << VersionInfo()
                << QString()
                << QStringList()
                << false;
    }

    // Loading of plugin with invalid version
    {
        QJsonObject instance1Config
        {
            { "value", "value1" }
        };

        QList<PluginInstanceConfig> instanceConfigs;
        instanceConfigs << PluginInstanceConfig("instance1", instance1Config);

        PluginConfig config(testPluginsDir.filePath("TestPlugin1.plugin"),
                            VersionInfo(1, 0, 1),
                            instanceConfigs);

        QTest::newRow("invalid: version 1") << config
                                            << QStringList()
                                            << VersionInfo()
                                            << QString()
                                            << QStringList()
                                            << false;

        config = PluginConfig(testPluginsDir.filePath("TestPlugin1.plugin"),
                              VersionInfo(1, 0, 1),
                              VersionInfo(1, 0, 2),
                              instanceConfigs);

        QTest::newRow("invalid: version 2") << config
                                            << QStringList()
                                            << VersionInfo()
                                            << QString()
                                            << QStringList()
                                            << false;
    }
}

// Main function -----------------------------------------------------------------------------------

QTEST_MAIN(TestPlugin)
#include "testPlugin.moc"
