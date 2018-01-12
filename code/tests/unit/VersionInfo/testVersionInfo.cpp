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
 * Contains unit tests for VersionInfo class
 */

// C++ Plugin Framework includes
#include <CppPluginFramework/VersionInfo.hpp>

// Qt includes
#include <QtCore/QDebug>
#include <QtTest/QTest>

// System includes

// Forward declarations

// Macros

// Test class declaration --------------------------------------------------------------------------

using namespace CppPluginFramework;
Q_DECLARE_METATYPE(VersionInfo)

class TestVersionInfo : public QObject
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
    void testParseValidVersions();
    void testParseValidVersions_data();

    void testParseInvalidVersions();
    void testParseInvalidVersions_data();

    void testIsValid();
    void testIsValid_data();

    void testIsRangeValid();
    void testIsRangeValid_data();

    void testIsVersionInRange();
    void testIsVersionInRange_data();

    void testToString();
    void testToString_data();
};

// Test Case init/cleanup methods ------------------------------------------------------------------

void TestVersionInfo::initTestCase()
{
}

void TestVersionInfo::cleanupTestCase()
{
}

// Test init/cleanup methods -----------------------------------------------------------------------

void TestVersionInfo::init()
{
}

void TestVersionInfo::cleanup()
{
}

// Test: parsing of valid versions -----------------------------------------------------------------

void TestVersionInfo::testParseValidVersions()
{
    QFETCH(QString, versionString);
    QFETCH(VersionInfo, expectedVersion);

    const VersionInfo version(versionString);

    qDebug() << "versionString:" << versionString;
    QVERIFY(version.isValid());
    QCOMPARE(version, expectedVersion);
}

void TestVersionInfo::testParseValidVersions_data()
{
    QTest::addColumn<QString>("versionString");
    QTest::addColumn<VersionInfo>("expectedVersion");

    int i = 0;

    QTest::addRow("%d", i++) << "0.0.0" << VersionInfo(0, 0, 0);
    QTest::addRow("%d", i++) << "9.9.9" << VersionInfo(9, 9, 9);

    QTest::addRow("%d", i++) << "000.000.000" << VersionInfo(0, 0, 0);
    QTest::addRow("%d", i++) << "999.999.999" << VersionInfo(999, 999, 999);

    QTest::addRow("%d", i++) << "0.0.0-a" << VersionInfo(0, 0, 0, "a");
    QTest::addRow("%d", i++) << "9.9.9-Z" << VersionInfo(9, 9, 9, "Z");
    QTest::addRow("%d", i++) << "0.0.0-0" << VersionInfo(0, 0, 0, "0");
    QTest::addRow("%d", i++) << "9.9.9-9" << VersionInfo(9, 9, 9, "9");
    QTest::addRow("%d", i++) << "0.0.0-." << VersionInfo(0, 0, 0, ".");
    QTest::addRow("%d", i++) << "0.0.0-_" << VersionInfo(0, 0, 0, "_");
    QTest::addRow("%d", i++) << "0.0.0--" << VersionInfo(0, 0, 0, "-");

    QTest::addRow("%d", i++) << "0.0.0-a$s97d-6fs_a8.6#7d" << VersionInfo(0, 0, 0, "a$s97d-6fs_a8.6#7d");
}

// Test: parsing of invalid versions ---------------------------------------------------------------

void TestVersionInfo::testParseInvalidVersions()
{
    QFETCH(QString, versionString);

    qDebug() << "versionString:" << versionString;
    QVERIFY(!VersionInfo(versionString).isValid());
}

void TestVersionInfo::testParseInvalidVersions_data()
{
    QTest::addColumn<QString>("versionString");

    int i = 0;

    QTest::addRow("%d", i++) << "0";
    QTest::addRow("%d", i++) << "9";

    QTest::addRow("%d", i++) << "0.";
    QTest::addRow("%d", i++) << "9.";

    QTest::addRow("%d", i++) << "0.0";
    QTest::addRow("%d", i++) << "9.9";

    QTest::addRow("%d", i++) << "00.00";
    QTest::addRow("%d", i++) << "99.99";

    QTest::addRow("%d", i++) << "0.0-a";
    QTest::addRow("%d", i++) << "9.9-z";

    QTest::addRow("%d", i++) << "0.0.";
    QTest::addRow("%d", i++) << "9.9.";

    QTest::addRow("%d", i++) << "a0";
    QTest::addRow("%d", i++) << "A9";
    QTest::addRow("%d", i++) << "Z0";
    QTest::addRow("%d", i++) << "Z9";

    QTest::addRow("%d", i++) << "0a";
    QTest::addRow("%d", i++) << "9A";
    QTest::addRow("%d", i++) << "0Z";
    QTest::addRow("%d", i++) << "9Z";

    QTest::addRow("%d", i++) << "0.a";
    QTest::addRow("%d", i++) << "0.A";
    QTest::addRow("%d", i++) << "0.z";
    QTest::addRow("%d", i++) << "0.Z";

    QTest::addRow("%d", i++) << "0.a0";
    QTest::addRow("%d", i++) << "0.A9";
    QTest::addRow("%d", i++) << "0.Z0";
    QTest::addRow("%d", i++) << "0.Z9";

    QTest::addRow("%d", i++) << "0a.0";
    QTest::addRow("%d", i++) << "9A.0";
    QTest::addRow("%d", i++) << "0Z.0";
    QTest::addRow("%d", i++) << "9Z.0";

    QTest::addRow("%d", i++) << "0.0.a";
    QTest::addRow("%d", i++) << "0.0.A";
    QTest::addRow("%d", i++) << "0.0.z";
    QTest::addRow("%d", i++) << "0.0.Z";

    QTest::addRow("%d", i++) << "0.0.0-";
    QTest::addRow("%d", i++) << "0.0.9-";
}

