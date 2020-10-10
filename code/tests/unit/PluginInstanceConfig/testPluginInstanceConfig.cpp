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
 * Contains unit tests for PluginInstanceConfig class
 */

// C++ Plugin Framework includes
#include <CppPluginFramework/PluginInstanceConfig.hpp>

// C++ Config Framework includes
#include <CppConfigFramework/ConfigValueNode.hpp>

// Qt includes
#include <QtCore/QDebug>
#include <QtTest/QTest>

// System includes

// Forward declarations

// Macros

// Test types --------------------------------------------------------------------------------------

using namespace CppConfigFramework;
using namespace CppPluginFramework;

using ConfigObjectNodePtr = std::shared_ptr<ConfigObjectNode>;

Q_DECLARE_METATYPE(ConfigObjectNodePtr)
Q_DECLARE_METATYPE(PluginInstanceConfig)

// Test class declaration --------------------------------------------------------------------------
class TestPluginInstanceConfig : public QObject
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

    void testName();
    void testConfig();
    void testDependencies();

    void testLoadConfig();
    void testLoadConfig_data();

};

// Test Case init/cleanup methods ------------------------------------------------------------------

void TestPluginInstanceConfig::initTestCase()
{
}

void TestPluginInstanceConfig::cleanupTestCase()
{
}

// Test init/cleanup methods -----------------------------------------------------------------------

void TestPluginInstanceConfig::init()
{
}

void TestPluginInstanceConfig::cleanup()
{
}

// Test: validity ----------------------------------------------------------------------------------

void TestPluginInstanceConfig::testIsValid()
{
    QFETCH(PluginInstanceConfig, config);
    QFETCH(bool, result);

    QCOMPARE(config.isValid(), result);
}

void TestPluginInstanceConfig::testIsValid_data()
{
    QTest::addColumn<PluginInstanceConfig>("config");
    QTest::addColumn<bool>("result");

    // Valid results
    QTest::newRow("valid: only name") << PluginInstanceConfig("instance1") << true;

    QTest::newRow("valid: name and single dependency")
            << PluginInstanceConfig("instance2", {}, { "instance3" } )
            << true;

    QTest::newRow("valid: name and multiple dependencies")
            << PluginInstanceConfig("instance2", {}, { "instance3", "instance4"} )
            << true;

    // Invalid results
    QTest::newRow("invalid: default constructed") << PluginInstanceConfig() << false;
    QTest::newRow("invalid: only invalid name") << PluginInstanceConfig("1instance") << false;

    QTest::newRow("invalid: valid name and invalid single dependency")
            << PluginInstanceConfig("instance2", {}, { "3instance" } )
            << false;

    QTest::newRow("invalid: valid name and invalid multiple dependencies")
            << PluginInstanceConfig("instance2", {}, { "3instance", "instance4" } )
            << false;

    QTest::newRow("invalid: valid name and dependency to itself")
            << PluginInstanceConfig("instance2", {}, { "instance2" } )
            << false;
}

// Test: instance name -----------------------------------------------------------------------------

void TestPluginInstanceConfig::testName()
{
    // Default constructed
    {
        PluginInstanceConfig instanceConfig;
        QVERIFY(instanceConfig.name().isEmpty());

        instanceConfig.setName("asd");
        QCOMPARE(instanceConfig.name(), "asd");
    }

    // Constructed with initial name
    {
        PluginInstanceConfig instanceConfig("aaa");
        QCOMPARE(instanceConfig.name(), "aaa");

        instanceConfig.setName("bbb");
        QCOMPARE(instanceConfig.name(), "bbb");
    }
}

// Test: instance config ---------------------------------------------------------------------------

void TestPluginInstanceConfig::testConfig()
{
    // Default constructed
    {
        PluginInstanceConfig instanceConfig;
        QCOMPARE(instanceConfig.config().count(), 0);

        ConfigObjectNode config
        {
            { "aaa", ConfigValueNode(1) },
            { "bbb", ConfigValueNode("str") }
        };

        instanceConfig.setConfig(config);
        QCOMPARE(instanceConfig.config(), config);
    }

    // Constructed with initial config
    {
        ConfigObjectNode config
        {
            { "aaa", ConfigValueNode(1) }
        };

        PluginInstanceConfig instanceConfig("aaa", config);
        QCOMPARE(instanceConfig.config(), config);

        config.setMember("bbb", ConfigValueNode("str"));
        instanceConfig.setConfig(config);
        QCOMPARE(instanceConfig.config(), config);
    }
}

// Test: instance dependencies ---------------------------------------------------------------------

void TestPluginInstanceConfig::testDependencies()
{
    // Default constructed
    {
        PluginInstanceConfig instanceConfig;
        QVERIFY(instanceConfig.dependencies().isEmpty());

        const QSet<QString> dependencies {"aaa", "bbb"};
        instanceConfig.setDependencies(dependencies);
        QCOMPARE(instanceConfig.dependencies(), dependencies);
    }

    // Constructed with initial dependencies
    {
        QSet<QString> dependencies {"aaa", "bbb"};
        PluginInstanceConfig instanceConfig("aaa", {}, dependencies);
        QCOMPARE(instanceConfig.dependencies(), dependencies);

        dependencies.insert("ccc");
        instanceConfig.setDependencies(dependencies);
        QCOMPARE(instanceConfig.dependencies(), dependencies);
    }
}

// Test: loadConfig() method -----------------------------------------------------------------------

