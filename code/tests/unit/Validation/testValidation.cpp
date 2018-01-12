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
 * Contains unit tests for validation methods
 */

// C++ Plugin Framework includes
#include <CppPluginFramework/Validation.hpp>

// Qt includes
#include <QtCore/QDebug>
#include <QtCore/QFileInfo>
#include <QtCore/QRegularExpression>
#include <QtTest/QTest>

// System includes

// Forward declarations

// Macros

// Test class declaration --------------------------------------------------------------------------

class TestValidation : public QObject {
    Q_OBJECT

private slots:
    // Functions executed by QtTest before and after test suite
    void initTestCase();
    void cleanupTestCase();

    // Functions executed by QtTest before and after each test
    void init();
    void cleanup();

    // Test functions
    void testValidatePluginInstanceName();
    void testValidatePluginInstanceName_data();

    void testValidateInterfaceName();
    void testValidateInterfaceName_data();

    void testValidateExportedInterfaces();
    void testValidateExportedInterfaces_data();

    void testValidateEnvironmentVariableName();
    void testValidateEnvironmentVariableName_data();

    void testValidateFilePath();
    void testValidateFilePath_data();

private:
    // Holds the path to the data directory
    QDir m_testDataDirPath;
};

// Test Case init/cleanup methods ------------------------------------------------------------------

void TestValidation::initTestCase()
{
    m_testDataDirPath = QDir(TEST_DATA_DIR_PATH);
}

void TestValidation::cleanupTestCase()
{
}

// Test init/cleanup methods ----------------------------------------------------------------------

void TestValidation::init()
{
}

void TestValidation::cleanup()
{
}

// Test: validation of plugin instance names -------------------------------------------------------

void TestValidation::testValidatePluginInstanceName()
{
    QFETCH(QString, name);
    QFETCH(bool, result);

    qDebug() << "name:" << name;
    QCOMPARE(CppPluginFramework::Validation::validatePluginInstanceName(name), result);
}

void TestValidation::testValidatePluginInstanceName_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<bool>("result");

    int i = 0;

    // Valid results
    QTest::addRow("%d", i++) << "a" << true;
    QTest::addRow("%d", i++) << "z" << true;
    QTest::addRow("%d", i++) << "A" << true;
    QTest::addRow("%d", i++) << "Z" << true;

    QTest::addRow("%d", i++) << "aA" << true;
    QTest::addRow("%d", i++) << "zZ" << true;
    QTest::addRow("%d", i++) << "AA" << true;
    QTest::addRow("%d", i++) << "ZZ" << true;

    QTest::addRow("%d", i++) << "aaA" << true;
    QTest::addRow("%d", i++) << "zzZ" << true;
    QTest::addRow("%d", i++) << "AAaa" << true;
    QTest::addRow("%d", i++) << "ZZzz" << true;

    QTest::addRow("%d", i++) << "a0" << true;
    QTest::addRow("%d", i++) << "z9" << true;
    QTest::addRow("%d", i++) << "A0" << true;
    QTest::addRow("%d", i++) << "Z9" << true;

    QTest::addRow("%d", i++) << "a0A" << true;
    QTest::addRow("%d", i++) << "zZ9" << true;
    QTest::addRow("%d", i++) << "A00A" << true;
    QTest::addRow("%d", i++) << "ZZ99" << true;

    QTest::addRow("%d", i++) << "nameName" << true;
    QTest::addRow("%d", i++) << "Name0Name" << true;

    QTest::addRow("%d", i++) << "name.Name" << true;
    QTest::addRow("%d", i++) << "name_Name" << true;
    QTest::addRow("%d", i++) << "name-name" << true;
    QTest::addRow("%d", i++) << "name-0name" << true;

    QTest::addRow("%d", i++) << "name.Name-name_09" << true;

    // Invalid results
    QTest::addRow("%d", i++) << "0" << false;
    QTest::addRow("%d", i++) << "9" << false;

    QTest::addRow("%d", i++) << "0a" << false;
    QTest::addRow("%d", i++) << "9Z" << false;

    QTest::addRow("%d", i++) << "_" << false;
    QTest::addRow("%d", i++) << "-" << false;
    QTest::addRow("%d", i++) << "." << false;
}

// Test: validation of plugin interface names ------------------------------------------------------

