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
 * Contains unit tests for ConfigFile class
 */

// C++ Plugin Framework includes
#include <CppPluginFramework/ConfigFile.hpp>

// Qt includes
#include <QtCore/QDebug>
#include <QtTest/QTest>

// System includes

// Forward declarations

// Macros

// Test class declaration --------------------------------------------------------------------------

using namespace CppPluginFramework;

class TestConfigFile : public QObject
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
    void testReadValidConfig();

    void testReadInvalidConfig();
    void testReadInvalidConfig_data();


private:
    // Holds the path to the data directory
    QDir m_testDataDirPath;
};

// Test Case init/cleanup methods ------------------------------------------------------------------

void TestConfigFile::initTestCase()
{
    m_testDataDirPath = QDir(TEST_DATA_DIR_PATH);
}

void TestConfigFile::cleanupTestCase()
{
}

// Test init/cleanup methods -----------------------------------------------------------------------

void TestConfigFile::init()
{
}

void TestConfigFile::cleanup()
{
}

// Test: read a valid config file ------------------------------------------------------------------

void TestConfigFile::testReadValidConfig()
{
    // Read config file
    const QString configFilePath(m_testDataDirPath.absoluteFilePath("ValidAppConfig.json"));
    ConfigFile configFile;
    QVERIFY(configFile.read(configFilePath, m_testDataDirPath.absolutePath()));

    // Check custom environment variables
    const EnvironmentVariables variables = configFile.environmentVariables();

    QCOMPARE(variables.value("var1"), "value1");
    QCOMPARE(variables.expandText(variables.value("var2")), "value1_var2");
    QCOMPARE(variables.value("PluginPath"), "plugins");

    // Check plugin configs
    const auto pluginConfigs = configFile.pluginConfigs();

    QCOMPARE(pluginConfigs.size(), 2);

    {
        const auto pluginConfig = pluginConfigs[0];

        QCOMPARE(pluginConfig.filePath(),
                 m_testDataDirPath.absoluteFilePath("plugins/dummyPlugin1"));
        QVERIFY(pluginConfig.isExactVersion());
        QCOMPARE(pluginConfig.version(), VersionInfo(1, 0, 0));
        QVERIFY(pluginConfig.minVersion().isNull());
        QVERIFY(pluginConfig.maxVersion().isNull());

        const auto instanceConfigs = pluginConfig.instanceConfigs();

        QCOMPARE(instanceConfigs.size(), 2);
        QCOMPARE(instanceConfigs[0], PluginInstanceConfig("instance1",
                                                          QJsonObject {
                                                              { "param1", "value1" },
                                                              { "param2", "value2" }
                                                          },
                                                          QSet<QString> {
                                                              "instance2",
                                                              "instance3"
                                                          } ));

        QCOMPARE(instanceConfigs[1], PluginInstanceConfig("instance2",
                                                          QJsonObject(),
                                                          QSet<QString>()));
    }

    {
        const auto pluginConfig = pluginConfigs[1];

        QCOMPARE(pluginConfig.filePath(),
                 m_testDataDirPath.absoluteFilePath("plugins/dummyPlugin2"));
        QVERIFY(pluginConfig.isVersionRange());
        QVERIFY(pluginConfig.version().isNull());
        QCOMPARE(pluginConfig.minVersion(), VersionInfo(1, 0, 0));
        QCOMPARE(pluginConfig.maxVersion(), VersionInfo(2, 0, 0));

        const auto instanceConfigs = pluginConfig.instanceConfigs();

        QCOMPARE(instanceConfigs.size(), 1);
        QCOMPARE(instanceConfigs[0], PluginInstanceConfig("instance3",
                                                          QJsonObject { { "param", "xyz" } },
                                                          QSet<QString>()));
    }
}

// Test: read a valid config file ------------------------------------------------------------------

void TestConfigFile::testReadInvalidConfig()
{
    QFETCH(QString, fileName);

    const QString configFilePath(m_testDataDirPath.absoluteFilePath(fileName));
    QVERIFY(QFile::exists(configFilePath));

    ConfigFile configFile;
    QVERIFY(!configFile.read(configFilePath, m_testDataDirPath.absolutePath()));
}

