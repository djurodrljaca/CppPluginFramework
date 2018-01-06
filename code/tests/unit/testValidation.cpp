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
#include <QtCore/QStringBuilder>
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

    // TODO: add other tests!

    void testValidateVersion();
    void testValidateVersion_data();

    void testIsVersionInRange();
    void testIsVersionInRange_data();

    void testValidateInterfaceName();
    void testValidateInterfaceName_data();
};

// Test Case init/cleanup metohods -----------------------------------------------------------------

void TestValidation::initTestCase()
{
}

void TestValidation::cleanupTestCase()
{
}

// Test init/cleanup metohods ----------------------------------------------------------------------

void TestValidation::init()
{
}

void TestValidation::cleanup()
{
}

// Test method: ValidateVersion --------------------------------------------------------------------

void TestValidation::testValidateVersion()
{
    QFETCH(QString, version);
    QFETCH(bool, result);

    qDebug() << "version:" << version;
    QCOMPARE(CppPluginFramework::Validation::validateVersion(version), result);
}

void TestValidation::testValidateVersion_data()
{
    QTest::addColumn<QString>("version");
    QTest::addColumn<bool>("result");

    // Valid results
    QTest::newRow("") << "0.0" << true;
    QTest::newRow("") << "9.9" << true;

    QTest::newRow("") << "00.00" << true;
    QTest::newRow("") << "99.99" << true;

    QTest::newRow("") << "0.0.0" << true;
    QTest::newRow("") << "9.9.9" << true;

    QTest::newRow("") << "000.000.000" << true;
    QTest::newRow("") << "999.999.999" << true;

    QTest::newRow("") << "0.0-a" << true;
    QTest::newRow("") << "9.9-z" << true;

    QTest::newRow("") << "0.0.0-a" << true;
    QTest::newRow("") << "9.9.9-z" << true;

    QTest::newRow("") << "0.0.0-0" << true;
    QTest::newRow("") << "9.9.9-9" << true;

    QTest::newRow("") << "0.0.0-aA" << true;
    QTest::newRow("") << "9.9.9-zZ" << true;

    QTest::newRow("") << "0.0.0-0aA" << true;
    QTest::newRow("") << "9.9.9-a5Z" << true;
    QTest::newRow("") << "9.9.9-zZ9" << true;

    QTest::newRow("") << "0.0.0-0_a" << true;
    QTest::newRow("") << "0.0.0-1_A" << true;
    QTest::newRow("") << "0.0.0-2_z" << true;
    QTest::newRow("") << "0.0.0-3_Z" << true;

    QTest::newRow("") << "0.0.0-0a_a" << true;
    QTest::newRow("") << "0.0.0-1A_A" << true;
    QTest::newRow("") << "0.0.0-2z_z" << true;
    QTest::newRow("") << "0.0.0-3Z_Z" << true;

    QTest::newRow("") << "0.0.0-0.a" << true;
    QTest::newRow("") << "0.0.0-1.A" << true;
    QTest::newRow("") << "0.0.0-2.z" << true;
    QTest::newRow("") << "0.0.0-3.Z" << true;

    QTest::newRow("") << "0.0.0-0a.a" << true;
    QTest::newRow("") << "0.0.0-1A.A" << true;
    QTest::newRow("") << "0.0.0-2z.z" << true;
    QTest::newRow("") << "0.0.0-3Z.Z" << true;

    QTest::newRow("") << "0.0.0-0-a" << true;
    QTest::newRow("") << "0.0.0-1-A" << true;
    QTest::newRow("") << "0.0.0-2-z" << true;
    QTest::newRow("") << "0.0.0-3-Z" << true;

    QTest::newRow("") << "0.0.0-0a-a" << true;
    QTest::newRow("") << "0.0.0-1A-A" << true;
    QTest::newRow("") << "0.0.0-2z-z" << true;
    QTest::newRow("") << "0.0.0-3Z-Z" << true;

    QTest::newRow("") << "0.0.0-0a_a.z-Z" << true;
    QTest::newRow("") << "0.0.0-0a-a_z.Z" << true;
    QTest::newRow("") << "0.0.0-0a.a-z_Z" << true;

    QTest::newRow("") << "0.0.0-a.." << true;
    QTest::newRow("") << "0.0.0-a__" << true;
    QTest::newRow("") << "0.0.0-a--" << true;

    // Invalid results
    QTest::newRow("") << "0" << false;
    QTest::newRow("") << "9" << false;

    QTest::newRow("") << "0." << false;
    QTest::newRow("") << "9." << false;

    QTest::newRow("") << "0.0." << false;
    QTest::newRow("") << "9.9." << false;

    QTest::newRow("") << "a0" << false;
    QTest::newRow("") << "A9" << false;
    QTest::newRow("") << "Z0" << false;
    QTest::newRow("") << "Z9" << false;

    QTest::newRow("") << "0a" << false;
    QTest::newRow("") << "9A" << false;
    QTest::newRow("") << "0Z" << false;
    QTest::newRow("") << "9Z" << false;

    QTest::newRow("") << "0.a" << false;
    QTest::newRow("") << "0.A" << false;
    QTest::newRow("") << "0.z" << false;
    QTest::newRow("") << "0.Z" << false;

    QTest::newRow("") << "0.a0" << false;
    QTest::newRow("") << "0.A9" << false;
    QTest::newRow("") << "0.Z0" << false;
    QTest::newRow("") << "0.Z9" << false;

    QTest::newRow("") << "0a.0" << false;
    QTest::newRow("") << "9A.0" << false;
    QTest::newRow("") << "0Z.0" << false;
    QTest::newRow("") << "9Z.0" << false;

    QTest::newRow("") << "0.0.a" << false;
    QTest::newRow("") << "0.0.A" << false;
    QTest::newRow("") << "0.0.z" << false;
    QTest::newRow("") << "0.0.Z" << false;

    QTest::newRow("") << "0.0.0-" << false;
    QTest::newRow("") << "0.0.9-" << false;

    QTest::newRow("") << "0.0.0-." << false;
    QTest::newRow("") << "0.0.0-_" << false;
    QTest::newRow("") << "0.0.0--" << false;
}

