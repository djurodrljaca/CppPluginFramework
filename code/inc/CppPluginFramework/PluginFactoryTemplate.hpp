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
 * Contains a template class for a factory that shall be able to create instances of a plugin
 */

#ifndef CPPPLUGINFRAMEWORK_PLUGINFACTORYTEMPLATE_HPP
#define CPPPLUGINFRAMEWORK_PLUGINFACTORYTEMPLATE_HPP

// C++ Plugin Framework includes
#include <CppPluginFramework/IPluginFactory.hpp>

// Qt includes
#include <QtCore/QtPlugin>

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppPluginFramework
{

/*!
 * This is a template class for a factory that shall be able to create instances of a plugin
 *
 * \tparam  T   Plugin class
 *
 * To create a plugin the following code needs to be put into the header file, preferably directly
 * after the declaration of the plugin class:
 *
 * \code{.cpp}
 *
 *     class PLUGIN_CLASS_EXPORT PluginFactory :
 *             public QObject,
 *             public CppPluginFramework::PluginFactoryTemplate<PLUGIN_CLASS>
 *     {
 *         Q_OBJECT
 *         Q_PLUGIN_METADATA(IID "CppPluginFramework::IPluginFactory")
 *         Q_INTERFACES(CppPluginFramework::IPluginFactory)
 *
 *     public:
 *         ~PluginFactory() override = default;
 *     };
 *
 * \endcode
 *
 * The following placeholders need to be replaced:
 * - PLUGIN_CLASS_EXPORT: replace it with the "export macro" for that plugin
 * - PLUGIN_CLASS: replace it with the plugin class name
 */
template<typename T, std::enable_if_t<std::is_base_of<IPlugin, T>::value, bool> = true>
class PluginFactoryTemplate : public IPluginFactory
{
public:
    //! Destructor
    ~PluginFactoryTemplate() override = default;

    //! \copydoc    IPluginFactory::createPluginInstance()
    std::unique_ptr<IPlugin> createInstance(const QString &instanceName) const override
    {
        return std::make_unique<T>(instanceName);
    }
};

} // namespace CppPluginFramework

#endif // CPPPLUGINFRAMEWORK_PLUGINFACTORYTEMPLATE_HPP
