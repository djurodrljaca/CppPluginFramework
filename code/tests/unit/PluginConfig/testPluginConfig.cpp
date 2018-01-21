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

// Qt includes
#include <QtCore/QDebug>
#include <QtTest/QTest>

// System includes

// Forward declarations

// Macros

// Test class declaration --------------------------------------------------------------------------

using namespace CppPluginFramework;
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

private:
    // Holds the path to the data directory
    QDir m_testDataDirPath;
};

// Test Case init/cleanup methods ------------------------------------------------------------------

void TestPluginConfig::initTestCase()
{
    m_testDataDirPath = QDir(TEST_DATA_DIR_PATH);
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
    const QString validFilePath = m_testDataDirPath.filePath("dummyPlugin");

    const VersionInfo validVersion1(1, 0, 0);
    const VersionInfo validVersion2(2, 0, 0);

    const QList<PluginInstanceConfig> validInstanceConfigs
    {
        PluginInstanceConfig("instance1"), PluginInstanceConfig("instance2")
    };

    // Valid results
    QTest::newRow("valid: specific version") << PluginConfig(validFilePath,
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

    QTest::newRow("invalid: specific version")
            << PluginConfig(validFilePath, VersionInfo(), validInstanceConfigs) << false;

    QTest::newRow("invalid: min version")
            << PluginConfig(validFilePath, VersionInfo(), validVersion2, validInstanceConfigs)
            << false;

    QTest::newRow("invalid: max version")
            << PluginConfig(validFilePath, validVersion1, VersionInfo(), validInstanceConfigs)
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

// Main function -----------------------------------------------------------------------------------

QTEST_MAIN(TestPluginConfig)
#include "testPluginConfig.moc"
