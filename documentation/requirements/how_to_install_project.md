# How to use the project

## Installation

Clone this repository and install the dependencies using the following command:

```bash
#On linux
cd scripts
chmod +x requirements.sh
./requirements.sh
cd ..
#On windows
#You don't need to do anything, the dependencies are already included in the following command :
$ cmake --preset windows-static
```

This will add the necessary dependencies mostly for SFML, which on Linux, is not shipping the necessary libraries by default.

## Instructions

To install the vcpkg dependencies, you can use the following command:

```bash
#On linux
cmake --preset linux
#On windows
cmake --preset windows-static
```

To build the project, you can use the following command:

```bash
cd build
cmake .. && cmake --build .
```

This will compile everything and create the executable in the build folder.