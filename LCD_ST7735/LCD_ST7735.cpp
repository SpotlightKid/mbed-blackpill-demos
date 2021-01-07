
#include "mbed.h"
#include "LCD_ST7735.h"

LCD_ST7735::LCD_ST7735(PinName csPin, PinName rsPin, PinName rstPin,
                       PanelColorFilter colorFilter)
    : _cs(csPin, 1),
      _rs(rsPin, 0),
      _rst(rstPin, 1),
      _colorFilter(colorFilter) {
    _cs = 1;

    spi_init();
    setForegroundColor(0xffff);
    setBackgroundColor(0x0000);
}

void LCD_ST7735::setOrientation(Orientation orientation, bool flip) {
    const static uint8_t my = 0x80;
    const static uint8_t mx = 0x40;
    const static uint8_t mv = 0x20;
    // const static uint8_t ml = 0x10;
    // const static uint8_t mh = 0x04;

    uint8_t madctlData = _colorFilter;
    switch (orientation) {
        case Rotate0:
            _width = 128;
            _height = 160;
            madctlData |= flip ? mx : 0;
            break;

        case Rotate90:
            _width = 160;
            _height = 128;
            madctlData |= flip ? my | mv | mx : mv | mx;
            break;

        case Rotate180:
            _width = 128;
            _height = 160;
            madctlData |= flip ? my : mx | my;
            break;

        case Rotate270:
            _width = 160;
            _height = 128;
            madctlData |= flip ? mv : mv | my;
            break;
    }
    write(CMD_MADCTL, madctlData);
}

int LCD_ST7735::getWidth() { return _width; }

int LCD_ST7735::getHeight() { return _height; }

void LCD_ST7735::clearScreen(uint16_t color) {
    uint16_t pixel[320];

    clipRect(0, 0, _width - 1, _height - 1);
    beginBatchCommand(CMD_RAMWR);
    for (int i = 0; i < _height; i++) {
        for (int j = 0; j < _width; j++) {
            pixel[j] = color;
        }
        spi_write((uint8_t*)pixel, _width * 2);
    }
    endBatchCommand();
}

void LCD_ST7735::setPixel(int x, int y, uint16_t color) {
    uint8_t CASET[] = {0, (uint8_t)x, 0, (uint8_t)x};
    write(CMD_CASET, CASET, 4);
    uint8_t RASET[] = {0, (uint8_t)y, 0, (uint8_t)y};
    write(CMD_RASET, RASET, 4);
    write16(CMD_RAMWR, color);
}

void LCD_ST7735::drawLine(int x1, int y1, int x2, int y2, uint16_t color) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    if (dx == 0) {
        drawVertLine(x1, y1, y2, color);
        return;
    } else if (dy == 0) {
        drawHorizLine(x1, y1, x2, color);
        return;
    }

    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    while (x1 != x2 || y1 != y2) {
        setPixel(x1, y1, color);
        int e2 = err << 1;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
    setPixel(x2, y2, color);
}

void LCD_ST7735::swap(int &a, int &b) {
    int t = a;
    a = b;
    b = t;
}

void LCD_ST7735::drawRect(int x1, int y1, int x2, int y2, uint16_t color) {
    if (x1 > x2) swap(x1, x2);
    if (y1 > y2) swap(y1, y2);

    drawHorizLine(x1, y1, x2, color);
    drawHorizLine(x1, y2, x2, color);
    drawVertLine(x1, y1, y2, color);
    drawVertLine(x2, y1, y2, color);
}

void LCD_ST7735::drawCircle(int x, int y, int r, uint16_t color) {
    int ix = r;
    int iy = 0;
    int err = 1 - r;

    while (ix >= iy) {
        setPixel(x + ix, y + iy, color);
        setPixel(x + iy, y + ix, color);
        setPixel(x - ix, y + iy, color);
        setPixel(x - iy, y + ix, color);
        setPixel(x - ix, y - iy, color);
        setPixel(x - iy, y - ix, color);
        setPixel(x + ix, y - iy, color);
        setPixel(x + iy, y - ix, color);
        iy++;
        if (err < 0) {
            err += 2 * iy + 1;
        } else {
            ix--;
            err += 2 * (iy - ix + 1);
        }
    }
}

