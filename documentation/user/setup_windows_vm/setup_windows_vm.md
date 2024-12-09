# Windows VM requirements

Prepare a windows 11 VM for running the project

### tiny11

```bash
https://fr.taiwebs.com/windows/download-tiny11-8649.html
```

## Install windows 11 for vcpkg

**Note** You want about half of your ram for the VM, and also about 50gb of disk space.

install chocolatey (powershell admin)
```bash
Set-ExecutionPolicy AllSigned
Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))
```

install git, cmake and ninja from chocolatey (powershell admin)

```bash
choco install git -y
choco install cmake -y
choco install ninja -y
```

Install visual studio
And in the visual studio installer, add the c++ dependencies
```bash
https://visualstudio.microsoft.com/fr/
```

Install windows sdk
```bash
https://go.microsoft.com/fwlink/?linkid=2286561
```

## On linux

### install gnome-boxes or virt-manager for VM management

```bash
sudo apt-get install virt-manager
sudo apt-get install gnome-boxes
```

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