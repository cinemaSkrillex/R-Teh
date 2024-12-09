# Windows VM requirements

Prepare a windows 11 VM for running the project

## Prerequisites

### Install windows 11 for VM

This is a light windows 11 version that can be used for testing purposes. It is not activated, but it is enough for our needs.

The installation media is called tiny11

You can download it here : 
```bash
https://fr.taiwebs.com/windows/download-tiny11-8649.html
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

5. Install windows sdk
    ```bash
    https://go.microsoft.com/fwlink/?linkid=2286561
    ```

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