void LCD_ST7735::drawEllipse(int x, int y, int rx, int ry, uint16_t color) {
    int a2 = rx * rx;
    int b2 = ry * ry;
    int fa2 = 4 * a2;
    int fb2 = 4 * b2;

    int ix, iy, sigma;
    for (ix = 0, iy = ry, sigma = 2 * b2 + a2 * (1 - 2 * ry);
         b2 * ix <= a2 * iy; ix++) {
        setPixel(x + ix, y + iy, color);
        setPixel(x - ix, y + iy, color);
        setPixel(x + ix, y - iy, color);
        setPixel(x - ix, y - iy, color);
        if (sigma >= 0) {
            sigma += fa2 * (1 - iy);
            iy--;
        }
        sigma += b2 * ((4 * ix) + 6);
    }

    for (ix = rx, iy = 0, sigma = 2 * a2 + b2 * (1 - 2 * rx);
         a2 * iy <= b2 * ix; iy++) {
        setPixel(x + ix, y + iy, color);
        setPixel(x - ix, y + iy, color);
        setPixel(x + ix, y - iy, color);
        setPixel(x - ix, y - iy, color);
        if (sigma >= 0) {
            sigma += fb2 * (1 - ix);
            ix--;
        }
        sigma += a2 * ((4 * iy) + 6);
    }
}
void LCD_ST7735::fillRect(int x1, int y1, int x2, int y2, uint16_t fillColor) {
    if (x1 > x2) swap(x1, x2);
    if (y1 > y2) swap(y1, y2);

    clipRect(x1, y1, x2, y2);
    int c = (((x2 - x1) + 1) * ((y2 - y1) + 1));
    beginBatchCommand(CMD_RAMWR);
    while (c--) {
        writeBatchData(fillColor);
    }
    endBatchCommand();
}

void LCD_ST7735::fillRect(int x1, int y1, int x2, int y2, uint16_t borderColor,
                          uint16_t fillColor) {
    if (x1 > x2) swap(x1, x2);
    if (y1 > y2) swap(y1, y2);

    drawRect(x1, y1, x2, y2, borderColor);
    ++x1;
    ++y1;
    --x2;
    --y2;
    if (x2 >= x1 && y2 >= y1) {
        int c = (((x2 - x1) + 1) * ((y2 - y1) + 1));

        clipRect(x1, y1, x2, y2);
        beginBatchCommand(CMD_RAMWR);
        while (c--) {
            writeBatchData(fillColor);
        }
        endBatchCommand();
    }
}

void LCD_ST7735::fillCircle(int x, int y, int r, uint16_t borderColor,
                            uint16_t fillColor) {
    int ix = r;
    int iy = 0;
    int err = 1 - r;

    while (ix >= iy) {
        setPixel(x - ix, y + iy, borderColor);
        setPixel(x + ix, y + iy, borderColor);
        drawHorizLine(x - ix + 1, y + iy, x + ix - 1, fillColor);

        setPixel(x - iy, y + ix, borderColor);
        setPixel(x + iy, y + ix, borderColor);
        drawHorizLine(x - iy + 1, y + ix, x + iy - 1, fillColor);

        setPixel(x - ix, y - iy, borderColor);
        setPixel(x + ix, y - iy, borderColor);
        drawHorizLine(x - ix + 1, y - iy, x + ix - 1, fillColor);

        setPixel(x - iy, y - ix, borderColor);
        setPixel(x + iy, y - ix, borderColor);
        drawHorizLine(x - iy + 1, y - ix, x + iy - 1, fillColor);
        iy++;
        if (err < 0) {
            err += 2 * iy + 1;
        } else {
            ix--;
            err += 2 * (iy - ix + 1);
        }
    }
}

