# C++ Plugin Framework

The purpose of this plugin framework is to simplify creation and loading of C++ plugins.

The framework can be used to create cross-platform plugins, since it is using only Qt and STL libraries.

It is recommended to build the C++ plugin framework, the plugins and the application with the same development environment to avoid the problem of ABI incompatibility between compilers and used libraries.

## Build requirements

* CMake v3.5 or later
* C++14 compliant compiler
* Qt5 framework (only Core and Test modules)
* [Cedar Framework](https://github.com/djurodrljaca/CedarFramework)
* [C++ Config Framework](https://github.com/djurodrljaca/CppConfigFramework)

## Requirements

Requirements document can be found [here](docs/Requirements.md).

## Design

Design document can be found [here](docs/Design.md).