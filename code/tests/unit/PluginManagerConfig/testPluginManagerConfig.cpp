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
 * Contains unit tests for PluginManagerConfig class
 */

// C++ Plugin Framework includes
#include <CppPluginFramework/PluginManagerConfig.hpp>

// Qt includes
#include <QtCore/QDebug>
#include <QtTest/QTest>

// System includes

// Forward declarations

// Macros

// Test class declaration --------------------------------------------------------------------------

using namespace CppConfigFramework;
using namespace CppPluginFramework;

using ConfigObjectNodePtr = std::shared_ptr<ConfigObjectNode>;

Q_DECLARE_METATYPE(ConfigObjectNodePtr)
Q_DECLARE_METATYPE(PluginManagerConfig)

class TestPluginManagerConfig : public QObject
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
    void testIsValid();
    void testIsValid_data();

    void testLoadConfig();
    void testLoadConfig_data();
};

// Test Case init/cleanup methods ------------------------------------------------------------------

void TestPluginManagerConfig::initTestCase()
{
}

void TestPluginManagerConfig::cleanupTestCase()
{
}

// Test init/cleanup methods -----------------------------------------------------------------------

void TestPluginManagerConfig::init()
{
}

void TestPluginManagerConfig::cleanup()
{
}

// Test: validity ----------------------------------------------------------------------------------

void TestPluginManagerConfig::testIsValid()
{
    QFETCH(PluginManagerConfig, config);
    QFETCH(bool, result);

    QCOMPARE(config.isValid(), result);
}

void TestPluginManagerConfig::testIsValid_data()
{
    QTest::addColumn<PluginManagerConfig>("config");
    QTest::addColumn<bool>("result");

    // Prepare common data
    const QString validFilePath1(":/TestData/dummyPlugin1");
    const QString validFilePath2(":/TestData/dummyPlugin2");
    const VersionInfo validVersion(1, 0, 0);
    const QList<PluginInstanceConfig> validInstanceConfigs1 { PluginInstanceConfig("instance1") };
    const QList<PluginInstanceConfig> validInstanceConfigs2 { PluginInstanceConfig("instance2") };
    const QList<PluginConfig> validPluginConfigs
    {
        PluginConfig(validFilePath1, validVersion, validInstanceConfigs1)
    };

    // Valid
    {
        PluginManagerConfig managerConfig;
        managerConfig.setPluginConfigs(validPluginConfigs);

        QTest::newRow("valid: empty") << PluginManagerConfig() << true;
        QTest::newRow("valid: nmon-empty") << managerConfig << true;
    }

    // Invalid: plugin config
    {
        PluginManagerConfig managerConfig;
        managerConfig.setPluginConfigs(
                    QList<PluginConfig> {
                        PluginConfig(QString(), validVersion, validInstanceConfigs1)
                    });

        QTest::newRow("invalid: plugin config") << managerConfig << false;
    }

    // Invalid: duplicated plugins
    {
        const QList<PluginConfig> pluginConfigs
        {
            PluginConfig(validFilePath1, validVersion, validInstanceConfigs1),
            PluginConfig(validFilePath1, validVersion, validInstanceConfigs2)
        };

        PluginManagerConfig managerConfig;
        managerConfig.setPluginConfigs(pluginConfigs);

        QTest::newRow("invalid: duplicated plugins") << managerConfig << false;
    }

    // Invalid: duplicated instance name
    {
        const QList<PluginConfig> pluginConfigs
        {
            PluginConfig(validFilePath1, validVersion, validInstanceConfigs1),
            PluginConfig(validFilePath2, validVersion, validInstanceConfigs1)
        };

        PluginManagerConfig managerConfig;
        managerConfig.setPluginConfigs(pluginConfigs);

        QTest::newRow("invalid: duplicated instance name") << managerConfig << false;
    }
}

// Test: loadConfig() method -----------------------------------------------------------------------

void TestPluginManagerConfig::testLoadConfig()
{
    QFETCH(ConfigObjectNodePtr, configNode);
    QFETCH(PluginManagerConfig, expectedPluginManagerConfig);
    QFETCH(bool, expectedResult);

    QString error;
    PluginManagerConfig managerConfig;
    const bool result = managerConfig.loadConfigAtPath(ConfigNodePath::ROOT_PATH,
                                                       *configNode,
                                                       &error);
    qDebug() << "TestPluginManagerConfig::testLoadConfig: error:" << error;

    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(managerConfig, expectedPluginManagerConfig);
    }
}

void TestPluginManagerConfig::testLoadConfig_data()
{
    QTest::addColumn<ConfigObjectNodePtr>("configNode");
    QTest::addColumn<PluginManagerConfig>("expectedPluginManagerConfig");
    QTest::addColumn<bool>("expectedResult");

    // Prepare common data
    const QString validFilePath(QCoreApplication::applicationFilePath());
    const VersionInfo validVersion(1, 0, 0);
    const QList<PluginInstanceConfig> validInstanceConfigs { PluginInstanceConfig("instance") };
    const QList<PluginConfig> validPluginConfigs
    {
        PluginConfig(validFilePath, validVersion, validInstanceConfigs)
    };

    ConfigObjectNode instance;
    instance.setMember("name", ConfigValueNode("instance"));

    ConfigObjectNode instances;
    instances.setMember("instance", instance);

    ConfigObjectNode plugin;
    plugin.setMember("file_path", ConfigValueNode(validFilePath));
    plugin.setMember("version", ConfigValueNode(validVersion.toString()));
    plugin.setMember("instances", instances);

    // Valid: empty
    {
        auto configNode = std::make_shared<ConfigObjectNode>();
        configNode->setMember("plugins", ConfigObjectNode());

        QTest::newRow("valid: empty") << configNode << PluginManagerConfig() << true;
    }

    // Valid: non-empty
    {
        ConfigObjectNode plugins;
        plugins.setMember("plugin1", plugin);

        auto configNode = std::make_shared<ConfigObjectNode>();
        configNode->setMember("plugins", plugins);

        PluginManagerConfig managerConfig;
        managerConfig.setPluginConfigs(validPluginConfigs);

        QTest::newRow("valid: non-empty") << configNode << managerConfig << true;
    }

    // Invalid
    {
        auto configNode = std::make_shared<ConfigObjectNode>();

        QTest::newRow("invalid") << configNode << PluginManagerConfig() << false;
    }
}

// Main function -----------------------------------------------------------------------------------

QTEST_MAIN(TestPluginManagerConfig)
#include "testPluginManagerConfig.moc"