void LCD_ST7735::fillEllipse(int x, int y, int rx, int ry, uint16_t borderColor,
                             uint16_t fillColor) {
    int a2 = rx * rx;
    int b2 = ry * ry;
    int fa2 = 4 * a2;
    int fb2 = 4 * b2;

    int ix, iy, sigma;
    for (ix = 0, iy = ry, sigma = 2 * b2 + a2 * (1 - 2 * ry);
         b2 * ix <= a2 * iy; ix++) {
        setPixel(x + ix, y + iy, borderColor);
        setPixel(x - ix, y + iy, borderColor);
        drawHorizLine(x - ix + 1, y + iy, x + ix - 1, fillColor);

        setPixel(x + ix, y - iy, borderColor);
        setPixel(x - ix, y - iy, borderColor);
        drawHorizLine(x - ix + 1, y - iy, x + ix - 1, fillColor);

        if (sigma >= 0) {
            sigma += fa2 * (1 - iy);
            iy--;
        }
        sigma += b2 * ((4 * ix) + 6);
    }

    for (ix = rx, iy = 0, sigma = 2 * a2 + b2 * (1 - 2 * rx);
         a2 * iy <= b2 * ix; iy++) {
        setPixel(x + ix, y + iy, borderColor);
        setPixel(x - ix, y + iy, borderColor);
        drawHorizLine(x - ix + 1, y + iy, x + ix - 1, fillColor);

        setPixel(x + ix, y - iy, borderColor);
        setPixel(x - ix, y - iy, borderColor);
        drawHorizLine(x - ix + 1, y - iy, x + ix - 1, fillColor);
        if (sigma >= 0) {
            sigma += fb2 * (1 - ix);
            ix--;
        }
        sigma += a2 * ((4 * iy) + 6);
    }
}

void LCD_ST7735::drawBitmap(int x, int y, const uint16_t *pbmp) {
    int w = *pbmp++;
    int h = *pbmp++;

    clip(x, y, w, h);
    int c = w * h;
    beginBatchCommand(CMD_RAMWR);
    while (c--) {
        writeBatchData(*pbmp++);
    }
    endBatchCommand();
}

void LCD_ST7735::drawBitmap(int x, int y, const uint16_t *pbmp, int srcX,
                            int srcY, int srcWidth, int srcHeight) {
    int w = *pbmp++;
    int h = *pbmp++;

    if (h < srcHeight) {
        return;
    }

    clip(x, y, srcWidth, srcHeight);
    beginBatchCommand(CMD_RAMWR);
    const uint16_t *p = pbmp + srcX + (srcY * w);
    for (int iy = 0; iy < srcHeight; ++iy) {
        for (int ix = 0; ix < srcWidth; ++ix) {
            writeBatchData(*(p + ix));
        }
        p += w;
    }
    endBatchCommand();
}

void LCD_ST7735::setForegroundColor(uint16_t color) {
    _foregroundColor = color;
}

void LCD_ST7735::setBackgroundColor(uint16_t color) {
    _backgroundColor = color;
}

void LCD_ST7735::drawString(const uint8_t *pFont, int x, int y,
                            const char *pString) {
    uint8_t w = *pFont;
    uint8_t h = *(pFont + 1);
    uint8_t offset = *(pFont + 2);
    uint8_t leftPad = *(pFont + 3);
    uint8_t rightPad = *(pFont + 4);
    uint8_t topPad = *(pFont + 5);
    uint8_t bottomPad = *(pFont + 6);

    if (y + topPad + h + bottomPad < 0) return;
    if (y >= _height) return;
    if (x + leftPad + w + rightPad < 0) return;

    char *p = (char *)pString;
    while (*p != 0) {
        if (x >= _width) return;
        drawChar(pFont, x, y, *p++, w, h, offset, leftPad, rightPad, topPad,
                 bottomPad);
        x += (w + leftPad + rightPad);
    }
}

void LCD_ST7735::measureString(const uint8_t *pFont, const char *pString,
                               uint8_t &width, uint8_t &height) {
    uint8_t w = *pFont;
    uint8_t h = *(pFont + 1);
    uint8_t leftPad = *(pFont + 3);
    uint8_t rightPad = *(pFont + 4);
    uint8_t topPad = *(pFont + 5);
    uint8_t bottomPad = *(pFont + 6);

    width = (w + leftPad + rightPad) * strlen(pString);
    height = (h + topPad + bottomPad);
}

