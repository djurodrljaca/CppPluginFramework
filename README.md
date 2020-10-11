# C++ Plugin Framework

The purpose of this plugin framework is to simplify creation and loading of C++ plugins.

The framework can be used to create cross-platform plugins, since it is using only Qt and STL libraries.

It is recommended to build the C++ plugin framework, the plugins and the application with the same development environment to avoid the problem of ABI incompatibility between compilers and used libraries.


## Requirements

Requirements document can be found [here](docs/Requirements.md).


## Design

Design document can be found [here](docs/Design.md).


## Build and deployment

For building the following requirements are needed:

* CMake v3.5 or later
* C++14 compliant compiler
* Qt v5.9 or later (only Core and Test modules)
* [Cedar Framework](https://github.com/djurodrljaca/CedarFramework)
* [C++ Config Framework](https://github.com/djurodrljaca/CppConfigFramework)

If the library needs to be deployed to a custom location you need to set the `CMAKE_INSTALL_PREFIX` variable or if you need to use a custom Qt installation, *CedarFramework*, or *CppConfigFramework*  is not deployed to one of the standard locations you need to set the `CMAKE_PREFIX_PATH` variable.

Example:

```
$ cmake -DCMAKE_INSTALL_PREFIX=path/to/install/dir -DCMAKE_PREFIX_PATH="path/to/qt/dir;path/to/CedarFramework/dir;path/to/CppConfigFramework/dir" path/to/source/dir
```

Then build and deploy the library:

```
$ cmake --build . --target install
```


## Usage

### CMake Integration

To use this module in a CMake project you need to have a built and deployed *CedarFramework* and *CppConfigFramework* dependencies. Then build and deploy *CppPluginFramework* and add this to your project's CMakeLists.txt:

```CMake
find_package(CppPluginFramework REQUIRED)
```

And link it to your target:

```CMake
target_link_libraries(target_name PUBLIC CppPluginFramework::CppPluginFramework)
```


### Creating an interface for the plugin

When creating a plugin it is recommended to split the actual plugin and the interfaces that will be implemented by it to separate targets so that other plugins can depend just on the interfaces (header files) instead of to the plugins.

```CMake
# File: CMakeLists.txt

add_library(ExampleInterface INTERFACE)

target_link_libraries(ExampleInterface INTERFACE ${LIBS})

target_include_directories(ExampleInterface INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
        $<INSTALL_INTERFACE:include>
    )

# Create dummy target so that IDEs show the header files
add_custom_target(ExampleInterface_IDE SOURCES
        inc/IExample.hpp
    )
```

```C++
// File: IExample.hpp

class IExample
{
public:
    virtual ~IExample() = default;
    virtual bool exampleMethod() const = 0;
};
```


### Creating a plugin

When the interface targets are available then the plugin can just add them as the target dependencies.

```CMake
# File: CMakeLists.txt

add_library(ExamplePlugin SHARED ${PARAM_SOURCES})

target_link_libraries(ExamplePlugin PUBLIC
        CppPluginFramework::CppPluginFramework
        ExampleInterface
    )

# Needed only if custom prefix and/or suffix are needed for the plugin library
set_target_properties(ExamplePlugin PROPERTIES
        PREFIX ""
        SUFFIX ".plugin"
    )
```

```C++
// File: ExamplePlugin.hpp

class ExamplePlugin : public CppPluginFramework::AbstractPlugin, public IExample
{
public:
    ExamplePlugin(const QString &name);
    ~ExamplePlugin() override = default;

    bool loadConfig(const CppConfigFramework::ConfigObjectNode &config) override;
    bool injectDependency(CppPluginFramework::IPlugin *plugin) override;
    void ejectDependencies() override;

    bool exampleMethod() const override;
};

class Q_DECL_EXPORT ExamplePluginFactory :
        public QObject,
        public CppPluginFramework::PluginFactoryTemplate<ExamplePlugin>
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "CppPluginFramework::IPluginFactory")
    Q_INTERFACES(CppPluginFramework::IPluginFactory)

public:
    ~ExamplePluginFactory() override = default;
};
```

```C++
// File: ExamplePlugin.cpp

ExamplePlugin::ExamplePlugin(const QString &name)
    : CppPluginFramework::AbstractPlugin(name,
                                         CppPluginFramework::VersionInfo(1, 0, 0),
                                         "Example plugin",
                                         { "IExample" }),
      IExample()
{
}

bool ExamplePlugin::loadConfig(const CppConfigFramework::ConfigObjectNode &config)
{
    return true;
}

bool ExamplePlugin::injectDependency(CppPluginFramework::IPlugin *plugin)
{
    return false;
}

void ExamplePlugin::ejectDependencies()
{
}

bool ExamplePlugin::exampleMethod() const
{
    return true;
}
```


### Using a plugin in an application

To be able to use a plugin you only need access to the *CppPluginFramework* (and its dependencies) and the plugin's interfaces.

The load the plugins you need to prepare a configuration and load it with the *PluginManager* class.

```json
{
    "config":
    {
        "plugins":
        {
            "example":
            {
                "$file_path": "${PluginPath}/ExamplePlugin.plugin",
                "version": "1.0.0",
                "instances":
                {
                    "example":
                    {
                        "name": "example_instance"
                    },
                }
            }
        }
    }
}
```

```C++
ConfigReader configReader;
EnvironmentVariables environmentVariables;

auto config = configReader.read("path/to/config/file",
                                QDir(QCoreApplication::applicationDirPath()),
                                ConfigNodePath::ROOT_PATH,
                                ConfigNodePath::ROOT_PATH,
                                {},
                                &environmentVariables);

PluginManagerConfig pluginManagerConfig;
pluginManagerConfig.loadConfig(*config);

PluginManager pluginManager;
pluginManager.load(pluginManagerConfig);
pluginManager.start();

auto example = pluginManager.pluginInstance("example_instance")
bool result = example->exampleMethod();

pluginManager.stop();
pluginManager.unload();
```
