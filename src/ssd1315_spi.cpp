#include "ssd1315_spi.hpp"

Ssd1315_spi::Ssd1315_spi(spi_inst_t *spi, uint8_t dc, uint8_t cs, uint8_t rst, uint8_t sck, uint8_t mosi)
    : bits_proc_(spi, dc, cs, rst, sck, mosi) {
    // Constructor body can be empty since initialization is done in the initializer list
}


void Ssd1315_spi::init() {
    bits_proc_.init();
    turn_on();
    draw_rectangle({0, 0}, WIDTH, HEIGHT, Pixel::On); // Clear the display buffer
    draw_text({20, 30}, "Neki_lib :)", 1, Pixel::On); // Draw initial text
    apply_changes(); // Push the changes to the display
    sleep_ms(2000); // Wait for a second to let the user see the initial message
    clear(); // Clear the display buffer after showing the initial message
    apply_changes(); // Push the changes to the display
}

void Ssd1315_spi::turn_on() {
    bits_proc_.send_command(Cmd::DISPLAY_ON); // Display ON command
}

void Ssd1315_spi::turn_off() {
    bits_proc_.send_command(Cmd::DISPLAY_OFF); // Display OFF command
}

void Ssd1315_spi::draw_rectangle(Coordinate pos, uint8_t width, uint8_t height, Pixel on, bool is_filled) {
    for (uint8_t y = pos.y; y < pos.y + height; ++y) {
        for (uint8_t x = pos.x; x < pos.x + width; ++x) {
            if (is_filled || y == pos.y || y == pos.y + height - 1 || x == pos.x || x == pos.x + width - 1) {
                set_pixel({x, y}, on); // Set the pixel in the display buffer
            }
        }
    }
}

void Ssd1315_spi::draw_line(Coordinate p0, Coordinate p1, Pixel on) {
    int dx = abs(p1.x - p0.x);
    int dy = abs(p1.y - p0.y);
    int sx = (p0.x < p1.x) ? 1 : -1;
    int sy = (p0.y < p1.y) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        set_pixel({p0.x, p0.y}, on); // Set the pixel at the current position
        if (p0.x == p1.x && p0.y == p1.y) break; // If we've reached the end point, exit
        int e2 = err * 2;
        if (e2 > -dy) {
            err -= dy;
            p0.x += sx;
        }
        if (e2 < dx) {
            err += dx;
            p0.y += sy;
        }
    }
}

void Ssd1315_spi::draw_text(Coordinate pos, const char* text, size_t size, Pixel on, size_t spacing) {
    for(size_t i = 0; text[i] != '\0'; ++i) {
        draw_char({static_cast<uint8_t>(pos.x + i * (5 * size + spacing)), pos.y}, text[i], on, size);
    }
}

void Ssd1315_spi::draw_char(Coordinate pos, char c, Pixel on, size_t size) {
    uint8_t char_data[5];
    load_character_data(c, char_data, size);

    for (size_t col = 0; col < 5; ++col) {
        for (size_t row = 0; row < 8; ++row) {
            bool pixel_on = (char_data[col] >> row) & 0x01;
            if (pixel_on) {
                // Draw a scaled block for each "on" pixel
                for (size_t dx = 0; dx < size; ++dx) {
                    for (size_t dy = 0; dy < size; ++dy) {
                        set_pixel({
                            static_cast<uint8_t>(pos.x + (col * size) + dx), 
                            static_cast<uint8_t>(pos.y + (row * size) + dy)
                        }, on);
                    }
                }
            }
        }
    }
}

void Ssd1315_spi::remove_area(Coordinate pos, uint8_t width, uint8_t height) {
    draw_rectangle(pos, width, height, Pixel::Off, true); // Fill the area with "off" pixels
}

void Ssd1315_spi::load_character_data(char c, uint8_t* buffer, size_t size) {
    char upper_c = std::toupper(static_cast<unsigned char>(c));
    if (FONT_MAP.find(upper_c) == FONT_MAP.end()) {
        std::printf("Character '%c' not found in FONT_MAP. Defaulting to space.\n", c);
        upper_c = ' '; // Default to space if character not found
    }

    const auto& char_data = FONT_MAP.at(upper_c);
    for (size_t i = 0; i < 5; ++i) {
        buffer[i] = char_data[i];
    }

}

void Ssd1315_spi::clear() {
    for (uint8_t y = 0; y < HEIGHT; ++y) {
        for (uint8_t x = 0; x < WIDTH; ++x) {
            set_pixel({x, y}, Pixel::Off); // Clear the pixel in the display buffer
        }
    }
}

void Ssd1315_spi::apply_changes() {
    bits_proc_.push_buffer(); // Push the buffer to the display
}

void Ssd1315_spi::set_pixel(Coordinate pos, Pixel on) {
    bits_proc_.modify_pixel(pos.x, pos.y, static_cast<bool>(on)); // Modify the pixel in the display buffer
}

int Ssd1315_spi::abs(int value) {
    return (value < 0) ? -value : value;
}
