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
 * Contains helper macros for logging
 */

#ifndef CPPPLUGINFRAMEWORK_LOGHELPER_HPP
#define CPPPLUGINFRAMEWORK_LOGHELPER_HPP

// C++ Plugin Framework includes

// Qt includes

// System includes

// Forward declarations

// Macros
#define CPPPLUGINFRAMEWORK_STRINGIFY(PARAM)     #PARAM
#define CPPPLUGINFRAMEWORK_TO_STRING(PARAM)     CPPPLUGINFRAMEWORK_STRINGIFY(PARAM)
#define CPPPLUGINFRAMEWORK_LOG_METHOD(METHOD)   ("CppPluginFramework::" METHOD ":" CPPPLUGINFRAMEWORK_TO_STRING(__LINE__) ":")

#endif // CPPPLUGINFRAMEWORK_LOGHELPER_HPP