void TestValidation::testValidateInterfaceName()
{
    QFETCH(QString, name);
    QFETCH(bool, result);

    qDebug() << "name:" << name;
    QCOMPARE(CppPluginFramework::Validation::validateInterfaceName(name), result);
}

void TestValidation::testValidateInterfaceName_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<bool>("result");

    int i = 0;

    // Valid results
    QTest::addRow("%d", i++) << "a" << true;
    QTest::addRow("%d", i++) << "z" << true;
    QTest::addRow("%d", i++) << "A" << true;
    QTest::addRow("%d", i++) << "Z" << true;

    QTest::addRow("%d", i++) << "aA" << true;
    QTest::addRow("%d", i++) << "zZ" << true;
    QTest::addRow("%d", i++) << "AA" << true;
    QTest::addRow("%d", i++) << "ZZ" << true;

    QTest::addRow("%d", i++) << "aaA" << true;
    QTest::addRow("%d", i++) << "zzZ" << true;
    QTest::addRow("%d", i++) << "AAaa" << true;
    QTest::addRow("%d", i++) << "ZZzz" << true;

    QTest::addRow("%d", i++) << "a0" << true;
    QTest::addRow("%d", i++) << "z9" << true;
    QTest::addRow("%d", i++) << "A0" << true;
    QTest::addRow("%d", i++) << "Z9" << true;

    QTest::addRow("%d", i++) << "a0A" << true;
    QTest::addRow("%d", i++) << "zZ9" << true;
    QTest::addRow("%d", i++) << "A00A" << true;
    QTest::addRow("%d", i++) << "ZZ99" << true;

    QTest::addRow("%d", i++) << "nameName" << true;
    QTest::addRow("%d", i++) << "NameName" << true;

    QTest::addRow("%d", i++) << "nameName::name" << true;
    QTest::addRow("%d", i++) << "nameName::Name" << true;
    QTest::addRow("%d", i++) << "nameName::name::Longer" << true;
    QTest::addRow("%d", i++) << "nameName::Name::longer" << true;

    // Invalid results
    QTest::addRow("%d", i++) << "0" << false;
    QTest::addRow("%d", i++) << "9" << false;

    QTest::addRow("%d", i++) << "0a" << false;
    QTest::addRow("%d", i++) << "9Z" << false;

    QTest::addRow("%d", i++) << "nameName::" << false;

    QTest::addRow("%d", i++) << "nameName::name::0" << false;
    QTest::addRow("%d", i++) << "nameName::9::Name" << false;

    QTest::addRow("%d", i++) << "nameName::name::00::Longer" << false;
    QTest::addRow("%d", i++) << "nameName::Name::longer::99" << false;
}

// Test: validation of exported interface names ----------------------------------------------------

void TestValidation::testValidateExportedInterfaces()
{
    QFETCH(QSet<QString>, names);
    QFETCH(bool, result);

    qDebug() << "names:" << names;
    QCOMPARE(CppPluginFramework::Validation::validateExportedInterfaces(names), result);
}

void TestValidation::testValidateExportedInterfaces_data()
{
    QTest::addColumn<QSet<QString>>("names");
    QTest::addColumn<bool>("result");

    int i = 0;

    // Valid results
    QTest::addRow("%d", i++) << QSet<QString> { "Interface1", "interface2" } << true;
    QTest::addRow("%d", i++) << QSet<QString> { "namespace::Interface1", "namespace::interface2" } << true;

    // Invalid results
    QTest::addRow("%d", i++) << QSet<QString> { "Interface1", "interface2+" } << false;
    QTest::addRow("%d", i++) << QSet<QString> { "Interface1", "interface2*" } << false;
    QTest::addRow("%d", i++) << QSet<QString> { "Interface1", "interface2::" } << false;

    QTest::addRow("%d", i++) << QSet<QString> { "namespace::Interface1", "namespace::+interface2" } << false;
    QTest::addRow("%d", i++) << QSet<QString> { "namespace::Interface1", "namespace::*interface2" } << false;
    QTest::addRow("%d", i++) << QSet<QString> { "namespace::Interface1", "namespace::*interface2-" } << false;
    QTest::addRow("%d", i++) << QSet<QString> { "namespace::Interface1", "namespace::*interface2::" } << false;
}

// Test: validation of environment variable names --------------------------------------------------

