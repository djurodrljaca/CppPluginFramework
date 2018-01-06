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
 * Contains macros and declarations needed for creation and loading of plugins
 */

#ifndef CPPPLUGINFRAMEWORK_PLUGINSHAREDEXPORT_HPP
#define CPPPLUGINFRAMEWORK_PLUGINSHAREDEXPORT_HPP

// C++ Plugin Framework includes

// Qt includes
#include <QtCore/QString>

// System includes

// Forward declarations

namespace CppPluginFramework
{

class IPlugin;

}

// Macros

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

/*!
 * \def CPPPLUGINFRAMEWORK_PLUGIN_EXPORT
 *
 * Macro used for importing and exporting plugin libraries symbols
 *
 * \note    To successfully build a plugin library the symbol CPPPLUGINFRAMEWORK_BUILD_PLUGIN needs
 *          to be defined!
 */
#if defined(CPPPLUGINFRAMEWORK_BUILD_PLUGIN)
#  define CPPPLUGINFRAMEWORK_PLUGIN_EXPORT Q_DECL_EXPORT
#else
#  define CPPPLUGINFRAMEWORK_PLUGIN_EXPORT Q_DECL_IMPORT
#endif

extern "C"
{

/*!
 * Function prototype for reading the plugin version
 *
 * \return  Plugin version
 */
CPPPLUGINFRAMEWORK_PLUGIN_EXPORT const char *readPluginVersion();

/*!
 * Function prototype for creating a plugin instance
 *
 * \param   instanceName    Name of the plugin instance
 *
 * \return  Pointer to a plugin instance or a null pointer on failure
 */
CPPPLUGINFRAMEWORK_PLUGIN_EXPORT
CppPluginFramework::IPlugin *createPluginInstance(const QString &instanceName);

}

#endif // CPPPLUGINFRAMEWORK_PLUGINSHAREDEXPORT_HPP
