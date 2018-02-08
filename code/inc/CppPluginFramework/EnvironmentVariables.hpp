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

#ifndef CPPPLUGINFRAMEWORK_ENVIRONMENTVARIABLES_HPP
#define CPPPLUGINFRAMEWORK_ENVIRONMENTVARIABLES_HPP

// C++ Plugin Framework includes
#include <CppPluginFramework/LibraryExport.hpp>

// Qt includes

// System includes
#include <memory>

// Forward declarations

// Macros

namespace CppPluginFramework
{

/*!
 * This class gives access to system and local environment variables. It can be used for accessing
 * the environment variable values and for expanding environment variable references in a string.
 *
 * System environment variables are read when an instance of this class is created and they are used
 * as the initial environment variables.
 *
 * \note    If a system environment variable is updated after that then that change will not be
 *          applied to this class!
 *
 * If an attempt is made to set an environment variable that does not exist then a new variable is
 * created.
 */
class CPPPLUGINFRAMEWORK_LIBRARY_EXPORT EnvironmentVariables
{
public:
    /*!
     * Constructor
     */
    EnvironmentVariables();

    /*!
     * Copy constructor
     *
     * \param   other   Object to copy
     */
    EnvironmentVariables(const EnvironmentVariables &other);

    /*!
     * Destructor
     */
    ~EnvironmentVariables();

    /*!
     * Assignment operator
     *
     * \param   rhs     Object to copy
     */
    EnvironmentVariables &operator=(EnvironmentVariables rhs);

    /*!
     * Resets the environment variables
     *
     * The stored environment variables are all cleared and replaced with the current state of the
     * system environment variables.
     */
    void reset();

    /*!
     * Checks if an environment variable with the specified name can be found
     *
     * \param   name    Environment variable name
     *
     * \retval  true    Found
     * \retval  false   Not found
     */
    bool contains(const QString &name) const;

    /*!
     * Reads the environment variable value
     *
     * \param   name    Environment variable name
     *
     * \return  Environment variable value or an empty string if the variable cannot be found
     */
    QString value(const QString &name) const;

    /*!
     * Sets the environment variable value
     *
     * \param   name    Environment variable name
     * \param   value   Value to set
     */
    void setValue(const QString &name, const QString &value);

    /*!
     * Expands all references to environment variables in the text
     *
     * \param   text    Text to extend
     *
     * \return  Extended text or an empty string if it was not able to fully extend the text (for
     *          example if it contained too many nested or cyclic environment variable references)
     */
    QString expandText(const QString &text) const;

    /*!
     * Convenience overload that converts the input UTF8 encoded byte array to a string, expands the
     * text and then converts it to a UTF8 encoded byte array
     *
     * \param   text    Text to extend
     */
    QByteArray expandText(const QByteArray &text) const;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

}

#endif // CPPPLUGINFRAMEWORK_ENVIRONMENTVARIABLES_HPP
