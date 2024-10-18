export PICO_SDK_PATH=$(pwd)/pico-sdk
mkdir -p build
cd build
cmake .. -DPICO_SDK_PATH=$PICO_SDK_PATH -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
make -j$(nproc)
