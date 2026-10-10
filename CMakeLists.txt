cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.3.1)
set(toolchainVersion 15_2_Rel1)
set(picotoolVersion 2.3.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(pico_ssd1315 C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# ----------------------------------------------------------------------------------
# 1. DEFINE THE SSD1315 LIBRARY
# ----------------------------------------------------------------------------------
add_library(pico_ssd1315_spi STATIC)

# Expose the include/ directory so users can write #include "ssd1315.hpp"
target_include_directories(pico_ssd1315_spi PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

# Attach source files
target_sources(pico_ssd1315_spi PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/bits_proc.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ssd1315_spi.cpp
)

# Link hardware libraries required for communication (I2C / SPI)
target_link_libraries(pico_ssd1315_spi PUBLIC
    pico_stdlib
    hardware_i2c
    hardware_spi
)

# ----------------------------------------------------------------------------------
# 2. DEFINE THE EXAMPLE EXECUTABLE
# ----------------------------------------------------------------------------------
add_executable(SSD1315_oled_example
    examples/main.cpp
)

pico_set_program_name(SSD1315_oled_example "SSD1315_oled_example")
pico_set_program_version(SSD1315_oled_example "0.1")

# USB/UART Stdio configuration
pico_enable_stdio_uart(SSD1315_oled_example 0)
pico_enable_stdio_usb(SSD1315_oled_example 1)

# Link your library and standard SDK to the executable
target_link_libraries(SSD1315_oled_example
    pico_stdlib
    pico_ssd1315_spi
)

# Generate .uf2, .hex, and .elf files
pico_add_extra_outputs(SSD1315_oled_example)