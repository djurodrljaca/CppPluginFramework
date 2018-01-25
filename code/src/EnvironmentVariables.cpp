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
 * Contains a class for accessing environment variables
 */

// C++ Plugin Framework includes
#include <CppPluginFramework/EnvironmentVariables.hpp>

// Qt includes
#include <QtCore/QProcessEnvironment>
#include <QtCore/QRegularExpression>
#include <QtCore/QtDebug>

// System includes

// Forward declarations

// Macros

namespace CppPluginFramework
{

// -------------------------------------------------------------------------------------------------
// Implementation
// -------------------------------------------------------------------------------------------------

/*!
 * Private implementation struct
 */
struct EnvironmentVariables::Impl
{
    /*!
     * Holds the local environment variables
     */
    QHash<QString, QString> m_variables;
};

// -------------------------------------------------------------------------------------------------
// API
// -------------------------------------------------------------------------------------------------

EnvironmentVariables::EnvironmentVariables()
    : m_impl(std::make_unique<EnvironmentVariables::Impl>())
{
    reset();
}

EnvironmentVariables::EnvironmentVariables(const EnvironmentVariables &other)
    : m_impl(std::make_unique<EnvironmentVariables::Impl>(*(other.m_impl)))
{
}

EnvironmentVariables::~EnvironmentVariables()
{
}

EnvironmentVariables &EnvironmentVariables::operator=(EnvironmentVariables rhs)
{
    m_impl = std::move(rhs.m_impl);

    return *this;
}

void EnvironmentVariables::reset()
{
    const QProcessEnvironment systemEnvironment = QProcessEnvironment::systemEnvironment();
    m_impl->m_variables.clear();

    for (const QString &name : systemEnvironment.keys())
    {
        m_impl->m_variables[name] = systemEnvironment.value(name);
    }
}

bool EnvironmentVariables::contains(const QString &name) const
{
    return m_impl->m_variables.contains(name);
}

QString EnvironmentVariables::value(const QString &name) const
{
    return m_impl->m_variables[name];
}

void EnvironmentVariables::setValue(const QString &name, const QString &value)
{
    if (!name.isEmpty())
    {
        m_impl->m_variables[name] = value;
    }
}

QString EnvironmentVariables::expandText(const QString &text) const
{
    // Set the initial value
    QString expandedText = text;

    // Expand environment text in a loop in case of "nested" environment variables (max 100 cycles)
    QRegularExpression regex("\\${([a-zA-Z0-9_]+)}");

    for (int i = 0; i < 100; i++)
    {
        // Find all environment variable references
        auto it = regex.globalMatch(expandedText);

        if (it.hasNext())
        {
            // Replace all environment variable references with their value
            do
            {
                auto match = it.next();

                const QString reference = match.captured(0);
                const QString referenceName = match.captured(1);
                const QString expandedReferenceValue = value(referenceName);

                expandedText.replace(reference, expandedReferenceValue);
            }
            while (it.hasNext());
        }
        else
        {
            // No environment variable references were found, exit loop
            break;
        }
    }

    // Check if all environment variable references were replaced with their value
    if (regex.match(expandedText).hasMatch())
    {
        expandedText.clear();
        qDebug() << "EnvironmentVariables::expandText: Error: unable to fully expand text:" << text;
    }

    return expandedText;
}

}
