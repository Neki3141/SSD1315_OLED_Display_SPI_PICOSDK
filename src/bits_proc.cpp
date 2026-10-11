#include "bits_proc.hpp"

BitsProc::BitsProc(spi_inst_t *spi, uint8_t dc, uint8_t cs, uint8_t rst, uint8_t sck, uint8_t mosi)
    : spi_(spi), dc_(dc), cs_(cs), rst_(rst), sck_(sck), mosi_(mosi) {
     // Set GPIO functions for SPI pins
    gpio_set_function(mosi_, GPIO_FUNC_SPI);
    gpio_set_function(sck_, GPIO_FUNC_SPI);
    gpio_set_function(dc_, GPIO_FUNC_SIO);
    gpio_set_function(cs_, GPIO_FUNC_SIO);
    gpio_set_function(rst_, GPIO_FUNC_SIO);

    // Set directions for control pins
    gpio_set_dir(dc_, GPIO_OUT);
    gpio_set_dir(cs_, GPIO_OUT);
    gpio_set_dir(rst_, GPIO_OUT);

    // Initialize control pins to default states
    gpio_put(cs_, PIN_HIGH);  // Chip select inactive (high)
    gpio_put(dc_, PIN_LOW);   // Data/Command pin low (command mode)
    
    // Reset the display
    gpio_put(rst_, PIN_LOW);  // Assert reset
    sleep_ms(RESET_STEP_MS);  // Wait for reset duration
    gpio_put(rst_, PIN_HIGH); // Deassert reset
    spi_set_format(spi_, SPI_DATA_BITS, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);  // mode 0
}

void BitsProc::init() {
    sleep_ms(100);                                   // let the panel settle after reset
    send_command(Cmd::DISPLAY_OFF);
    send_command(Cmd::SET_CLOCK_DIV, 0x80);
    send_command(Cmd::SET_MUX_RATIO, HEIGHT - 1);
    send_command(Cmd::SET_DISPLAY_OFFSET, 0x00);
    send_command(Cmd::SET_START_LINE_0);
    send_command(Cmd::CHARGE_PUMP, 0x14);            // enable internal charge pump
    send_command(Cmd::SET_MEMORY_MODE, 0x00);        // horizontal addressing
    send_command(Cmd::SEG_REMAP_FLIPPED);
    send_command(Cmd::COM_SCAN_REMAPPED);
    send_command(Cmd::SET_COM_PINS, 0x12);
    send_command(Cmd::SET_CONTRAST, 0xCF);
    send_command(Cmd::SET_PRECHARGE, 0xF1);
    send_command(Cmd::SET_VCOMH, 0x30);
    send_command(Cmd::RESUME_FROM_RAM);
    send_command(Cmd::DISPLAY_NORMAL);
    send_command(Cmd::DISPLAY_ON);
}

void BitsProc::modify_pixel(uint8_t x, uint8_t y, bool on) {
    if (x >= WIDTH || y >= HEIGHT) return; // Out of bounds check
    size_t index = (y / PAGE_HEIGHT) * WIDTH + x; // Calculate the index in the buffer
    uint8_t bit_position = y % PAGE_HEIGHT; // Determine which bit to set/clear

    if (on) {
        pixels_buffer_[index] |= (1 << bit_position); // Set the pixel
    } else {
        pixels_buffer_[index] &= ~(1 << bit_position); // Clear the pixel
    }
}

void BitsProc::push_buffer() {
    send_command(Cmd::SET_COLUMN_RANGE, 0, WIDTH - 1);
    send_command(Cmd::SET_PAGE_RANGE, 0, PAGE_HEIGHT - 1);
    write_bytes(true, pixels_buffer_, sizeof(pixels_buffer_));
}


void BitsProc::send_command(uint8_t command) {
    write_bytes(false, &command, 1);
}
void BitsProc::send_command(uint8_t command, uint8_t d) {
    uint8_t buf[2] = {command, d};
    write_bytes(false, buf, 2);
}
void BitsProc::send_command(uint8_t command, uint8_t d1, uint8_t d2) {
    uint8_t buf[3] = {command, d1, d2};
    write_bytes(false, buf, 3);
}

bool BitsProc::write_bytes(bool is_data, const uint8_t* data, size_t length) {
    gpio_put(dc_, is_data ? DC_DATA : DC_COMMAND);
    gpio_put(cs_, PIN_LOW);
    spi_write_blocking(spi_, data, length);
    gpio_put(cs_, PIN_HIGH);
    return true;
}





