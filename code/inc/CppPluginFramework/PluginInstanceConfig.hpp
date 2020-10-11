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
 * Contains a config class for a Plugin Instance
 */

#pragma once

// C++ Plugin Framework includes
#include <CppPluginFramework/CppPluginFrameworkExport.hpp>

// C++ Config Framework includes
#include <CppConfigFramework/ConfigLoader.hpp>

// Qt includes

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppPluginFramework
{

//! Config class for a Plugin Instance
class CPPPLUGINFRAMEWORK_EXPORT PluginInstanceConfig : public CppConfigFramework::ConfigLoader
{
public:
    //! Constructor
    PluginInstanceConfig() = default;

    /*!
     * Constructor
     *
     * \param   name            Name of the plugin instance
     * \param   config          Plugin instance's config
     * \param   dependencies    List of plugin's dependencies
     */
    PluginInstanceConfig(const QString &name,
                         const CppConfigFramework::ConfigObjectNode &config = {},
                         const QSet<QString> &dependencies = {});

    /*!
     * Copy constructor
     *
     * \param   other   Instance to copy
     */
    PluginInstanceConfig(const PluginInstanceConfig &other);

    /*!
     * Move constructor
     *
     * \param   other   Instance to move
     */
    PluginInstanceConfig(PluginInstanceConfig &&other) noexcept = default;

    //! Destructor
    ~PluginInstanceConfig() override = default;

    /*!
     * Copy assignment operator
     *
     * \param   other   Instance to copy assign
     *
     * \return  Reference to this instance after the assignment is made
     */
    PluginInstanceConfig &operator=(const PluginInstanceConfig &other);

    /*!
     * Move assignment operator
     *
     * \param   other   Instance to move assign
     *
     * \return  Reference to this instance after the assignment is made
     */
    PluginInstanceConfig &operator=(PluginInstanceConfig &&other) noexcept = default;

    /*!
     * Checks if plugin instance config is valid
     *
     * \retval  true    Plugin instance config is valid
     * \retval  false   Plugin instance config is not valid
     */
    bool isValid() const;

    /*!
     * Returns plugin instance's name
     *
     * \return  Plugin instance's name
     */
    QString name() const;

    /*!
     * Sets plugin instance's name
     *
     * \param   name    Plugin instance's name
     */
    void setName(const QString &name);

    /*!
     * Returns the plugin instance's config
     *
     * \return  Plugin instance's config
     */
    const CppConfigFramework::ConfigObjectNode &config() const;

    /*!
     * Sets the plugin instance's config
     *
     * \param   config  Plugin instance's config
     */
    void setConfig(const CppConfigFramework::ConfigObjectNode &config);

    /*!
     * Returns list of plugin instance's dependencies
     *
     * \return  List of plugin instance's dependencies
     */
    QSet<QString> dependencies() const;

    /*!
     * Sets list of plugin instance's dependencies
     *
     * \param   dependencies    List of plugin instance's dependencies
     */
    void setDependencies(const QSet<QString> &dependencies);

private:
    //! \copydoc    CppConfigFramework::ConfigLoader::loadConfigParameters()
    bool loadConfigParameters(const CppConfigFramework::ConfigObjectNode &config) override;

    //! \copydoc    CppConfigFramework::ConfigLoader::validateConfig()
    QString validateConfig() const override;

private:
    //! Holds the plugin instance's name
    QString m_name;

    //! Holds the (optional) plugin instance's config node
    CppConfigFramework::ConfigObjectNode m_config;

    //! Holds the list of plugin instance's dependencies
    QSet<QString> m_dependencies;
};

} // namespace CppPluginFramework

// -------------------------------------------------------------------------------------------------

/*!
 * Global "equal to" operator for CppPluginFramework::PluginInstanceConfig
 *
 * \param   left    Instance config
 * \param   right   Instance config
 *
 * \retval  true    Instance configs are equal
 * \retval  false   Instance configs are not equal
 */
CPPPLUGINFRAMEWORK_EXPORT bool operator==(const CppPluginFramework::PluginInstanceConfig &left,
                                          const CppPluginFramework::PluginInstanceConfig &right);

// -------------------------------------------------------------------------------------------------

/*!
 * Global "not equal to" operator for CppPluginFramework::PluginInstanceConfig
 *
 * \param   left    Instance config
 * \param   right   Instance config
 *
 * \retval  true    Instance configs are not equal
 * \retval  false   Instance configs are equal
 */
CPPPLUGINFRAMEWORK_EXPORT bool operator!=(const CppPluginFramework::PluginInstanceConfig &left,
                                          const CppPluginFramework::PluginInstanceConfig &right);
