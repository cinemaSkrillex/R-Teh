# Windows VM requirements

Prepare a windows 11 VM for running the project

## Prerequisites

### Install windows 11 for VM

Download the iso from the official windows website : 
```bash
https://www.microsoft.com/fr-fr/software-download/windows11
```

### On the host machine (Linux)

1. Install virt-manager or gnome-boxes for managing the VM

    ```bash
    sudo apt-get install virt-manager
    #or
    sudo apt-get install gnome-boxes
    ```

2. Add the ISO into the VM

3. You want about half of your ram for the VM, and also about 50gb of disk space.

4. Create the VM

5. Start the VM

## Windows 11 installation

1. Do the installation of windows 11, setup stuff like language, keyboard, etc.


2. Install chocolatey (powershell admin)
    ```bash
    Set-ExecutionPolicy AllSigned
    Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))
    ```

3. Install git, cmake and ninja from chocolatey (powershell admin)

    ```bash
    choco install git -y
    choco install cmake -y
    choco install ninja -y
    ```

4. Install visual studio, and in the visual studio installer, add the c++ dependencies
    ```bash
    https://visualstudio.microsoft.com/fr/
    ```
    In the visual studio installer, add c++, vcpkg, msvc v141 (2017) for x64 and x86

5. Install windows sdk
    ```bash
    https://go.microsoft.com/fwlink/?linkid=2286561
    ```

6. Quality of life : Spice windows guest tools : https://www.spice-space.org/download.html

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
cmake --preset windows
```