cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
include(pico_sdk_import.cmake)

project(macro_pad)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# ADD HERE 

add_executable(main
    main.c
)

target_link_libraries(main pico_stdlib)
pico_add_extra_outputs(main)