void TestConfigFile::testReadInvalidConfig_data()
{
    QTest::addColumn<QString>("fileName");

    // Environment variables
    QTest::newRow("invalid env. var. format") << "InvalidAppConfig_EnvVarFormat.json";
    QTest::newRow("non-string env. var. value") << "InvalidAppConfig_EnvVarNonString.json";
    QTest::newRow("invalid env. var. name") << "InvalidAppConfig_EnvVarName.json";

    // Plugins
    QTest::newRow("missing plugins") << "InvalidAppConfig_PluginsMissing.json";
    QTest::newRow("invalid plugins format") << "InvalidAppConfig_PluginsFormat.json";
    QTest::newRow("plugins empty") << "InvalidAppConfig_PluginsEmpty.json";

    QTest::newRow("invalid plugin format") << "InvalidAppConfig_PluginFormat.json";
    QTest::newRow("missing plugin file path") << "InvalidAppConfig_PluginFilePathMissing.json";
    QTest::newRow("invalid plugin file path format")
            << "InvalidAppConfig_PluginFilePathFormat.json";
    QTest::newRow("invalid plugin file path") << "InvalidAppConfig_PluginFilePath.json";

    QTest::newRow("missing version") << "InvalidAppConfig_PluginVersionMissing.json";
    QTest::newRow("missing min version") << "InvalidAppConfig_PluginMinVersionMissing.json";
    QTest::newRow("missing max version") << "InvalidAppConfig_PluginMaxVersionMissing.json";
    QTest::newRow("invalid version format") << "InvalidAppConfig_PluginVersionFormat.json";
    QTest::newRow("invalid version") << "InvalidAppConfig_PluginVersion.json";
    QTest::newRow("invalid min version format") << "InvalidAppConfig_PluginMinVersionFormat.json";
    QTest::newRow("invalid max version format") << "InvalidAppConfig_PluginMaxVersionFormat.json";
    QTest::newRow("invalid min version") << "InvalidAppConfig_PluginMinVersion.json";
    QTest::newRow("invalid max version") << "InvalidAppConfig_PluginMaxVersion.json";

    QTest::newRow("missing instances") << "InvalidAppConfig_PluginInstancesMissing.json";
    QTest::newRow("invalid instances format") << "InvalidAppConfig_PluginInstancesFormat.json";
    QTest::newRow("instances empty") << "InvalidAppConfig_PluginInstancesEmpty.json";
    QTest::newRow("invalid instance format") << "InvalidAppConfig_PluginInstanceFormat.json";
    QTest::newRow("missing instance name") << "InvalidAppConfig_PluginInstanceNameMissing.json";
    QTest::newRow("invalid instance name format")
            << "InvalidAppConfig_PluginInstanceNameFormat.json";
    QTest::newRow("invalid instance name") << "InvalidAppConfig_PluginInstanceName.json";
    QTest::newRow("missing instance config file")
            << "InvalidAppConfig_PluginInstanceConfigFileMissing.json";
    QTest::newRow("invalid instance config file")
            << "InvalidAppConfig_PluginInstanceConfigFile.json";
    QTest::newRow("invalid instance config") << "InvalidAppConfig_PluginInstanceConfig.json";
    QTest::newRow("instance with both config and config file path")
            << "InvalidAppConfig_PluginInstanceConfigAndConfigFile.json";
    QTest::newRow("invalid dependencies format")
            << "InvalidAppConfig_PluginInstanceDependenciesFormat.json";
    QTest::newRow("invalid dependency format")
            << "InvalidAppConfig_PluginInstanceDependencyFormat.json";
    QTest::newRow("missing dependency name")
            << "InvalidAppConfig_PluginInstanceDependencyNameMissing.json";
    QTest::newRow("invalid dependency name")
            << "InvalidAppConfig_PluginInstanceDependencyName.json";
    QTest::newRow("duplicate dependency name")
            << "InvalidAppConfig_PluginInstanceDependencyNameDuplicate.json";
}

// Main function -----------------------------------------------------------------------------------

QTEST_MAIN(TestConfigFile)
#include "testConfigFile.moc"
