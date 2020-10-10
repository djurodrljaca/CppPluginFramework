# Requirements


## [R1] Plugin metadata

Each plugin shall expose the following metadata:

* Instance Name
* Version
* Description
* Exported Interfaces

For *Instance Name* see [[R2] Plugin instance name](#r2-plugin-instance-name).

For *Version* see [[R3] Plugin version](#r3-plugin-version).

*Description* is an optional text that describes the purpose of the plugin and/or any other information about the plugin.

For *Exported Interfaces* see [[R4] Exported interfaces](#r4-exported-interfaces).


## [R2] Plugin instance name

Represents the name of plugin instance. Allowed characters are:

* All letters of the English alphabet (A-Z, a-z)
* Numbers (0-9)
* Period character (U+002E)
* Hyphen character (U+002D)
* Underscore character (U+005F)

The plugin instance name shall always start with a letter.


## [R3] Plugin version

Each plugin shall have a version which shall be in one of the following formats:

* `MAJOR.MINOR.PATCH`
* `MAJOR.MINOR.PATCH-DEV`


### Major version

`MAJOR` part of the version shall be a non-negative integer. This part of the version shall be incremented every time when there is a significant change in the architecture and/or design of the plugin.


### Minor version

`MINOR` part of the version shall be a non-negative integer. This part of the version shall be incremented every time when API, ABI, or behavior of the plugin is changed.


### Patch version

`PATCH` part of the version shall be a non-negative integer. This part of the version shall be incremented every time when a new release is made and the API, ABI, and (intended) behavior of the plugin is not changed.

This is generally meant for bug fix releases.


### Development version

`DEV` optional part of the version shall be a string that can be used for example to specify the type of and number of the release (`alpha3`, `beta1`, `RC5`, `bugfix-123`, `IntegrationTest-20180102-1` etc.). All characters are allowed.


## [R4] Exported interfaces

Each plugin shall export at least one interface. A plugin shall make it possible for its users to query if a specific interface is exported by the plugin.


## [R5] Plugin dependencies

A plugin shall optionally have dependencies to other plugins. The dependent plugins shall be injected to the plugin before it is started. A dependent plugin shall only be ejected from a plugin when both plugins are stopped.


## [R6] Plugin instance

A plugin shall enable creation of multiple instances of the same plugin, but only if they have different names!


## [R7] Configuration

Loading of plugins shall done according to its configuration which shall be stored in *CppConfigFramework* format.

The configuration shall consist of two parts:

* Plugin configurations
* Plugin startup priorities


### Plugin configuration

This part of the configuration shall be used to define which plugins need to be loaded and which instances of these plugins need to be created. It shall contain the following information for each plugin:

* Plugin file path
* Version requirements
* Comment
* Plugin instances:

    * Plugin instance name
    * Configuration
    * Comment (optional, it shall not be processed)
    * Plugin dependencies

*Plugin file path* shall point to plugin file (dynamic library). It shall be either an absolute or relative path and it shall be possible to reference environment variables.

*Version requirements* shall specify either a specific version or a version range of the plugin which needs to be loaded. For more details about versioning see [[R3] Plugin version](#r3-plugin-version).

*Comment* is optional and it shall not be processed. It can for example be used to explain the purpose of this plugin.

In case a version range is defined then the following formula shall be used:

```
min_version ≤ version < max_version
```


#### Plugin instances

A plugin shall configure at least one plugin instance to be created.

*Plugin instance name* shall define the name of the plugin instance. See [[R2] Plugin instance name](#r2-plugin-instance-name).

*Configuration* shall be optional for a plugin instance and it shall be stored in *CppConfigFramework* format.

*Comment* is optional and it shall not be processed. It can for example be used to explain the purpose of this plugin instance.

A plugin instance shall optionally define its dependencies to other plugins. *Plugin instance dependencies* shall contain a list of references (plugin instance names) to another plugin instances.


### Plugin startup priorities

This is an optional part of the configuration and it shall be used to define the order in which the referenced plugins (by plugin instance names) need to be started.

All unreferenced plugins shall be started after these plugins in no particular order.


## [R8] Plugin Startup

The following startup procedure shall be implemented:

1. To be able to start the plugins they shall first have to be loaded
2. For each plugin all of their plugin instances shall be created and configured
3. Only when all the plugin instances are created it shall be possible to inject the configured dependencies into the plugin instances
4. At this point the plugins can all be started in the configured startup order
5. The application shall now be able to use the plugins


## [R9] Plugin Shutdown

The following shutdown procedure shall be implemented:

1. Plugin instances shall be stopped in the reverse order as they were started
2. All dependencies shall be ejected
3. All plugin instances shall be destroyed