void TestPluginInstanceConfig::testLoadConfig()
{
    QFETCH(ConfigObjectNodePtr, configNode);
    QFETCH(PluginInstanceConfig, expectedInstanceConfig);
    QFETCH(bool, expectedResult);

    PluginInstanceConfig instanceConfig;
    const bool result = instanceConfig.loadConfig("instance", *configNode);

    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(instanceConfig, expectedInstanceConfig);
    }
}

void TestPluginInstanceConfig::testLoadConfig_data()
{
    QTest::addColumn<ConfigObjectNodePtr>("configNode");
    QTest::addColumn<PluginInstanceConfig>("expectedInstanceConfig");
    QTest::addColumn<bool>("expectedResult");

    // Valid: just name
    {
        ConfigObjectNode configNode
        {
            {
                "instance", ConfigObjectNode
                {
                    { "name", ConfigValueNode("test1") }
                }
            }
        };

        auto instanceConfig = PluginInstanceConfig("test1");

        QTest::newRow("valid: only name")
                << std::make_shared<ConfigObjectNode>(std::move(configNode))
                << instanceConfig
                << true;
    }

    // Valid: name and config
    {
        ConfigObjectNode config
        {
            { "param", ConfigValueNode("value") }
        };

        ConfigObjectNode configNode
        {
            {
                "instance", ConfigObjectNode
                {
                    { "name", ConfigValueNode("test2") },
                    { "config", std::move(config.clone()->toObject()) }
                }
            }
        };

        auto instanceConfig = PluginInstanceConfig("test2", config);

        QTest::newRow("valid: name and config")
                << std::make_shared<ConfigObjectNode>(std::move(configNode))
                << instanceConfig
                << true;
    }

    // Valid: name and dependencies
    {
        const QSet<QString> dependencies = {"aaa", "bbb"};

        ConfigObjectNode configNode
        {
            {
                "instance", ConfigObjectNode
                {
                    { "name", ConfigValueNode("test3") },
                    {
                        "dependencies",
                        ConfigValueNode(QJsonArray::fromStringList(dependencies.values()))
                    }
                }
            }
        };

        auto instanceConfig = PluginInstanceConfig("test3", {}, dependencies);

        QTest::newRow("valid: name and dependencies")
                << std::make_shared<ConfigObjectNode>(std::move(configNode))
                << instanceConfig
                << true;
    }

    // Valid: all params
    {
        const QSet<QString> dependencies = {"aaa", "bbb"};

        ConfigObjectNode config
        {
            { "param", ConfigValueNode("value") }
        };

        ConfigObjectNode configNode
        {
            {
                "instance", ConfigObjectNode
                {
                    { "name", ConfigValueNode("test4") },
                    { "config", std::move(config.clone()->toObject()) },
                    {
                        "dependencies",
                        ConfigValueNode(QJsonArray::fromStringList(dependencies.values()))
                    }
                }
            }
        };

        auto instanceConfig = PluginInstanceConfig("test4", config, dependencies);

        QTest::newRow("valid: all params")
                << std::make_shared<ConfigObjectNode>(std::move(configNode))
                << instanceConfig
                << true;
    }

    // Invalid: name
    {
        ConfigObjectNode configNode1
        {
            {
                "instance", ConfigObjectNode
                {
                    { "name", ConfigValueNode("0test") }
                }
            }
        };

        QTest::newRow("invalid: name 1")
                << std::make_shared<ConfigObjectNode>(std::move(configNode1))
                << PluginInstanceConfig()
                << false;

        ConfigObjectNode configNode2
        {
            {
                "instance", ConfigObjectNode
                {
                    { "name", ConfigValueNode("") }
                }
            }
        };

        QTest::newRow("invalid: name 2")
                << std::make_shared<ConfigObjectNode>(std::move(configNode2))
                << PluginInstanceConfig()
                << false;
    }

    // Invalid: config
    {
        ConfigObjectNode configNode
        {
            {
                "instance", ConfigObjectNode
                {
                    { "name", ConfigValueNode("test") },
                    { "config", ConfigValueNode() }
                }
            }
        };

        QTest::newRow("invalid: config")
                << std::make_shared<ConfigObjectNode>(std::move(configNode))
                << PluginInstanceConfig()
                << false;
    }

    // Invalid: dependencies
    {
        ConfigObjectNode configNode1
        {
            {
                "instance", ConfigObjectNode
                {
                    { "name", ConfigValueNode("test") },
                    { "dependencies", ConfigValueNode(QJsonArray { "test1", "test2", "test1" }) }
                }
            }
        };

        QTest::newRow("invalid: dependencies 1")
                << std::make_shared<ConfigObjectNode>(std::move(configNode1))
                << PluginInstanceConfig()
                << false;

        ConfigObjectNode configNode2
        {
            {
                "instance", ConfigObjectNode
                {
                    { "name", ConfigValueNode("test") },
                    { "dependencies", ConfigValueNode(QJsonArray { "test1", "test" }) }
                }
            }
        };

        QTest::newRow("invalid: dependencies 2")
                << std::make_shared<ConfigObjectNode>(std::move(configNode2))
                << PluginInstanceConfig()
                << false;

        ConfigObjectNode configNode3
        {
            {
                "instance", ConfigObjectNode
                {
                    { "name", ConfigValueNode("test") },
                    { "dependencies", ConfigValueNode(QJsonArray { "0ab" }) }
                }
            }
        };

        QTest::newRow("invalid: dependencies 3")
                << std::make_shared<ConfigObjectNode>(std::move(configNode3))
                << PluginInstanceConfig()
                << false;
    }

}

// Main function -----------------------------------------------------------------------------------

QTEST_MAIN(TestPluginInstanceConfig)
#include "testPluginInstanceConfig.moc"
