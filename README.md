# R-type project

## Team members 🤝

- [Quentin Duval](quentin1.duval@epitech.eu) 👑
- [Ewan Massonet](ewan.massonet@epitech.eu)
- [Alan Trebugeais](alan.trebugeais@epitech.eu)
- [Thibaut Tavernier](thibaut1.tavernier@epitech.eu)
- [Yann Malaret](yann.malaret@epitech.eu)

Duration : 7 weeks.

## Description 📝

This project is a multiplayer game engine, running games like R-TYPE.

## Technologies used 🖥

- Frontend : Wordpress
- Backend : C++
- Organisation : Excel

## Prerequisites 📋

On Ubuntu:
```bash
# vcpkg dependencies
sudo apt install cmake
sudo apt install autoconf
sudo apt install automake
sudo apt install libtool
sudo apt install pkg-config
sudo apt install libfreetype-dev
sudo apt-get install libopenal-dev
sudo apt install liblua5.4-dev
sudo apt-get install libx11-dev libxrandr-dev libxcursor-dev libxi-dev libudev-dev libgl1-mesa-dev # sfml required libraries by vcpkg
```

On Windows:
```bash
For simpler use install chocolatey
choco install cmake
choco install autoconf
choco install automake
choco install libtool
```

## Install the dependencies

Our project uses the vcpkg package manager to manage the dependencies.

On Windows:
```bash
vcpkg install --triplet x64-windows
```

On Linux:
```bash
vcpkg install --triplet x64-linux
```

## How to build the project 🔨
 
```bash
cd build

# Build debug mode (g3 flag)
cmake -DCMAKE_BUILD_TYPE=Debug ..
# Build release mode (O3 flag)
cmake -DCMAKE_BUILD_TYPE=Release ..

# Alternative way to build
cmake ..
cmake --build .

```

## How to run the project 🚀

Launch the server:
```bash
./build/network/rtype_server [PORT]
```

## How to run the tests 🧪

```bash
```

## Commit tag

- [ADD] : Add a new feature :sparkles:
- [FIX] : Fix a bug :bug:
- [DEL] : Remove a feature :x:
- [DOC] : Add documentation :book:
- [TST] : Add tests :microscope:

## Example of the project 📸

## Have fun! 🎉

More informations in the [documentation](./documentation/r_type_doc.md)