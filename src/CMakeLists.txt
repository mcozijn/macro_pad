cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
include(pico_sdk_import.cmake)

project(macro_pad)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# ADD HERE 

add_executable(macro_pad
    main.c
    hid.c 
    usb_descriptors.c
)

pico_set_program_name(macro_pad "macro_pad")
pico_set_program_version(macro_pad "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(macro_pad 1)
pico_enable_stdio_usb(macro_pad 0)

# Add the standard library to the build
target_link_libraries(macro_pad
        pico_stdlib pico_unique_id tinyusb_device tinyusb_board)


target_include_directories(macro_pad PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/..
)

pico_add_extra_outputs(macro_pad)