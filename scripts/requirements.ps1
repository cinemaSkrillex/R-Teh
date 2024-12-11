# Set execution policy to bypass
Set-ExecutionPolicy Bypass -Scope Process -Force

# Install Chocolatey
[System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072
iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))

# Install dependencies using Chocolatey
choco install -y cmake
choco install -y autoconf
choco install -y automake
choco install -y libtool
choco install -y git

# git clone vcpkg

git clone https://github.com/microsoft/vcpkg.git