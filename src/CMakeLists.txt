cmake_minimum_required(VERSION 3.13)

# Initialize the SDK based on PICO_SDK_PATH
include(pico_sdk_import.cmake)

project(macro_pad)
set(CMAKE_C_STANDARD 11)

# Initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# Add the executable for the project
add_executable(macro_pad
    main.c
    lib/mhid.c
    lib/usb_descriptors.c
    lib/encoder.c
    lib/ssd1306.c
    lib/hid_helpers.c
)

pico_set_program_name(macro_pad "macro_pad")
pico_set_program_version(macro_pad "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(macro_pad 0)
pico_enable_stdio_usb(macro_pad 1)

# Update the include directories to point to the lib folder and the root folder
target_include_directories(macro_pad PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/lib
    ${PICO_SDK_PATH}/lib/tinyusb/src
    ${PICO_SDK_PATH}/lib/tinyusb/src/class/hid
    ${PICO_SDK_PATH}/lib/tinyusb/lib

)

# Link libraries to the target executable
target_link_libraries(macro_pad
    pico_stdlib
    pico_unique_id
    hardware_i2c
    tinyusb_device
    tinyusb_board
)

# Set additional compile options if needed
target_compile_options(macro_pad PRIVATE -Wall -Wextra -Wconversion -Werror=conversion)

# Generate UF2, bin, and hex files
pico_add_extra_outputs(macro_pad)
