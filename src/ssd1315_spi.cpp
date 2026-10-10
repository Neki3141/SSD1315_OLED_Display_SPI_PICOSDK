#include "ssd1315_spi.hpp"

Ssd1315_spi::Ssd1315_spi(spi_inst_t *spi, uint8_t dc, uint8_t cs, uint8_t rst, uint8_t sck, uint8_t mosi)
    : bits_proc_(spi, dc, cs, rst, sck, mosi) {
    // Constructor body can be empty since initialization is done in the initializer list
}


void Ssd1315_spi::init() {
    bits_proc_.init();
}

void Ssd1315_spi::turn_on() {
    const uint8_t display_on_command[2] = {Cmd::DISPLAY_ON};
    bits_proc_.send_command(Cmd::DISPLAY_ON); // Display ON command
}

void Ssd1315_spi::turn_off() {
    const uint8_t display_off_command[2] = {Cmd::DISPLAY_OFF};
    bits_proc_.send_command(Cmd::DISPLAY_OFF); // Display OFF command
}

void Ssd1315_spi::draw_rectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height, bool is_filled) {
    for (uint8_t i = 0; i < height; ++i) {
        for (uint8_t j = 0; j < width; ++j) {
            if (is_filled || i == 0 || i == height - 1 || j == 0 || j == width - 1) {
                set_pixel(x + j, y + i, true);
            }
        }
    }
    
}

void Ssd1315_spi::draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
    int dx = abs(x1 - x0);
    int dy = -abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx + dy;

    while (true) {
        set_pixel(x0, y0, true);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void Ssd1315_spi::apply_changes() {
    bits_proc_.push_buffer(); // Push the buffer to the display
}

void Ssd1315_spi::set_pixel(uint8_t x, uint8_t y, bool on) {
    if (x >= WIDTH || y >= HEIGHT) return; // Out of bounds check
    size_t index = (y / 8) * WIDTH + x; // Calculate the index in the buffer
    uint8_t bit_position = y % 8; // Determine which bit to set/clear

    if (on) {
        bits_proc_.modify_pixel(x, y, true); // Set the pixel
    } else {
        bits_proc_.modify_pixel(x, y, false); // Clear the pixel
    }

}

int Ssd1315_spi::abs(int value) {
    return (value < 0) ? -value : value;
}
