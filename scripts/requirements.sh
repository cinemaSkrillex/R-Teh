#!/bin/bash

# Function to install packages using apt
install_packages_apt() {
    sudo apt update
    sudo apt install -y cmake autoconf automake libtool pkg-config libfreetype-dev libopenal-dev liblua5.4-dev libx11-dev libxrandr-dev libxcursor-dev libxi-dev libudev-dev libgl1-mesa-dev
    sudo apt autoremove -y
}

# Function to install packages using pamac
install_packages_pamac() {
    sudo pacman update
    sudo pacman -S cmake autoconf automake libtool pkgconf freetype2 openal lua54 libx11 libxrandr libxcursor libxi systemd mesa
}

# Detect the distribution and install packages
if [ -f /etc/debian_version ]; then
    echo "Detected Debian-based distribution"
    install_packages_apt
elif [ -f /etc/arch-release ]; then
    echo "Detected Arch Linux"
    install_packages_pamac
else
    echo "Unsupported distribution"
    exit 1
fi

# Clone the vcpkg repository
if [ ! -d "vcpkg" ]; then
    git clone https://github.com/microsoft/vcpkg.git
    ./vcpkg/bootstrap-vcpkg.sh
else
    echo "vcpkg repository already exists"
fi