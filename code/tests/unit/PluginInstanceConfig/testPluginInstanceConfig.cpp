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

// Qt includes
#include <QtCore/QDebug>
#include <QtTest/QTest>

// System includes

// Forward declarations

// Macros

// Test class declaration --------------------------------------------------------------------------

using namespace CppPluginFramework;
Q_DECLARE_METATYPE(PluginInstanceConfig)

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
            << PluginInstanceConfig("instance2",
                                    QJsonObject(),
                                    QSet<QString> { "instance3" } ) << true;

    QTest::newRow("valid: name and multiple dependencies")
            << PluginInstanceConfig("instance2",
                                    QJsonObject(),
                                    QSet<QString> { "instance3", "instance4"} ) << true;

    // Invalid results
    QTest::newRow("invalid: default constructed") << PluginInstanceConfig() << false;
    QTest::newRow("invalid: only name") << PluginInstanceConfig("1instance") << false;

    QTest::newRow("invalid: valid name and invalid single dependency")
            << PluginInstanceConfig("instance2",
                                    QJsonObject(),
                                    QSet<QString> { "3instance" } ) << false;

    QTest::newRow("invalid: valid name and invalid multiple dependencies")
            << PluginInstanceConfig("instance2",
                                    QJsonObject(),
                                    QSet<QString> { "3instance", "instance4" } ) << false;

    QTest::newRow("invalid: valid name and dependency to itself")
            << PluginInstanceConfig("instance2",
                                    QJsonObject(),
                                    QSet<QString> { "instance2" } ) << false;
}

// Main function -----------------------------------------------------------------------------------

QTEST_MAIN(TestPluginInstanceConfig)
#include "testPluginInstanceConfig.moc"
