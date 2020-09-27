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
 * Contains unit tests for PluginConfig class
 */

// C++ Plugin Framework includes
#include <CppPluginFramework/PluginConfig.hpp>

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

using ConfigObjectNodePtr = std::shared_ptr<ConfigObjectNode>;

Q_DECLARE_METATYPE(ConfigObjectNodePtr)
Q_DECLARE_METATYPE(PluginConfig)

class TestPluginConfig : public QObject
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

    void testFilePath();
    void testVersion();
    void testInstanceConfigs();

    void testLoadConfig();
    void testLoadConfig_data();
};

// Test Case init/cleanup methods ------------------------------------------------------------------

void TestPluginConfig::initTestCase()
{
}

void TestPluginConfig::cleanupTestCase()
{
}

// Test init/cleanup methods -----------------------------------------------------------------------

void TestPluginConfig::init()
{
}

void TestPluginConfig::cleanup()
{
}

// Test: validity ----------------------------------------------------------------------------------

void TestPluginConfig::testIsValid()
{
    QFETCH(PluginConfig, config);
    QFETCH(bool, result);

    QCOMPARE(config.isValid(), result);
}

void TestPluginConfig::testIsValid_data()
{
    QTest::addColumn<PluginConfig>("config");
    QTest::addColumn<bool>("result");

    // Prepare common data
    const QString validFilePath(QCoreApplication::applicationFilePath());

    const VersionInfo validVersion1(1, 0, 0);
    const VersionInfo validVersion2(2, 0, 0);

    const QList<PluginInstanceConfig> validInstanceConfigs
    {
        PluginInstanceConfig("instance1"), PluginInstanceConfig("instance2")
    };

    // Valid results
    QTest::newRow("valid: exact version") << PluginConfig(validFilePath,
                                                          validVersion1,
                                                          validInstanceConfigs) << true;

    QTest::newRow("valid: version range") << PluginConfig(validFilePath,
                                                          validVersion1,
                                                          validVersion2,
                                                          validInstanceConfigs) << true;

    // Invalid results
    QTest::newRow("invalid: default constructed") << PluginConfig() << false;

    QTest::newRow("invalid: file path")
            << PluginConfig(QString(), validVersion1, validInstanceConfigs) << false;

    QTest::newRow("invalid: exact version 1")
            << PluginConfig(validFilePath, VersionInfo(), validInstanceConfigs) << false;

    QTest::newRow("invalid: exact version 2")
            << PluginConfig(validFilePath, VersionInfo(-1, 0, 0), validInstanceConfigs) << false;

    QTest::newRow("invalid: min version 1")
            << PluginConfig(validFilePath, VersionInfo(), validVersion2, validInstanceConfigs)
            << false;

    QTest::newRow("invalid: min version 2")
            << PluginConfig(validFilePath,
                            VersionInfo(-1, 0, 0),
                            validVersion2,
                            validInstanceConfigs)
            << false;

    QTest::newRow("invalid: max version 1")
            << PluginConfig(validFilePath, validVersion1, VersionInfo(), validInstanceConfigs)
            << false;

    QTest::newRow("invalid: max version 2")
            << PluginConfig(validFilePath,
                            validVersion1,
                            VersionInfo(-1, 0, 0),
                            validInstanceConfigs)
            << false;

    QTest::newRow("invalid: version range")
            << PluginConfig(validFilePath, validVersion2, validVersion1, validInstanceConfigs)
            << false;

    QTest::newRow("invalid: no instance configs")
            << PluginConfig(validFilePath, validVersion1, QList<PluginInstanceConfig>())
            << false;

    QTest::newRow("invalid: invalid instance config")
            << PluginConfig(validFilePath, validVersion1, QList<PluginInstanceConfig> {
                                PluginInstanceConfig("instance1"),
                                PluginInstanceConfig()
                            }) << false;

    QTest::newRow("invalid: duplicate instance name")
            << PluginConfig(validFilePath, validVersion1, QList<PluginInstanceConfig> {
                                PluginInstanceConfig("instance1"),
                                PluginInstanceConfig("instance1")
                            }) << false;
}

// Test: file path ---------------------------------------------------------------------------------

void TestPluginConfig::testFilePath()
{
    // Default constructor
    {
        PluginConfig config;
        QCOMPARE(config.filePath(), QString());

        config.setFilePath("file.so");
        QCOMPARE(config.filePath(), QString("file.so"));
    }

    // Constructor
    {
        PluginConfig config("file1.so", VersionInfo(), {});
        QCOMPARE(config.filePath(), QString("file1.so"));

        config = PluginConfig("file2.so", VersionInfo(), VersionInfo(), {});
        QCOMPARE(config.filePath(), QString("file2.so"));
    }
}

// Test: version -----------------------------------------------------------------------------------