void LCD_ST7735::drawVertLine(int x1, int y1, int y2, uint16_t color) {
    clipRect(x1, y1, x1, y2);
    beginBatchCommand(CMD_RAMWR);
    int c = (y2 - y1) << 1;
    for (int i = 0; i < c; ++i) {
        writeBatchData(color);
    }
    endBatchCommand();
}

void LCD_ST7735::drawHorizLine(int x1, int y1, int x2, uint16_t color) {
    clipRect(x1, y1, x2, y1);
    beginBatchCommand(CMD_RAMWR);
    int c = (x2 - x1) << 1;
    for (int i = 0; i < c; ++i) {
        writeBatchData(color);
    }
    endBatchCommand();
}

void LCD_ST7735::drawChar(const uint8_t *pFont, int x, int y, char c, uint8_t w,
                          uint8_t h, uint8_t offset, uint8_t leftPad,
                          uint8_t rightPad, uint8_t topPad, uint8_t bottomPad) {
    const uint8_t *pChar = (pFont + 7) + ((c - offset) * h);

    clip(x, y, w + leftPad + rightPad, h + topPad + bottomPad);

    beginBatchCommand(CMD_RAMWR);

    // Render top spacing
    for (int r = 0; r < topPad; ++r) {
        for (int c = 0; c < w + leftPad + rightPad; ++c) {
            writeBatchData(_backgroundColor);
        }
    }

    // Render character
    for (int r = 0; r < h; ++r) {
        uint8_t b = pChar[r];

        // Render left spacing
        for (int c = 0; c < leftPad; ++c) {
            writeBatchData(_backgroundColor);
        }
        for (int c = 0; c < w; ++c) {
            if (b & 0x80) {
                writeBatchData(_foregroundColor);
            } else {
                writeBatchData(_backgroundColor);
            }

            b <<= 1;
        }

        for (int c = 0; c < rightPad; ++c) {
            writeBatchData(_backgroundColor);
        }
    }

    // Render bottom spacing
    for (int r = 0; r < bottomPad; ++r) {
        for (int c = 0; c < w + leftPad + rightPad; ++c) {
            writeBatchData(_backgroundColor);
        }
    }
    endBatchCommand();
}

void LCD_ST7735::initDisplay() {
    _cs = 0;
    reset();

    // Software reset, w/delay 150 ms
    writeCommand(CMD_SWRESET);
    wait_us(150 * 1000);
    // Out of sleep mode, w/delay 250 ms
    writeCommand(CMD_SLPOUT);
    wait_us(250 * 1000);

    uint8_t FRMCTR1[] = {0x01, 0x2c, 0x2d};
    write(CMD_FRMCTR1, FRMCTR1, 3);
    uint8_t FRMCTR2[] = {0x01, 0x2c, 0x2d};
    write(CMD_FRMCTR2, FRMCTR2, 3);
    uint8_t FRMCTR3[] = {0x01, 0x2c, 0x2d, 0x01, 0x2c, 0x2d};
    write(CMD_FRMCTR3, FRMCTR3, 6);

    write(CMD_INVCTR, 0x07);

    uint8_t PWCTR1[] = {0xa2, 0x02, 0x84};
    write(CMD_PWCTR1, PWCTR1, 3);
    write(CMD_PWCTR2, 0xc5);
    uint8_t PWCTR3[] = {0x0a, 0x00};
    write(CMD_PWCTR3, PWCTR3, 2);
    uint8_t PWCTR4[] = {0x8a, 0x2a};
    write(CMD_PWCTR4, PWCTR4, 2);
    uint8_t PWCTR5[] = {0x8a, 0xee};
    write(CMD_PWCTR5, PWCTR5, 2);

    write(CMD_VMCTR1, 0x0e);
    write(CMD_INVOFF, 0x0);

    // default orientation
    setOrientation(Rotate0, false);

    // 65k color mode
    write(CMD_COLMOD, 0x05);

    // Column addr set
    uint8_t CASET[] = {0x00, 0x00, 0x00, (uint8_t)(_width - 1)};
    write(CMD_CASET, CASET, 4);
    // Row addr set
    uint8_t RASET[] = {0x00, 0x00, 0x00, (uint8_t)(_height - 1)};
    write(CMD_RASET, RASET, 4);

    // Gama sequence
    // 1: Magical unicorn dust, 16 args, no delay
    uint8_t GAMCTRP1[] = {
        0x02, 0x1c, 0x07, 0x12,
        0x37, 0x32, 0x29, 0x2d,
        0x29, 0x25, 0x2B, 0x39,
        0x00, 0x01, 0x03, 0x10
    };
    write(CMD_GAMCTRP1, GAMCTRP1, 16);

    // 2: Sparkles and rainbows, 16 args, no delay
    uint8_t GAMCTRN1[] = {
        0x03, 0x1d, 0x07, 0x06,
        0x2E, 0x2C, 0x29, 0x2D,
        0x2E, 0x2E, 0x37, 0x3F,
        0x00, 0x00, 0x02, 0x10
    };
    write(CMD_GAMCTRN1, GAMCTRN1, 16);

    // Normal display on, no args, w/delay 10 ms
    writeCommand(CMD_NORON);
    wait_us(10 * 1000);

    // Enable display, w/delay 10 0ms
    writeCommand(CMD_DISPON);
    wait_us(100 * 1000);
}

