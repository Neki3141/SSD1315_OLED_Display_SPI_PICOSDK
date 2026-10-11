#ifndef SSD1315_SPI_HPP
#define SSD1315_SPI_HPP 

#include <cstdint>
#include <cstdio>
#include <array>
#include <unordered_map>
#include <cctype>
#include "bits_proc.hpp"
#include "pico/stdlib.h"
#include "hardware/spi.h"

/**
 * @brief Structure to represent a character and its corresponding column data for the font.
 * @param ch The character.
 * @param col The column data for the character.
 */
struct Character { char ch; uint8_t col[5]; };



static const std::unordered_map<char, const std::array<uint8_t, 5>> FONT_MAP = {
    {' ', {0x00,0x00,0x00,0x00,0x00}}, {'!', {0x00,0x00,0x5F,0x00,0x00}},
    {'-', {0x08,0x08,0x08,0x08,0x08}}, {'.', {0x00,0x60,0x60,0x00,0x00}},
    {':', {0x00,0x36,0x36,0x00,0x00}},
    {';', {0x00,0x36,0x36,0x00,0x00}}, {'#', {0x14,0x7F,0x14,0x7F,0x14}},
    {'$', {0x24,0x2A,0x7F,0x2A,0x12}}, {'%', {0x23,0x13,0x08,0x64,0x62}},
    {'&', {0x36,0x49,0x55,0x22,0x50}}, {'\'', {0x00,0x00,0x07,0x00,0x00}},
    {'(', {0x00,0x1C,0x22,0x41,0x00}}, {')', {0x00,0x41,0x22,0x1C,0x00}},
    {'*', {0x14,0x08,0x3E,0x08,0x14}}, {'+', {0x08,0x08,0x3E,0x08,0x08}},
    {'0', {0x3E,0x51,0x49,0x45,0x3E}}, {'1', {0x00,0x42,0x7F,0x40,0x00}},
    {'2', {0x42,0x61,0x51,0x49,0x46}}, {'3', {0x21,0x41,0x45,0x4B,0x31}},
    {'4', {0x18,0x14,0x12,0x7F,0x10}}, {'5', {0x27,0x45,0x45,0x45,0x39}},
    {'6', {0x3C,0x4A,0x49,0x49,0x30}}, {'7', {0x01,0x71,0x09,0x05,0x03}},
    {'8', {0x36,0x49,0x49,0x49,0x36}}, {'9', {0x06,0x49,0x49,0x29,0x1E}},
    {'A', {0x7E,0x11,0x11,0x11,0x7E}}, {'B', {0x7F,0x49,0x49,0x49,0x36}},
    {'C', {0x3E,0x41,0x41,0x41,0x22}}, {'D', {0x7F,0x41,0x41,0x22,0x1C}},
    {'E', {0x7F,0x49,0x49,0x49,0x41}}, {'F', {0x7F,0x09,0x09,0x09,0x01}},
    {'G', {0x3E,0x41,0x49,0x49,0x7A}}, {'H', {0x7F,0x08,0x08,0x08,0x7F}},
    {'I', {0x00,0x41,0x7F,0x41,0x00}}, {'J', {0x20,0x40,0x41,0x3F,0x01}},
    {'K', {0x7F,0x08,0x14,0x22,0x41}}, {'L', {0x7F,0x40,0x40,0x40,0x40}},
    {'M', {0x7F,0x02,0x0C,0x02,0x7F}}, {'N', {0x7F,0x04,0x08,0x10,0x7F}},
    {'O', {0x3E,0x41,0x41,0x41,0x3E}}, {'P', {0x7F,0x09,0x09,0x09,0x06}},
    {'Q', {0x3E,0x41,0x51,0x21,0x5E}}, {'R', {0x7F,0x09,0x19,0x29,0x46}},
    {'S', {0x46,0x49,0x49,0x49,0x31}}, {'T', {0x01,0x01,0x7F,0x01,0x01}},
    {'U', {0x3F,0x40,0x40,0x40,0x3F}}, {'V', {0x1F,0x20,0x40,0x20,0x1F}},
    {'W', {0x3F,0x40,0x38,0x40,0x3F}}, {'X', {0x63,0x14,0x08,0x14,0x63}},
    {'Y', {0x07,0x08,0x70,0x08,0x07}}, {'Z', {0x61,0x51,0x49,0x45,0x43}},
};
   

class Ssd1315_spi {
public:
    /// @brief Constructor for the Ssd1315_spi class.
    /// @param spi spi instance to use for communication (default is spi0).
    /// @param dc The data/command pin.
    /// @param cs The chip select pin.
    /// @param rst The reset pin.
    /// @param sck The serial clock pin.
    /// @param mosi The master out slave in pin.
    Ssd1315_spi(spi_inst_t *spi = spi0, uint8_t dc = 0, uint8_t cs = 1, 
    uint8_t rst = 2, uint8_t sck = 3, uint8_t mosi = 4); // Initialized ONCE here      
    ~Ssd1315_spi() = default;
    
    /// @brief Structure to represent a coordinate on the display.
    /// @param x The x-coordinate (0-127).
    /// @param y The y-coordinate (0-63).
    struct Coordinate { uint8_t x; uint8_t y; };

    /// @brief Initialize the SSD1315 display.
    /// This function initializes the display by sending the necessary commands to set it up.
    void init();
    
    /// @brief Turn on the SSD1315 display.
    void turn_on();
    /// @brief Turn off the SSD1315 display.
    void turn_off();
    // void set_contrast(uint8_t contrast);
    // void set_inverted(bool inverted);

    /// @brief Draw a rectangle on the display.
    /// @param pos position of the top-left corner of the rectangle.
    /// @param width The width of the rectangle.
    /// @param height The height of the rectangle.
    /// @param is_filled Whether the rectangle should be filled.
    void draw_rectangle(Coordinate pos, uint8_t width, uint8_t height, bool is_filled = false);
    
    
    /// @brief Draw a line on the display.
    /// @param p0 The start point of the line.
    /// @param p1 The end point of the line.
    void draw_line(Coordinate p0, Coordinate p1);

    /// @brief Draw text on the display.
    /// @param pos The position to start drawing the text.
    /// @param text The text to draw.
    /// @param size How big the text is.
    /// @param spacing The spacing between characters.
    void draw_text(Coordinate pos, const char* text, size_t size, size_t spacing = 1);

    /// @brief Draw a single character on the display.
    /// @param pos The position to start drawing the character.
    /// @param c The character to draw.
    /// @param size How big the character is.
    void draw_char(Coordinate pos, char c, size_t size);
    
    /// @brief Clear the display buffer and the actual display.
    void clear();
    
    /// @brief Apply the changes made to the display buffer to the actual display.
    void apply_changes();

private:
    BitsProc bits_proc_; // Instance of BitsProc class for SPI communication
    
    void load_character_data(char c, uint8_t* buffer, size_t size);

    /// @brief Set the state of a pixel in the display buffer.
    /// @param x x-coordinate of the pixel (0-127)
    /// @param y y-coordinate of the pixel (0-63)
    /// @param on The state of the pixel (true for on, false for off)
    void set_pixel(Coordinate pos, bool on = true);
    
    /// @brief Get the state of a pixel in the display buffer.
    /// @param x x-coordinate of the pixel (0-127)
    /// @param y y-coordinate of the pixel (0-63)
    bool get_pixel(Coordinate pos);

    /// @brief Get the absolute value of an integer.
    /// @param value The integer to get the absolute value of.
    int abs(int value);

};

    

#endif // SSD1315_SPI_HPP