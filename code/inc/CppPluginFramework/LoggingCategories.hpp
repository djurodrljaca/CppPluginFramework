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
 * Contains logging categories
 */

#ifndef CPPPLUGINFRAMEWORK_LOGGINGCATEGORIES_HPP
#define CPPPLUGINFRAMEWORK_LOGGINGCATEGORIES_HPP

// C++ Plugin Framework includes
#include <CppPluginFramework/LibraryExport.hpp>

// Qt includes
#include <QtCore/QLoggingCategory>

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppPluginFramework
{

namespace LoggingCategory
{

//! Logging category for plugin config
CPPPLUGINFRAMEWORK_LIBRARY_EXPORT extern const QLoggingCategory Config;

//! Logging category for Plugin
CPPPLUGINFRAMEWORK_LIBRARY_EXPORT extern const QLoggingCategory Plugin;

//! Logging category for PluginManager
CPPPLUGINFRAMEWORK_LIBRARY_EXPORT extern const QLoggingCategory PluginManager;

} // namespace LoggingCategory

} // namespace CppPluginFramework

#endif // CPPPLUGINFRAMEWORK_LOGGINGCATEGORIES_HPP
