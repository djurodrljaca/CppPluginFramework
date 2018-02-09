# Design

The plugin framework has two main uses:

* Creating plugins
* Plugin management


## Plugins

All plugins must inherit from the `IPlugin` interface.

A simple base class `AbstractPlugin` is provided which implements the boilerplate part of a plugin.

![Plugin class diagram](ClassDiagrams/PluginClassDiagram.png  "Plugin class diagram")


### Plugin Implementation

Most plugins should be able to use `AbstractPlugin` base class. In this case only the following needs to be implemented:

* Loading the plugin's configuration (optional feature)
* Management of plugin's dependencies (optional feature)
* Handling of plugin startup and shutdown procedures (for some plugins the default implementation might be good enough)
* Exported interfaces (one or more)


### Dynamically Linked Library

A dynamically linked library must be created for each plugin. This library must  provide functions for:

* Reading the plugin version
* Creating an instance of the plugin

```cpp
const char *readPluginVersion();
CppPluginFramework::IPlugin *createPluginInstance(const QString &instanceName);
```


## Plugin Management

TODO