// Test method: isVersionInRange --------------------------------------------------------------

void TestValidation::testIsVersionInRange()
{
    QFETCH(QString, version);
    QFETCH(QString, min);
    QFETCH(QString, max);
    QFETCH(bool, result);

    qDebug() << version << min << max;
    QCOMPARE(CppPluginFramework::Validation::isVersionInRange(version, min, max), result);
}

void TestValidation::testIsVersionInRange_data()
{
    QTest::addColumn<QString>("version");
    QTest::addColumn<QString>("min");
    QTest::addColumn<QString>("max");
    QTest::addColumn<bool>("result");

    // Valid results
    QTest::newRow("") << "1.0"   << "1.0" << "2.0" << true;
    QTest::newRow("") << "1.999" << "1.0" << "2.0" << true;

    QTest::newRow("") << "1.0.0"     << "1.0.0" << "2.0.0" << true;
    QTest::newRow("") << "1.999.0"   << "1.0.0" << "2.0.0" << true;
    QTest::newRow("") << "1.999.999" << "1.0.0" << "2.0.0" << true;

    QTest::newRow("") << "1.9"  << "1.5" << "2.3" << true;
    QTest::newRow("") << "1.10" << "1.5" << "2.3" << true;
    QTest::newRow("") << "2.2"  << "1.5" << "2.3" << true;

    QTest::newRow("") << "1.9.0" << "1.5.0" << "2.3.0" << true;
    QTest::newRow("") << "2.2.9" << "1.5.0" << "2.3.0" << true;

    QTest::newRow("") << "1.9.0" << "1.5.8" << "2.3.1" << true;
    QTest::newRow("") << "2.2.9" << "1.5.9" << "2.3.0" << true;

    QTest::newRow("") << "1.9-a" << "1.9-a" << "1.9-c" << true;
    QTest::newRow("") << "1.9-b" << "1.9-a" << "1.9-c" << true;

    QTest::newRow("") << "1.9.6-a" << "1.9.6-a" << "1.9.6-c" << true;
    QTest::newRow("") << "1.9.6-b" << "1.9.6-a" << "1.9.6-c" << true;
    QTest::newRow("") << "1.9.6-c" << "1.9.0-a" << "1.9.9-c" << true;

    // Invalid results
    QTest::newRow("") << "0.9" << "1.0" << "2.0" << false;
    QTest::newRow("") << "2.0" << "1.0" << "2.0" << false;

    QTest::newRow("") << "0.999.999" << "1.0" << "2.0" << false;
    QTest::newRow("") << "2.0.0"     << "1.0" << "2.0" << false;

    QTest::newRow("") << "1.9-a" << "1.9-b" << "1.9-c" << false;
    QTest::newRow("") << "1.9-d" << "1.9-b" << "1.9-c" << false;

    QTest::newRow("") << "1.9.6-a"  << "1.9.6-b" << "1.9.6-c" << false;
    QTest::newRow("") << "1.9.6-d"  << "1.9.6-b" << "1.9.6-c" << false;
    QTest::newRow("") << "1.8.9-b"  << "1.9.0-b" << "1.9.9-c" << false;
    QTest::newRow("") << "1.9.10-a" << "1.9.0-b" << "1.9.9-c" << false;

    QTest::newRow("") << "1.9-9"  << "1.9-10" << "1.9-11" << false;
    QTest::newRow("") << "1.9-97" << "1.9-98" << "1.9-99" << false;
}