void TestPluginConfig::testVersion()
{
    // Default constructor
    {
        PluginConfig config;
        QVERIFY(config.version().isNull());
        QVERIFY(config.minVersion().isNull());
        QVERIFY(config.maxVersion().isNull());

        config.setVersion(VersionInfo(1, 0, 0));
        QCOMPARE(config.version(), VersionInfo(1, 0, 0));
        QCOMPARE(config.minVersion(), VersionInfo());
        QCOMPARE(config.maxVersion(), VersionInfo());
        QCOMPARE(config.isExactVersion(), true);
        QCOMPARE(config.isVersionRange(), false);

        config.setMinVersion(VersionInfo(1, 1, 0));
        QCOMPARE(config.version(), VersionInfo(1, 0, 0));
        QCOMPARE(config.minVersion(), VersionInfo(1, 1, 0));
        QCOMPARE(config.maxVersion(), VersionInfo());
        QCOMPARE(config.isExactVersion(), false);
        QCOMPARE(config.isVersionRange(), false);

        config.setVersion(VersionInfo());
        QCOMPARE(config.version(), VersionInfo());
        QCOMPARE(config.minVersion(), VersionInfo(1, 1, 0));
        QCOMPARE(config.maxVersion(), VersionInfo());
        QCOMPARE(config.isExactVersion(), false);
        QCOMPARE(config.isVersionRange(), false);

        config.setMaxVersion(VersionInfo(1, 1, 1));
        QCOMPARE(config.version(), VersionInfo());
        QCOMPARE(config.minVersion(), VersionInfo(1, 1, 0));
        QCOMPARE(config.maxVersion(), VersionInfo(1, 1, 1));
        QCOMPARE(config.isExactVersion(), false);
        QCOMPARE(config.isVersionRange(), true);

        config.setMinVersion(VersionInfo());
        QCOMPARE(config.version(), VersionInfo());
        QCOMPARE(config.minVersion(), VersionInfo());
        QCOMPARE(config.maxVersion(), VersionInfo(1, 1, 1));
        QCOMPARE(config.isExactVersion(), false);
        QCOMPARE(config.isVersionRange(), false);

        config.setVersion(VersionInfo(1, 0, 0));
        QCOMPARE(config.version(), VersionInfo(1, 0, 0));
        QCOMPARE(config.minVersion(), VersionInfo());
        QCOMPARE(config.maxVersion(), VersionInfo(1, 1, 1));
        QCOMPARE(config.isExactVersion(), false);
        QCOMPARE(config.isVersionRange(), false);
    }

    // Constructor
    {
        PluginConfig config("file.so", VersionInfo(1, 0, 0), {});
        QCOMPARE(config.version(), VersionInfo(1, 0, 0));
        QCOMPARE(config.minVersion(), VersionInfo());
        QCOMPARE(config.maxVersion(), VersionInfo());
        QCOMPARE(config.isExactVersion(), true);
        QCOMPARE(config.isVersionRange(), false);

        config = PluginConfig("file.so", VersionInfo(1, 0, 1), VersionInfo(1, 1, 0), {});
        QCOMPARE(config.version(), VersionInfo());
        QCOMPARE(config.minVersion(), VersionInfo(1, 0, 1));
        QCOMPARE(config.maxVersion(), VersionInfo(1, 1, 0));
        QCOMPARE(config.isExactVersion(), false);
        QCOMPARE(config.isVersionRange(), true);
    }
}

// Test: instance configs --------------------------------------------------------------------------

void TestPluginConfig::testInstanceConfigs()
{
    QList<PluginInstanceConfig> instanceConfigs;
    instanceConfigs.append(PluginInstanceConfig("test1"));
    instanceConfigs.append(PluginInstanceConfig("test2"));

    // Default constructor
    {
        PluginConfig config;
        QVERIFY(config.instanceConfigs().isEmpty());

        config.setInstanceConfigs(instanceConfigs);
        QCOMPARE(config.instanceConfigs(), instanceConfigs);
    }

    // Constructor
    {
        PluginConfig config("file.so", VersionInfo(1, 0, 0), {});
        QVERIFY(config.instanceConfigs().isEmpty());

        config = PluginConfig("file.so", VersionInfo(1, 0, 0), instanceConfigs);
        QCOMPARE(config.instanceConfigs(), instanceConfigs);

        config = PluginConfig("file.so", VersionInfo(1, 0, 1), VersionInfo(1, 1, 0), {});
        QVERIFY(config.instanceConfigs().isEmpty());

        config = PluginConfig("file.so",
                              VersionInfo(1, 0, 1),
                              VersionInfo(1, 1, 0),
                              instanceConfigs);
        QCOMPARE(config.instanceConfigs(), instanceConfigs);
    }
}

// Test: loadConfig() method -----------------------------------------------------------------------

void TestPluginConfig::testLoadConfig()
{
    QFETCH(ConfigObjectNodePtr, configNode);
    QFETCH(PluginConfig, expectedPluginConfig);
    QFETCH(bool, expectedResult);

    PluginConfig pluginConfig;
    const bool result = pluginConfig.loadConfig("plugin", *configNode);

    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(pluginConfig, expectedPluginConfig);
    }
}

