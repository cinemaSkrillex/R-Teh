---
title: Installation vcpkg dependencies
parent: Requirements
grand_parent: User
nav_order: 2
---

## Install vcpkg dependencies

### vcpkg
```bash
# LINUX vcpkg:
# in the root of the repository
git clone https://github.com/microsoft/vcpkg.git
```

WINDOWS vcpkg :
```bash
#in the root of the repository
open git CMD
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
```

### Install vcpkg dependencies
```bash
# In the root of the repository
#Linux
cmake --preset linux
#Windows
cmake --preset windows-release (or windows-debug)
```