void TestValidation::testValidateEnvironmentVariableName()
{
    QFETCH(QString, name);
    QFETCH(bool, result);

    qDebug() << "name:" << name;
    QCOMPARE(CppPluginFramework::Validation::validateEnvironmentVariableName(name), result);
}

void TestValidation::testValidateEnvironmentVariableName_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<bool>("result");

    int i = 0;

    // Valid results
    QTest::addRow("%d", i++) << "a" << true;
    QTest::addRow("%d", i++) << "z" << true;
    QTest::addRow("%d", i++) << "A" << true;
    QTest::addRow("%d", i++) << "Z" << true;

    QTest::addRow("%d", i++) << "aA" << true;
    QTest::addRow("%d", i++) << "zZ" << true;
    QTest::addRow("%d", i++) << "AA" << true;
    QTest::addRow("%d", i++) << "ZZ" << true;

    QTest::addRow("%d", i++) << "aaA" << true;
    QTest::addRow("%d", i++) << "zzZ" << true;
    QTest::addRow("%d", i++) << "AAaa" << true;
    QTest::addRow("%d", i++) << "ZZzz" << true;

    QTest::addRow("%d", i++) << "a0" << true;
    QTest::addRow("%d", i++) << "z9" << true;
    QTest::addRow("%d", i++) << "A0" << true;
    QTest::addRow("%d", i++) << "Z9" << true;

    QTest::addRow("%d", i++) << "a0A" << true;
    QTest::addRow("%d", i++) << "zZ9" << true;
    QTest::addRow("%d", i++) << "A00A" << true;
    QTest::addRow("%d", i++) << "ZZ99" << true;

    QTest::addRow("%d", i++) << "nameName" << true;
    QTest::addRow("%d", i++) << "NameName" << true;

    // Invalid results
    QTest::addRow("%d", i++) << "0" << false;
    QTest::addRow("%d", i++) << "9" << false;

    QTest::addRow("%d", i++) << "0a" << false;
    QTest::addRow("%d", i++) << "9Z" << false;

    QTest::addRow("%d", i++) << "nameName:" << false;
    QTest::addRow("%d", i++) << "nameName+" << false;
    QTest::addRow("%d", i++) << "nameName-" << false;
    QTest::addRow("%d", i++) << "nameName*" << false;
}

// Test: validation of file paths ------------------------------------------------------------------

void TestValidation::testValidateFilePath()
{
    QFETCH(QString, filePath);
    QFETCH(bool, result);

    qDebug() << "name:" << filePath;
    QCOMPARE(CppPluginFramework::Validation::validateFilePath(filePath), result);
}

void TestValidation::testValidateFilePath_data()
{
    QTest::addColumn<QString>("filePath");
    QTest::addColumn<bool>("result");

    int i = 0;

    // Valid results
    QTest::addRow("%d", i++) << m_testDataDirPath.filePath("file") << true;
    QTest::addRow("%d", i++) << m_testDataDirPath.filePath("file.txt") << true;
    QTest::addRow("%d", i++) << m_testDataDirPath.filePath("file.ext") << true;

    QTest::addRow("%d", i++) << m_testDataDirPath.filePath("somePath/file") << true;
    QTest::addRow("%d", i++) << m_testDataDirPath.filePath("somePath/file.txt") << true;
    QTest::addRow("%d", i++) << m_testDataDirPath.filePath("somePath/file.ext") << true;

    // Invalid results
    QTest::addRow("%d", i++) << m_testDataDirPath.filePath("somePath") << false;
    QTest::addRow("%d", i++) << m_testDataDirPath.filePath("file.aaa") << false;
    QTest::addRow("%d", i++) << m_testDataDirPath.filePath("file1") << false;
    QTest::addRow("%d", i++) << m_testDataDirPath.filePath("file1.txt") << false;
    QTest::addRow("%d", i++) << m_testDataDirPath.filePath("file1.ext") << false;

    QTest::addRow("%d", i++) << m_testDataDirPath.filePath("somePath/file.aaa") << false;
    QTest::addRow("%d", i++) << m_testDataDirPath.filePath("somePath/file1") << false;
    QTest::addRow("%d", i++) << m_testDataDirPath.filePath("somePath/file1.txt") << false;
    QTest::addRow("%d", i++) << m_testDataDirPath.filePath("somePath/file1.ext") << false;
}

// Main function -----------------------------------------------------------------------------------

QTEST_MAIN(TestValidation)
#include "testValidation.moc"
