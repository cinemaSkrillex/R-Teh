# Install vcpkg

### tiny11

```bash
https://fr.taiwebs.com/windows/download-tiny11-8649.html
```

## Install windows 11 for vcpkg

RAM / 2 (16 = 8), 70+ gb (visual studio = 15 + windows sdk 3.5), tiny11 3.5gb

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
LINUX vcpkg:
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
```

WINDOWS vcpkg :
```bash
open git CMD

git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
```

Now to install vcpkg sfml.


**Note** On both system this command works.
```bash
vcpkg install sfml
```
