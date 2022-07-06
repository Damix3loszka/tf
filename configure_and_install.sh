echo "Creating build folder..."
mkdir ./build
echo "Generating build system..."
cmake -B ./build
cd ./build
echo "Building..."
make
echo "Installing..."
sudo make install
echo "Done"