void TestPluginConfig::testLoadConfig_data()
{
    QTest::addColumn<ConfigObjectNodePtr>("configNode");
    QTest::addColumn<PluginConfig>("expectedPluginConfig");
    QTest::addColumn<bool>("expectedResult");

    // Prepare common data
    const QString validFilePath(QCoreApplication::applicationFilePath());

    const VersionInfo validVersion1(1, 0, 0);
    const VersionInfo validVersion2(2, 0, 0);

    const QList<PluginInstanceConfig> validInstanceConfigs
    {
        PluginInstanceConfig("instance1"), PluginInstanceConfig("instance2")
    };

    ConfigObjectNode instances
    {
        {
            "instance1", ConfigObjectNode
            {
                { "name", ConfigValueNode("instance1") }
            }
        },
        {
            "instance2", ConfigObjectNode
            {
                { "name", ConfigValueNode("instance2") }
            }
        }

    };

    // Valid: exact version
    {
        ConfigObjectNode configNode
        {
            {
                "plugin", ConfigObjectNode
                {
                    { "file_path", ConfigValueNode(validFilePath) },
                    { "version", ConfigValueNode(validVersion1.toString()) },
                    { "instances", std::move(instances.clone()->toObject()) }
                }
            }
        };

        QTest::newRow("valid: exact version")
                << std::make_shared<ConfigObjectNode>(std::move(configNode))
                << PluginConfig(validFilePath, validVersion1, validInstanceConfigs)
                << true;
    }

    // Valid: version range
    {
        ConfigObjectNode configNode
        {
            {
                "plugin", ConfigObjectNode
                {
                    { "file_path", ConfigValueNode(validFilePath) },
                    { "min_version", ConfigValueNode(validVersion1.toString()) },
                    { "max_version", ConfigValueNode(validVersion2.toString()) },
                    { "instances", std::move(instances.clone()->toObject()) }
                }
            }
        };

        QTest::newRow("valid: version range")
                << std::make_shared<ConfigObjectNode>(std::move(configNode))
                << PluginConfig(validFilePath, validVersion1, validVersion2, validInstanceConfigs)
                << true;
    }

    // Invalid: file path missing
    {
        ConfigObjectNode configNode
        {
            {
                "plugin", ConfigObjectNode
                {
                    { "version", ConfigValueNode(validVersion1.toString()) },
                    { "instances", std::move(instances.clone()->toObject()) }
                }
            }
        };

        QTest::newRow("invalid: file path missing")
                << std::make_shared<ConfigObjectNode>(std::move(configNode))
                << PluginConfig()
                << false;
    }

    // Invalid: exact version
    {
        ConfigObjectNode configNode
        {
            {
                "plugin", ConfigObjectNode
                {
                    { "file_path", ConfigValueNode(validFilePath) },
                    { "version", ConfigValueNode(123) },
                    { "instances", std::move(instances.clone()->toObject()) }
                }
            }
        };

        QTest::newRow("invalid: exact version")
                << std::make_shared<ConfigObjectNode>(std::move(configNode))
                << PluginConfig()
                << false;
    }

    // Invalid: min version
    {
        ConfigObjectNode configNode
        {
            {
                "plugin", ConfigObjectNode
                {
                    { "file_path", ConfigValueNode(validFilePath) },
                    { "min_version", ConfigValueNode(123) },
                    { "max_version", ConfigValueNode(validVersion2.toString()) },
                    { "instances", std::move(instances.clone()->toObject()) }
                }
            }
        };

        QTest::newRow("invalid: min version")
                << std::make_shared<ConfigObjectNode>(std::move(configNode))
                << PluginConfig()
                << false;
    }

    // Invalid: max version
    {
        ConfigObjectNode configNode
        {
            {
                "plugin", ConfigObjectNode
                {
                    { "file_path", ConfigValueNode(validFilePath) },
                    { "min_version", ConfigValueNode(validVersion1.toString()) },
                    { "max_version", ConfigValueNode(123) },
                    { "instances", std::move(instances.clone()->toObject()) }
                }
            }
        };

        QTest::newRow("invalid: max version")
                << std::make_shared<ConfigObjectNode>(std::move(configNode))
                << PluginConfig()
                << false;
    }

    // Invalid: instances
    {
        ConfigObjectNode configNode
        {
            {
                "plugin", ConfigObjectNode
                {
                    { "file_path", ConfigValueNode(validFilePath) },
                    { "min_version", ConfigValueNode(validVersion1.toString()) },
                    { "max_version", ConfigValueNode(validVersion2.toString()) },
                    { "instances", ConfigValueNode() }
                }
            }
        };

        QTest::newRow("invalid: instances")
                << std::make_shared<ConfigObjectNode>(std::move(configNode))
                << PluginConfig()
                << false;
    }
}

// Main function -----------------------------------------------------------------------------------

QTEST_MAIN(TestPluginConfig)
#include "testPluginConfig.moc"