// Test method: validateInterfaceName --------------------------------------------------------------

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

    // Valid results
    QTest::newRow("") << "a" << true;
    QTest::newRow("") << "z" << true;
    QTest::newRow("") << "A" << true;
    QTest::newRow("") << "Z" << true;

    QTest::newRow("") << "aA" << true;
    QTest::newRow("") << "zZ" << true;
    QTest::newRow("") << "AA" << true;
    QTest::newRow("") << "ZZ" << true;

    QTest::newRow("") << "aaA" << true;
    QTest::newRow("") << "zzZ" << true;
    QTest::newRow("") << "AAaa" << true;
    QTest::newRow("") << "ZZzz" << true;

    QTest::newRow("") << "a0" << true;
    QTest::newRow("") << "z9" << true;
    QTest::newRow("") << "A0" << true;
    QTest::newRow("") << "Z9" << true;

    QTest::newRow("") << "a0A" << true;
    QTest::newRow("") << "zZ9" << true;
    QTest::newRow("") << "A00A" << true;
    QTest::newRow("") << "ZZ99" << true;

    QTest::newRow("") << "nameName" << true;
    QTest::newRow("") << "NameName" << true;

    QTest::newRow("") << "nameName::name" << true;
    QTest::newRow("") << "nameName::Name" << true;
    QTest::newRow("") << "nameName::name::Longer" << true;
    QTest::newRow("") << "nameName::Name::longer" << true;

    // Invalid results
    QTest::newRow("") << "0" << false;
    QTest::newRow("") << "9" << false;

    QTest::newRow("") << "0a" << false;
    QTest::newRow("") << "9Z" << false;

    QTest::newRow("") << "nameName::" << false;

    QTest::newRow("") << "nameName::name::0" << false;
    QTest::newRow("") << "nameName::9::Name" << false;

    QTest::newRow("") << "nameName::name::00::Longer" << false;
    QTest::newRow("") << "nameName::Name::longer::99" << false;
}

// Main function -----------------------------------------------------------------------------------

QTEST_MAIN(TestValidation)
#include "testValidation.moc"