// Test: checking if version is valid --------------------------------------------------------------

void TestVersionInfo::testIsValid()
{
    QFETCH(VersionInfo, version);
    QFETCH(bool, result);

    qDebug() << "version:" << version.toString();
    QCOMPARE(version.isValid(), result);
}

void TestVersionInfo::testIsValid_data()
{
    QTest::addColumn<VersionInfo>("version");
    QTest::addColumn<bool>("result");

    int i = 0;

    // Valid versions
    QTest::addRow("%d", i++) << VersionInfo(0, 0, 0) << true;
    QTest::addRow("%d", i++) << VersionInfo(1, 2, 3) << true;
    QTest::addRow("%d", i++) << VersionInfo(1, 2, 3, "aSdF-._") << true;

    // Invalid versions
    QTest::addRow("%d", i++) << VersionInfo(-1, 0, 0) << false;
    QTest::addRow("%d", i++) << VersionInfo(0, -1, 0) << false;
    QTest::addRow("%d", i++) << VersionInfo(0, 0, -1) << false;
}

// Test: checking if version range is valid --------------------------------------------------------

void TestVersionInfo::testIsRangeValid()
{
    QFETCH(VersionInfo, minVersion);
    QFETCH(VersionInfo, maxVersion);
    QFETCH(bool, result);

    qDebug() << "minVersion:" << minVersion.toString();
    qDebug() << "maxVersion:" << maxVersion.toString();
    QCOMPARE(VersionInfo::isRangeValid(minVersion, maxVersion), result);
}

void TestVersionInfo::testIsRangeValid_data()
{
    QTest::addColumn<VersionInfo>("minVersion");
    QTest::addColumn<VersionInfo>("maxVersion");
    QTest::addColumn<bool>("result");

    int i = 0;

    // Valid and in range versions
    QTest::addRow("%d", i++) << VersionInfo(0, 0, 0) << VersionInfo(0, 0, 1) << true;
    QTest::addRow("%d", i++) << VersionInfo(0, 0, 0) << VersionInfo(0, 1, 0) << true;
    QTest::addRow("%d", i++) << VersionInfo(0, 0, 0) << VersionInfo(1, 0, 0) << true;

    QTest::addRow("%d", i++) << VersionInfo(1, 6, 0) << VersionInfo(2, 0, 0) << true;
    QTest::addRow("%d", i++) << VersionInfo(1, 6, 6) << VersionInfo(2, 0, 0) << true;

    QTest::addRow("%d", i++) << VersionInfo(0, 0, 0, "a") << VersionInfo(1, 0, 0, "b") << true;
    QTest::addRow("%d", i++) << VersionInfo(0, 0, 0, "a") << VersionInfo(1, 0, 0, "Z") << true;
    QTest::addRow("%d", i++) << VersionInfo(0, 0, 0, "0") << VersionInfo(1, 0, 0, "1") << true;

    QTest::addRow("%d", i++) << VersionInfo(0, 0, 0, "aa") << VersionInfo(1, 0, 0, "ab") << true;
    QTest::addRow("%d", i++) << VersionInfo(0, 0, 0, "10") << VersionInfo(1, 0, 0, "11") << true;

    QTest::addRow("%d", i++) << VersionInfo(0, 0, 0, "") << VersionInfo(1, 0, 0, "a") << true;
    QTest::addRow("%d", i++) << VersionInfo(0, 0, 0, "") << VersionInfo(1, 0, 0, "A") << true;
    QTest::addRow("%d", i++) << VersionInfo(0, 0, 0, "") << VersionInfo(1, 0, 0, "0") << true;

    // Valid and not in range versions
    QTest::addRow("%d", i++) << VersionInfo(0, 0, 0) << VersionInfo(0, 0, 0) << false;
    QTest::addRow("%d", i++) << VersionInfo(0, 0, 1) << VersionInfo(0, 0, 1) << false;
    QTest::addRow("%d", i++) << VersionInfo(0, 1, 0) << VersionInfo(0, 1, 0) << false;
    QTest::addRow("%d", i++) << VersionInfo(1, 0, 0) << VersionInfo(1, 0, 0) << false;

    QTest::addRow("%d", i++) << VersionInfo(0, 0, 1) << VersionInfo(0, 0, 0) << false;
    QTest::addRow("%d", i++) << VersionInfo(0, 1, 0) << VersionInfo(0, 0, 0) << false;
    QTest::addRow("%d", i++) << VersionInfo(1, 0, 0) << VersionInfo(0, 0, 0) << false;

    QTest::addRow("%d", i++) << VersionInfo(0, 0, 0, "a") << VersionInfo(0, 0, 0, "a") << false;
    QTest::addRow("%d", i++) << VersionInfo(0, 0, 0, "a") << VersionInfo(0, 0, 0, "A") << false;
    QTest::addRow("%d", i++) << VersionInfo(0, 0, 0, "0") << VersionInfo(0, 0, 0, "0") << false;

    QTest::addRow("%d", i++) << VersionInfo(0, 0, 0, "aa") << VersionInfo(0, 0, 0, "aa") << false;
    QTest::addRow("%d", i++) << VersionInfo(0, 0, 0, "10") << VersionInfo(0, 0, 0, "10") << false;

    // Invalid versions
    QTest::addRow("%d", i++) << VersionInfo() << VersionInfo(0, 0, 1) << false;
    QTest::addRow("%d", i++) << VersionInfo(0, 0, 1) << VersionInfo() << false;
}

