#!/bin/bash

sudo apt update

sudo apt install -y cmake
sudo apt install -y autoconf
sudo apt install -y automake
sudo apt install -y libtool
sudo apt install -y pkg-config
sudo apt install -y libfreetype-dev
sudo apt-get install -y libopenal-dev
sudo apt install -y liblua5.4-dev
sudo apt-get install -y libx11-dev libxrandr-dev libxcursor-dev libxi-dev libudev-dev libgl1-mesa-dev
# clear unused packages
sudo apt autoremove -y