#ifndef BITS_PROC_HPP
#define BITS_PROC_HPP

#include <stdio.h>
#include <stdint.h>   
#include "pico/stdlib.h"
#include "hardware/spi.h"

#define HEIGHT 64
#define WIDTH 128
#define PAGE_HEIGHT 8

namespace Cmd
{
    constexpr uint8_t SET_CONTRAST = 0x81;
    constexpr uint8_t CHARGE_PUMP = 0x8D;
    constexpr uint8_t SET_MEMORY_MODE = 0x20;
    constexpr uint8_t SET_COLUMN_RANGE = 0x21;
    constexpr uint8_t SET_PAGE_RANGE = 0x22;
    constexpr uint8_t SET_START_LINE_0 = 0x40;  // start line = 0
    constexpr uint8_t SEG_REMAP_NORMAL = 0xA0;  // col 0 -> SEG0
    constexpr uint8_t SEG_REMAP_FLIPPED = 0xA1; // col 127 -> SEG0
    constexpr uint8_t RESUME_FROM_RAM = 0xA4;   // output follows RAM content
    constexpr uint8_t ALL_PIXELS_ON = 0xA5;     // ignore RAM, light everything
    constexpr uint8_t DISPLAY_NORMAL = 0xA6;
    constexpr uint8_t DISPLAY_INVERTED = 0xA7;
    constexpr uint8_t SET_MUX_RATIO = 0xA8;
    constexpr uint8_t IREF_SELECT = 0xAD; // SSD1315: internal/external IREF
    constexpr uint8_t DISPLAY_OFF = 0xAE;
    constexpr uint8_t DISPLAY_ON = 0xAF;
    constexpr uint8_t COM_SCAN_NORMAL = 0xC0;
    constexpr uint8_t COM_SCAN_REMAPPED = 0xC8;
    constexpr uint8_t SET_DISPLAY_OFFSET = 0xD3;
    constexpr uint8_t SET_CLOCK_DIV = 0xD5;
    constexpr uint8_t SET_PRECHARGE = 0xD9;
    constexpr uint8_t SET_COM_PINS = 0xDA;
    constexpr uint8_t SET_VCOMH = 0xDB;
}

// ------------------------------------------------------------------
// Command parameters
// ------------------------------------------------------------------
namespace CmdParam{
    constexpr uint8_t COMMAND_ACTIVATE      = 0x01;
    constexpr uint8_t CLOCK_DIV_DEFAULT     = 0x80;  // divide ratio 1, oscillator default
    constexpr uint8_t DISPLAY_OFFSET_NONE   = 0x00;
    constexpr uint8_t MEMORY_MODE_HORIZONTAL = 0x00;
    constexpr uint8_t COM_PINS_ALTERNATIVE  = 0x12;
    constexpr uint8_t PRECHARGE_DEFAULT     = 0xF1;
    constexpr uint8_t VCOMH_083_VCC         = 0x30;  // SSD1315: only bits 5:4 are valid
    constexpr uint8_t IREF_INTERNAL_30UA    = 0x30;
}
// ------------------------------------------------------------------
// SPI / timing
// ------------------------------------------------------------------
constexpr uint8_t  SPI_DATA_BITS = 8;
constexpr uint32_t RESET_STEP_MS = 10;
constexpr uint8_t  PIN_LOW  = 0;
constexpr uint8_t  PIN_HIGH = 1;
constexpr uint8_t  DC_COMMAND = PIN_LOW;
constexpr uint8_t  DC_DATA    = PIN_HIGH;

class BitsProc
{
public:
    BitsProc(spi_inst_t *spi = spi0, 
            uint8_t dc = 0, 
            uint8_t cs = 1, 
            uint8_t rst = 2, 
            uint8_t sck = 3, 
            uint8_t mosi = 4);
    
    ~BitsProc() = default;

    /// @brief Initialize the SPI interface and GPIO pins for communication with the SSD1315 display.
    void init();
    

    /// @brief Modify a pixel in the display buffer.
    /// @param x The x-coordinate of the pixel.
    /// @param y The y-coordinate of the pixel.
    /// @param value The value to set the pixel to.
    void modify_pixel(uint8_t x, uint8_t y, bool on);
    
    /// @brief Push the display buffer to the SSD1315 display.
    void push_buffer();
    

    /// @brief Send a command to the SSD1315 display.
    /// @param command The command byte to send.
    /// @param data Optional data to send with the command.
    void send_command(uint8_t command, const uint8_t data);

    /// @brief Send a command with two data bytes to the SSD1315 display.
    /// @param command The command byte to send.
    /// @param data1 The first data byte to send.
    /// @param data2 The second data byte to send.
    void send_command(uint8_t command, const uint8_t data1, const uint8_t data2);

    /// @brief Send a command with no data to the SSD1315 display.
    /// @param command The command byte to send.
    void send_command(uint8_t command); 

private:
    spi_inst_t *spi_{nullptr};
    uint8_t dc_, cs_, rst_, sck_, mosi_;
    bool is_command_mode_{true}; // true for command mode, false for data mode
    uint8_t pixels_buffer_[WIDTH * HEIGHT / 8]{0};

    /// @brief Write bytes to the SPI interface.
    /// @param is_data true if the bytes are data, false if they are commands.
    /// @param data data to send over SPI.
    /// @param length legth of the data to send.
    /// @return true if the bytes were written successfully, false otherwise.
    bool write_bytes(bool is_data, const uint8_t* data, size_t length);  // replaces send_spi
};

#endif // BITS_PROC_HPP