// Test: checking if version is in the specified version range -------------------------------------

void TestVersionInfo::testIsVersionInRange()
{
    QFETCH(VersionInfo, version);
    QFETCH(VersionInfo, minVersion);
    QFETCH(VersionInfo, maxVersion);
    QFETCH(bool, result);

    qDebug() << "version:" << version.toString();
    qDebug() << "minVersion:" << minVersion.toString();
    qDebug() << "maxVersion:" << maxVersion.toString();
    QCOMPARE(VersionInfo::isVersionInRange(version, minVersion, maxVersion), result);
}

void TestVersionInfo::testIsVersionInRange_data()
{
    QTest::addColumn<VersionInfo>("version");
    QTest::addColumn<VersionInfo>("minVersion");
    QTest::addColumn<VersionInfo>("maxVersion");
    QTest::addColumn<bool>("result");

    int i = 0;

    // Valid results
    QTest::addRow("%d", i++) << VersionInfo(1, 0, 0)   << VersionInfo(1, 0, 0) << VersionInfo(2, 0, 0) << true;
    QTest::addRow("%d", i++) << VersionInfo(1, 9, 0)   << VersionInfo(1, 0, 0) << VersionInfo(2, 0, 0) << true;
    QTest::addRow("%d", i++) << VersionInfo(1, 9, 999) << VersionInfo(1, 0, 0) << VersionInfo(2, 0, 0) << true;

    QTest::addRow("%d", i++) << VersionInfo(1, 9, 0) << VersionInfo(1, 5, 0) << VersionInfo(2, 3, 0) << true;
    QTest::addRow("%d", i++) << VersionInfo(2, 2, 9) << VersionInfo(1, 5, 0) << VersionInfo(2, 3, 0) << true;

    QTest::addRow("%d", i++) << VersionInfo(1, 9, 0) << VersionInfo(1, 5, 8) << VersionInfo(2, 3, 1) << true;
    QTest::addRow("%d", i++) << VersionInfo(2, 2, 9) << VersionInfo(1, 5, 9) << VersionInfo(2, 3, 0) << true;

    QTest::addRow("%d", i++) << VersionInfo(1, 9, 6, "a") << VersionInfo(1, 9, 6, "a") << VersionInfo(1, 9, 6, "c") << true;
    QTest::addRow("%d", i++) << VersionInfo(1, 9, 6, "b") << VersionInfo(1, 9, 6, "a") << VersionInfo(1, 9, 6, "c") << true;
    QTest::addRow("%d", i++) << VersionInfo(1, 9, 6, "c") << VersionInfo(1, 9, 0, "a") << VersionInfo(1, 9, 9, "c") << true;

    // Invalid version
    QTest::addRow("%d", i++) << VersionInfo(1, 0, -1)   << VersionInfo(1, 0, 0) << VersionInfo(2, 0, 0) << false;
    QTest::addRow("%d", i++) << VersionInfo(1, 999, -1) << VersionInfo(1, 0, 0) << VersionInfo(2, 0, 0) << false;

    QTest::addRow("%d", i++) << VersionInfo(1, 0, 0) << VersionInfo(1, 0, -1) << VersionInfo(2, 0, 0)  << false;
    QTest::addRow("%d", i++) << VersionInfo(1, 9, 0) << VersionInfo(1, 0, 0)  << VersionInfo(2, 0, -1) << false;

    QTest::addRow("%d", i++) << VersionInfo(1, 9, -1, "a") << VersionInfo(1, 9, 0, "a") << VersionInfo(1, 9, 0, "c") << false;
    QTest::addRow("%d", i++) << VersionInfo(1, 9, -1, "b") << VersionInfo(1, 9, 0, "a") << VersionInfo(1, 9, 0, "c") << false;

    // Invalid results
    QTest::addRow("%d", i++) << VersionInfo(0, 9, 999) << VersionInfo(1, 0, 0) << VersionInfo(2, 0, 0) << false;
    QTest::addRow("%d", i++) << VersionInfo(2, 0, 0)   << VersionInfo(1, 0, 0) << VersionInfo(2, 0, 0) << false;

    QTest::addRow("%d", i++) << VersionInfo(1, 9, 6, "a")  << VersionInfo(1, 9, 6, "b") << VersionInfo(1, 9, 6, "c") << false;
    QTest::addRow("%d", i++) << VersionInfo(1, 9, 6, "d")  << VersionInfo(1, 9, 6, "b") << VersionInfo(1, 9, 6, "c") << false;
    QTest::addRow("%d", i++) << VersionInfo(1, 8, 9, "b")  << VersionInfo(1, 9, 0, "b") << VersionInfo(1, 9, 9, "c") << false;
    QTest::addRow("%d", i++) << VersionInfo(1, 9, 10, "a") << VersionInfo(1, 9, 0, "b") << VersionInfo(1, 9, 9, "c") << false;

    QTest::addRow("%d", i++) << VersionInfo(1, 9, 0, "9")  << VersionInfo(1, 9, 0, "10") << VersionInfo(1, 9, 0, "11") << false;
    QTest::addRow("%d", i++) << VersionInfo(1, 9, 0, "97") << VersionInfo(1, 9, 0, "98") << VersionInfo(1, 9, 0, "99") << false;
}

