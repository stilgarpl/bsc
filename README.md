# BSC::Project

BSC Project aim is to create a set of tools to backup and sync files and configurations across multiple computers.

More information is in pages for the specific modules

*This project is under active development and code can change without warning. It doesn't have a stable release yet. Modules that are not
listed below are not usable*

## List of modules

* [parser](src/parser/README.md)
* [properties](src/properties/README.md)
* [testaid](src/testaid/README.md)

# Building instructions:

## Building with conan package manager

```shell
mkdir -p build
cd build
cmake .. -DBSC_USE_PACKAGE_MANAGER=ON
make 
make install
```

## Building without package managers (all dependencies must be installed in OS)

```shell
mkdir -p build
cd build
cmake .. 
make 
make install
```