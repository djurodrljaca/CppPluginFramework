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
 * Contains a base class for a factory that shall be able to create instances of a plugin
 */

#pragma once

// C++ Plugin Framework includes
#include <CppPluginFramework/IPlugin.hpp>

// Qt includes

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppPluginFramework
{

//! This is a base class for a factory that shall be able to create instances of a plugin
class CPPPLUGINFRAMEWORK_EXPORT IPluginFactory
{
public:
    //! Destructor
    virtual ~IPluginFactory() = default;

    /*!
     * Creates a plugin instance
     *
     * \param   instanceName    Name of the plugin instance
     *
     * \return  Plugin instance or nullptr if the instance could not be created
     */
    virtual std::unique_ptr<IPlugin> createInstance(const QString &instanceName) const = 0;
};

} // namespace CppPluginFramework

//! Maps the class name to its string equivalent
Q_DECLARE_INTERFACE(CppPluginFramework::IPluginFactory, "CppPluginFramework::IPluginFactory")
