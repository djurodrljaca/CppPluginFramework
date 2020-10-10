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
* Qt5 framework (only Core and Test modules)
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

```
find_package(CppPluginFramework REQUIRED)
```

And link it to your target:

```
target_link_libraries(target_name PUBLIC CppPluginFramework::CppPluginFramework)
```

TODO: add chapters for how to create and use plugins