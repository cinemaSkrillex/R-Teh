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

- Main language : C++
- Scripting language : Lua
- Network protocol : TCP/UDP
- Graphic library : SFML
- Audio library : OpenAL
- Build system : CMake
- CI/CD : GitHub Actions
- Code formatter : ClangFormat
- Package manager : Vcpkg

## Prerequisites 📋

On Linux:
```bash
<chmod +x ./scripts/requirements.sh>
./scripts/requirements.sh
```

On Windows:
```bash
# Open an administrator PowerShell and run the following command : 
Set-ExecutionPolicy Bypass -Scope Process -Force; .\scripts\requirements.ps1
```

## Install the dependencies

On Windows:
```bash
cmake --preset windows
```

On Linux:
```bash
cmake --preset linux
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

## Commit tags

- [ADD] : Add a new feature :sparkles:
- [FIX] : Fix a bug :bug:
- [DEL] : Remove a feature :x:
- [DOC] : Add documentation :book:
- [TST] : Add tests :microscope:

## Example of the project 📸

## Have fun! 🎉

More informations in the [documentation](./documentation/r_type_doc.md)