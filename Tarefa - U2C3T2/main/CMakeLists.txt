# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 1.5.1)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(main C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(PICO_BOARD pico_w)
if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

pico_sdk_init()

add_compile_options(
    -Wall
    -Wno-format
    -Wno-unused-result
    -Wno-unused-function
    -Wno-maybe-uninitialized
)

if (PICO_CYW43_SUPPORTED)
    if (NOT TARGET pico_cyw43_arch)
        message("Skipping build as support is not available")
    else()
        add_executable(main
            main.c
            ./src/wifi.c
            ./src/mqtt.c
            ./src/sensors.c
            ./src/ssd1306.c
        )

        pico_enable_stdio_usb(main 1)

        target_include_directories(main PRIVATE
            ${CMAKE_CURRENT_LIST_DIR}
            ${CMAKE_CURRENT_LIST_DIR}/..
            ./include/
            ./src/
        )

        target_link_libraries(main
            pico_cyw43_arch_lwip_poll
            pico_lwip_mbedtls
            pico_lwip_mqtt
            pico_mbedtls
            pico_stdlib
            hardware_gpio
            hardware_adc
            hardware_i2c
        )

        pico_add_extra_outputs(main
            
        )
    endif()
endif()