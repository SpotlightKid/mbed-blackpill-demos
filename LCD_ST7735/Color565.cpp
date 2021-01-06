///////////////////////////////////////////////////////////////////////////////
// LCD_ST7735 - Driver for ST7735 LCD display controller
// Author: Chris Taylor (taylorza)
#include "mbed.h"
#include "Color565.h"

const uint16_t Color565::White = Color565::fromRGB(0xff, 0xff, 0xff);
const uint16_t Color565::Silver = Color565::fromRGB(0xc0, 0xc0, 0xc0);
const uint16_t Color565::Gray = Color565::fromRGB(0x80, 0x80, 0x80);
const uint16_t Color565::Black = Color565::fromRGB(0x00, 0x00, 0x00);
const uint16_t Color565::Red = Color565::fromRGB(0xff, 0x00, 0x00);
const uint16_t Color565::Maroon = Color565::fromRGB(0x80, 0x00, 0x00);
const uint16_t Color565::Yellow = Color565::fromRGB(0xff, 0xff, 0x00);
const uint16_t Color565::Olive = Color565::fromRGB(0x80, 0x80, 0x00);
const uint16_t Color565::Lime = Color565::fromRGB(0x00, 0xff, 0x00);
const uint16_t Color565::Green = Color565::fromRGB(0x00, 0x80, 0x00);
const uint16_t Color565::Aqua = Color565::fromRGB(0x00, 0xff, 0xff);
const uint16_t Color565::Teal = Color565::fromRGB(0x00, 0x80, 0x80);
const uint16_t Color565::Blue = Color565::fromRGB(0x00, 0x00, 0xff);
const uint16_t Color565::Navy = Color565::fromRGB(0x00, 0x00, 0x80);
const uint16_t Color565::Fuchsia = Color565::fromRGB(0xff, 0x00, 0xff);
const uint16_t Color565::Purple = Color565::fromRGB(0x80, 0x00, 0x80);