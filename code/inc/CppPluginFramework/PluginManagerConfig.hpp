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
 * Contains a config class for the Plugin Manager
 */

#ifndef CPPPLUGINFRAMEWORK_PLUGINMANAGERCONFIG_HPP
#define CPPPLUGINFRAMEWORK_PLUGINMANAGERCONFIG_HPP

// C++ Plugin Framework includes
#include <CppPluginFramework/EnvironmentVariables.hpp>
#include <CppPluginFramework/PluginConfig.hpp>

// Qt includes
#include <QtCore/QJsonObject>

// System includes
#include <memory>

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppPluginFramework
{

//! Config class for the Plugin Manager
class CPPPLUGINFRAMEWORK_LIBRARY_EXPORT PluginManagerConfig
        : public CppConfigFramework::ConfigLoader
{
public:
    //! Constructor
    PluginManagerConfig() = default;

    /*!
     * Copy constructor
     *
     * \param   other   Instance to copy
     */
    PluginManagerConfig(const PluginManagerConfig &other) = default;

    /*!
     * Move constructor
     *
     * \param   other   Instance to move
     */
    PluginManagerConfig(PluginManagerConfig &&other) noexcept = default;

    //! Destructor
    ~PluginManagerConfig() override = default;

    /*!
     * Copy assignment operator
     *
     * \param   other   Instance to copy assign
     *
     * \return  Reference to this instance after the assignment is made
     */
    PluginManagerConfig &operator=(const PluginManagerConfig &other) = default;

    /*!
     * Move assignment operator
     *
     * \param   other   Instance to move assign
     *
     * \return  Reference to this instance after the assignment is made
     */
    PluginManagerConfig &operator=(PluginManagerConfig &&other) noexcept = default;

    /*!
     * Checks if plugin config is valid
     *
     * \retval  true    Plugin config is valid
     * \retval  false   Plugin config is not valid
     */
    bool isValid() const;

    /*!
     * Gets plugin configs that were read from the config file
     *
     * \return  Plugin configs
     */
    const QList<PluginConfig> &pluginConfigs() const;

    /*!
     * Sets list of plugin configs
     *
     * \param   pluginConfigs   List of plugin configs
     */
    void setPluginConfigs(const QList<PluginConfig> &pluginConfigs);

private:
    //! \copydoc    CppConfigFramework::ConfigLoader::loadConfigParameters()
    bool loadConfigParameters(const CppConfigFramework::ConfigObjectNode &config,
                              QString *error) override;

    //! \copydoc    CppConfigFramework::ConfigLoader::validateConfig()
    QString validateConfig() const override;

private:
    //! Holds the plugin configs
    QList<PluginConfig> m_pluginConfigs;
};

} // namespace CppPluginFramework

/*!
 * Global "equal to" operator for CppPluginFramework::PluginManagerConfig
 *
 * \param   left    Plugin config
 * \param   right   Plugin config
 *
 * \retval  true    Plugin configs are equal
 * \retval  false   Plugin configs are not equal
 */
CPPPLUGINFRAMEWORK_LIBRARY_EXPORT bool operator==(
        const CppPluginFramework::PluginManagerConfig &left,
        const CppPluginFramework::PluginManagerConfig &right);

/*!
 * Global "not equal to" operator for CppPluginFramework::PluginManagerConfig
 *
 * \param   left    Plugin config
 * \param   right   Plugin config
 *
 * \retval  true    Plugin configs are not equal
 * \retval  false   Plugin configs are equal
 */
CPPPLUGINFRAMEWORK_LIBRARY_EXPORT bool operator!=(
        const CppPluginFramework::PluginManagerConfig &left,
        const CppPluginFramework::PluginManagerConfig &right);

#endif // CPPPLUGINFRAMEWORK_PLUGINMANAGERCONFIG_HPP
