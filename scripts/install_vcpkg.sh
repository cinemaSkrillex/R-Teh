# Clone the vcpkg repository
git clone https://github.com/microsoft/vcpkg.git

# Navigate to the vcpkg directory
cd vcpkg

# Bootstrap vcpkg
./bootstrap-vcpkg.sh

# Set the VCPKG_ROOT environment variable
# export VCPKG_ROOT=$(pwd) >> ~/.bashrc
# echo "VCPKG_ROOT is set to $VCPKG_ROOT"
echo "export VCPKG_ROOT=$(pwd)" >> ~/.bashrc
echo "export PATH=\"$(pwd):$PATH\"" >> ~/.bashrc
echo $VCPKG_ROOT && echo $PATH