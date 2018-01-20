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
 * Contains the interface for the "test plugin 1"
 */

#ifndef CPPPLUGINFRAMEWORK_TESTPLUGINS_ITESTPLUGIN1_HPP
#define CPPPLUGINFRAMEWORK_TESTPLUGINS_ITESTPLUGIN1_HPP

// C++ Plugin Framework includes
#include <CppPluginFramework/PluginExport.hpp>

// Qt includes

// System includes

// Forward declarations

// Macros

namespace CppPluginFramework
{
namespace TestPlugins
{

class CPPPLUGINFRAMEWORK_PLUGIN_EXPORT ITestPlugin1
{
public:
    virtual ~ITestPlugin1() = default;
    virtual QString value() const = 0;
};

}
}

#endif // CPPPLUGINFRAMEWORK_TESTPLUGINS_ITESTPLUGIN1_HPP
