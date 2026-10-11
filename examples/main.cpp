#include <stdio.h>
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
    
    
    display.init();
    display.turn_on();
    display.draw_rectangle(rect_pos, 50, 30, false); 
    display.draw_line(line_start, line_end); // Draw a line from top-left to bottom-right
    display.draw_text({10, 50}, "Hello -%", 2, 4); // Draw the text "Hello" at position (10, 50)
    display.draw_char({100, 10}, 'A', 2); // Draw the character 'A' at position (100, 50)
    display.apply_changes(); // Push the changes to the display
    

    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);

    }
}
