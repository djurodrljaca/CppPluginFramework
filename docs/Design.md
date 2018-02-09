# Design

The plugin framework has two main uses:

* Creating plugins
* Plugin management


## Plugins

All plugins must inherit from the `IPlugin` interface.

A simple base class `AbstractPlugin` is provided which implements the boilerplate part of a plugin.

![Plugin class diagram](ClassDiagrams/PluginClassDiagram.png  "Plugin class diagram")


### Dynamically linked library

A dynamically linked library must be created for each plugin. This library must  provide functions for:

* Reading the plugin version
* Creating an instance of the plugin

```cpp
const char *readPluginVersion();
CppPluginFramework::IPlugin *createPluginInstance(const QString &instanceName);
```


## Plugin Management

TODO