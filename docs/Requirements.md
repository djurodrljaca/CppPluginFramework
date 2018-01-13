# Requirements


## [R1] Plugin Metadata

Each plugin shall expose the following metadata:

* Instance Name
* Description
* Version
* Exported Interfaces

For *Instance Name* see [[R2] Plugin Instance Name](#r2-plugin-instance-name).

*Description* is an optional text that describes the purpose of the plugin and/or any other information about the plugin.

For *Version* see [[R3] Plugin Version](#r3-plugin-version).

For *Exported Interfaces* see [[R4] Exported Interfaces](#r4-exported-interfaces).


## [R2] Plugin Instance Name

Represents the name of plugin instance. Allowed characters are:

* All letters of the English alphabet (A-Z, a-z)
* Numbers (0-9)
* Period character (U+002E)
* Hyphen character (U+002D)
* Underscore character (U+005F)

The plugin instance name shall always start with a letter.


## [R3] Plugin Version

Each plugin shall have a version which shall be in one of the following formats:

* `MAJOR.MINOR.PATCH`
* `MAJOR.MINOR.PATCH-DEV`


### Major Version

`MAJOR` part of the version shall be a non-negative integer. This part of the version shall be incremented every time when there is a significant change in the architecture and/or design of the plugin.


### Minor Version

`MINOR` part of the version shall be a non-negative integer. This part of the version shall be incremented every time when API, ABI, or behavior of the plugin is changed.


### Patch Version

`PATCH` part of the version shall be a non-negative integer. This part of the version shall be incremented every time when a new release is made and the API, ABI, and (intended) behavior of the plugin is not changed.

This is generally meant for bug fix releases.


### Development Version

`DEV` optional part of the version shall be a string that can be used for example to specify the type of and number of the release (`alpha3`, `beta1`, `RC5`, `bugfix-123`, `IntegrationTest-20180102-1` etc.). All characters are allowed.


## [R4] Exported Interfaces

Each plugin shall export at least one of interface. A plugin shall make it possible for its users to query if a specific interface is exported by the plugin.


## [R5] Plugin Dependencies

A plugin shall optionally have dependencies to other plugins. The dependent plugins shall be injected to the plugin before it is started. A dependent plugin shall only be ejected from a plugin when both plugins are stopped.


## [R6] Plugin Instance

A plugin shall enable creation of multiple instances of the same plugin, but only if they have different names!


## [R7] Configuration

Loading of plugins shall done according to its configuration which shall be stored in JSON format. It shall contain the following information:

* List of custom environment variables

    * Name
    * Value

* List of plugins

    * Plugin file path
    * Version requirements
    * Comment
    * List of plugin instances
    
        * Plugin instance name
        * Configuration
        * Comment
        * Plugin dependencies
    
            * Plugin instance name
            * Comment


### [R7.1] Custom Environment Variables

Specifying a list of custom environment variables is an optional part of the configuration and it can be used to make the configuration more readable and more practical to write. For example it can be used for specifying the plugin and configuration file paths.

Allowed characters for the environment variable names are:

* All letters of the English alphabet (A-Z, a-z)
* Numbers (0-9)
* Underscore character (U+005F)

The name shall only start with a letter or underscore!

The value can be any string and it can also contain references to system environment variables and other custom environment variables defined in this list.

Format for referencing a system environment variables or the ones from this list shall be: `${VARIABLE_NAME}`


### [R7.2] Plugin Configuration

This part of the configuration shall be used to specify which plugins need to be loaded and which instances need to be created.

*Plugin file path* shall point to plugin file (dynamic library). It shall be either an absolute or relative path and it shall be possible to reference any system or custom environment variable.

*Version requirements* shall specify either a specific version or a version range of the plugin which needs to be loaded. For more details about versioning see [[R3] Plugin Version](#r3-plugin-version).

*Comment* is optional and it can for example be used to explain the purpose of this plugin to the reader of the configuration file.

In case a version range is defined then the following formula shall be used: `min_version ≤ version < max_version`


#### List of plugin instances

A plugin shall define at least one plugin instance to be created.

*Plugin name* shall define the name of the plugin instance. See [[R2] Plugin Instance Name](#r2-plugin-instance-name).

*Configuration* is optional and if it is needed it shall either point to a configuration file for this plugin instance or it shall contain the configuration in JSON format. The configuration file path shall be either an absolute or relative path and it shall be possible to reference any system or custom environment variable.

*Comment* is optional and it can for example be used to explain the purpose of this plugin instance to the reader of the configuration file.


##### Plugin instance dependencies

A plugin instance shall optionally define its dependencies to other plugins.

*Plugin instance name* shall reference another plugin's instance name.

*Comment* is optional and it can for example be used to explain the purpose of this dependency to the reader of the configuration file.


### [R7.3] Relative Paths

All relative paths referenced in the configuration shall be relative to a *Working Directory* specified by the user of the library.

Here are a for examples of *Working Directories* that an application could specify:

* Directory where the application's executable is located
* Directory where the configuration file is located
* User's *home* directory


## [R8] Plugin Startup

The following startup procedure shall be implemented:

1. Plugins (dynamic libraries) shall be loaded
2. Plugin instances shall created
3. Plugin instances shall shall be configured
4. Dependencies shall be injected to the plugin instances
5. Plugin instances shall be started in the same order as listed in the configuration file
6. Application shall now be able to use the plugins


## [R9] Plugin Shutdown

The following shutdown procedure shall be implemented:

1. Plugin instances shall be stopped in the reverse order as listed in the configuration file
2. All dependencies shall be ejected
3. All plugin instances shall be destroyed
4. All plugins shall be unloaded
