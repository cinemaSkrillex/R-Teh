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

#### Languages and Libraries
- Main language: C++
- Graphic library: SFML
- Network protocol: TCP/UDP using Asio

#### Tools and Systems
- Build system: CMake
- Code formatter: ClangFormat
- Package manager: Vcpkg
- CI/CD: GitHub Actions


## Prerequisites 📋

On Linux:
```bash
# Run the script to install the dependencies (from root)
chmod +x ./Scripts/requirements.sh
./Scripts/requirements.sh
```

On Windows:
```bash
# Open an administrator PowerShell and run the following command :
Set-ExecutionPolicy Bypass -Scope Process -Force; .\Scripts\requirements.ps1
```

## Install the dependencies

On Linux:
```bash
cmake --preset linux
```

On Windows:
```bash
#Release
cmake --preset windows-release
#Debug
cmake --preset windows-debug
```

## How to build the project 🔨
 
```bash
cd build/default

# Build debug mode (g3 flag)
cmake -DCMAKE_BUILD_TYPE=Debug .
# Build release mode (O3 flag)
cmake -DCMAKE_BUILD_TYPE=Release .

# Build the cmake and build the project with 20 threads
cmake . && cmake --build . -j 20

```

## How to run the project 🚀

Launch the server:
```bash
./build/default/r_type_server/RtypeServer [PORT]
```

Launch the client:
```bash
./build/default/RtypeGame/rtype [serverIp] [SERVER_PORT] [clientPort]
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

### Custom commit tags (CI/CD)

- [BUILD] : Build the project 🔨
- [RELEASE] : Release the project 🚀
- [DOC] : Deploy documentation online 📘

## Documentation 📚

Our documentation is hosted on github.io. You can find it here [website link](https://cinemaskrillex.github.io/R-Teh/).

## Images 📸

![Rtype-game](<documentation/images/rtype_readme_images.png>)

## Have fun! 🎉