void LCD_ST7735::reset() {
    _rst = 0;
    wait_us(100);
    _rst = 1;
    wait_us(100);
}

void LCD_ST7735::clip(int x, int y, int w, int h) {
    clipRect(x, y, (x + w) - 1, (y + h) - 1);
}

void LCD_ST7735::clipRect(int x1, int y1, int x2, int y2) {
    uint8_t x1l = (uint8_t)x1;
    uint8_t x1h = (uint8_t)(x1 >> 8);
    uint8_t x2l = (uint8_t)x2;
    uint8_t x2h = (uint8_t)(x2 >> 8);
    uint8_t CASET[] = {x1h, x1l, x2h, x2l};
    write(CMD_CASET, CASET, 4);

    uint8_t y1l = (uint8_t)y1;
    uint8_t y1h = (uint8_t)(y1 >> 8);
    uint8_t y2l = (uint8_t)y2;
    uint8_t y2h = (uint8_t)(y2 >> 8);
    uint8_t RASET[] = {y1h, y1l, y2h, y2l};
    write(CMD_RASET, RASET, 4);
}

void LCD_ST7735::writeCommand(uint8_t cmd) {
    _cs = 0;
    _rs = 0;
    spi_write(cmd);
    _cs = 1;
}

void LCD_ST7735::write(uint8_t cmd, uint8_t* data, uint16_t dataLen) {
    _cs = 0;
    _rs = 0;
    spi_write(cmd);
    if (data != NULL && dataLen > 0) {
        _rs = 1;
        spi_write(data, dataLen);
        _rs = 0;
    }
    _cs = 1;
}

void LCD_ST7735::write(uint8_t cmd, uint8_t data) {
    _cs = 0;
    _rs = 0;
    spi_write(cmd);
    _rs = 1;
    spi_write(data);
    _rs = 0;
    _cs = 1;
}

void LCD_ST7735::write16(uint8_t cmd, uint16_t data) {
    _cs = 0;
    _rs = 0;
    spi_write(cmd);
    _rs = 1;
    spi_writew(data);
    _rs = 0;
    _cs = 1;
}

void LCD_ST7735::beginBatchCommand(uint8_t cmd) {
    _cs = 0;
    _rs = 0;
    spi_write(cmd);
    _rs = 1;
}

void LCD_ST7735::writeBatchData(uint8_t data) { spi_write(data); }

void LCD_ST7735::writeBatchData(uint8_t dataHigh, uint8_t dataLow) {
    spi_write(dataHigh);
    spi_write(dataLow);
}

void LCD_ST7735::writeBatchData(uint16_t data) {
    spi_writew(data);
}

void LCD_ST7735::endBatchCommand() {
    _rs = 0;
    _cs = 1;
}
