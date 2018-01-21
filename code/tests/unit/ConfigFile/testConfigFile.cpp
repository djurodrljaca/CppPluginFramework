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

    // TODO: add "negative" tests!

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
    const QString configFilePath(m_testDataDirPath.absoluteFilePath("AppConfig.json"));
    ConfigFile configFile;
    QVERIFY(configFile.read(configFilePath, m_testDataDirPath.absolutePath()));

    // Check custom environment variables
    const auto variables = configFile.customEnvironmentVariables();

    QCOMPARE(variables.size(), 3);

    QCOMPARE(variables["var1"], "value1");
    QCOMPARE(variables["var2"], "value1_var2");
    QCOMPARE(variables["PluginPath"], "plugins");

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

// Main function -----------------------------------------------------------------------------------

QTEST_MAIN(TestConfigFile)
#include "testConfigFile.moc"
