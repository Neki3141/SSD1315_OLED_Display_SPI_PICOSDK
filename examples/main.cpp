#include <stdio.h>
#include <string>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "ssd1315_spi.hpp"

// SPI Defines
// We are going to use SPI 0, and allocate it to the following GPIO pins
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define SPI_PORT spi0
#define BAUD_RATE 1000*1000  // 1 MHz
// #define PIN_MISO 16
// #define PIN_CS   17
// #define PIN_SCK  18
// #define PIN_MOSI 19

static constexpr uint8_t PIN_SCK  = 18;
static constexpr uint8_t PIN_MOSI = 19;
static constexpr uint8_t PIN_CS   = 17;
static constexpr uint8_t PIN_DC   = 20;
static constexpr uint8_t PIN_RST  = 21;


int main()
{
    stdio_init_all();
    sleep_ms(2000); // Wait for USB to be ready
    // SPI initialisation. This example will use SPI at 1MHz.
    spi_init(SPI_PORT, BAUD_RATE);

    Ssd1315_spi display(SPI_PORT, PIN_DC, PIN_CS, PIN_RST, PIN_SCK, PIN_MOSI);
    Ssd1315_spi::Coordinate rect_pos = {20, 20}; // Position for the rectangle
    Ssd1315_spi::Coordinate line_start = {0, 0}; // Start position for the line
    Ssd1315_spi::Coordinate line_end = {127, 63}; // End position for the line
    Ssd1315_spi::Coordinate text_pos = {60, 30}; // Position for the text
    
    display.init();
    
    display.turn_on();
    display.draw_rectangle(rect_pos, 50, 30, Ssd1315_spi::Pixel::On, true); // Draw a filled rectangle
    display.draw_line(line_start, line_end, Ssd1315_spi::Pixel::On); // Draw a line from top-left to bottom-right
    display.apply_changes(); // Push the changes to the display
    sleep_ms(2000); // Wait for 2 seconds to let
    display.remove_area(rect_pos, 50, 30); // Remove the rectangle area
    display.apply_changes(); // Push the changes to the display
    sleep_ms(2000); // Wait for 2 seconds to let the user see the changes
    display.clear(); // Clear the display buffer
    display.apply_changes(); // Push the changes to the display


    char start = 'a'; // Size of the text
    while (true) {
        display.draw_text(text_pos, std::string(1, start).c_str(), 2, Ssd1315_spi::Pixel::On); // Draw the character
        display.apply_changes(); // Push the changes to the display
        sleep_ms(1000); 
        display.draw_text(text_pos, std::string(1, start).c_str(), 2, Ssd1315_spi::Pixel::Off); // Delete the character
        display.apply_changes(); 
        start++; 
        if (start > 'z') start = '0'; // Reset to '0' after 'z'
        if (start > '9' && start < 'a') start = 'a'; // Skip non-alphanumeric characters


    }
}