// Test: checking if version is in the specified version range -------------------------------------

void TestVersionInfo::testToString()
{
    QFETCH(VersionInfo, version);
    QFETCH(QString, result);

    qDebug() << "version:" << version.toString();
    QCOMPARE(version.toString(), result);
}

void TestVersionInfo::testToString_data()
{
    QTest::addColumn<VersionInfo>("version");
    QTest::addColumn<QString>("result");

    int i = 0;

    QTest::addRow("%d", i++) << VersionInfo(0, 0, 0) << "0.0.0";
    QTest::addRow("%d", i++) << VersionInfo(0, 0, 1) << "0.0.1";
    QTest::addRow("%d", i++) << VersionInfo(0, 1, 0) << "0.1.0";
    QTest::addRow("%d", i++) << VersionInfo(1, 0, 0) << "1.0.0";
    QTest::addRow("%d", i++) << VersionInfo(1, 2, 3) << "1.2.3";

    QTest::addRow("%d", i++) << VersionInfo(1, 0, 0, "a") << "1.0.0-a";
    QTest::addRow("%d", i++) << VersionInfo(1, 0, 0, "A") << "1.0.0-A";
    QTest::addRow("%d", i++) << VersionInfo(1, 0, 0, "z") << "1.0.0-z";
    QTest::addRow("%d", i++) << VersionInfo(1, 0, 0, "Z") << "1.0.0-Z";
    QTest::addRow("%d", i++) << VersionInfo(1, 0, 0, "0") << "1.0.0-0";
    QTest::addRow("%d", i++) << VersionInfo(1, 0, 0, "9") << "1.0.0-9";
    QTest::addRow("%d", i++) << VersionInfo(1, 0, 0, ".") << "1.0.0-.";
    QTest::addRow("%d", i++) << VersionInfo(1, 0, 0, "_") << "1.0.0-_";
    QTest::addRow("%d", i++) << VersionInfo(1, 0, 0, "-") << "1.0.0--";
    QTest::addRow("%d", i++) << VersionInfo(1, 0, 0, "#") << "1.0.0-#";

    QTest::addRow("%d", i++) << VersionInfo(1, 0, 0, "a$s97d-6fs_a8.6#7d") << "1.0.0-a$s97d-6fs_a8.6#7d";
}

// Main function -----------------------------------------------------------------------------------

QTEST_MAIN(TestVersionInfo)
#include "testVersionInfo.moc"
