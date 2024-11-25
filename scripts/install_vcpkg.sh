#!/bin/bash

# Determine the shell configuration file
if [[ $SHELL == *"zsh"* ]]; then
    SHELL_CONFIG_FILE="$HOME/.zshrc"
else
    SHELL_CONFIG_FILE="$HOME/.bashrc"
fi

# Clone the vcpkg repository
git clone https://github.com/microsoft/vcpkg.git

cd vcpkg

# Bootstrap vcpkg
./bootstrap-vcpkg.sh

# Update the shell configuration file
echo "export VCPKG_ROOT=$(pwd)" >> $SHELL_CONFIG_FILE
echo "export PATH=\"$(pwd):\$PATH\"" >> $SHELL_CONFIG_FILE

source $SHELL_CONFIG_FILE

echo $VCPKG_ROOT && echo $PATH