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
 * Contains macros needed for building C++ Plugin Framework library
 */

#ifndef CPPPLUGINFRAMEWORK_LIBRARYEXPORT_HPP
#define CPPPLUGINFRAMEWORK_LIBRARYEXPORT_HPP

// C++ Plugin Framework includes

// Qt includes
#include <QtCore/QtGlobal>

// System includes

// Forward declarations

// Macros

#if defined(CPPPLUGINFRAMEWORK_BUILD_LIBRARY) && defined(CPPPLUGINFRAMEWORK_BUILD_PLUGIN)
#  error    You are only allowed to define one of the symbols: CPPPLUGINFRAMEWORK_BUILD_LIBRARY or CPPPLUGINFRAMEWORK_BUILD_PLUGIN
#endif

/*!
 * \def CPPPLUGINFRAMEWORK_EXPORT
 *
 * Macro used for importing and exporting the C++ Plugin Framework library symbols
 *
 * \note    To successfully build the C++ Plugin Framework library the symbol
 *          CPPPLUGINFRAMEWORK_BUILD_LIBRARY needs to be defined!
 */
#if defined(CPPPLUGINFRAMEWORK_BUILD_LIBRARY)
#  define CPPPLUGINFRAMEWORK_LIBRARY_EXPORT Q_DECL_EXPORT
#else
#  define CPPPLUGINFRAMEWORK_LIBRARY_EXPORT Q_DECL_IMPORT
#endif

#endif // CPPPLUGINFRAMEWORK_LIBRARYEXPORT_HPP
