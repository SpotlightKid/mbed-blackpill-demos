///////////////////////////////////////////////////////////////////////////////
// LCD_ST7735 - Driver for ST7735 LCD display controller
// Author: Chris Taylor (taylorza)

#pragma once

#include <stdint.h>
#include <stddef.h>

#include "spidma.h"

#ifndef MIN
#define MIN(a,b) ( (a) < (b) ? (a) : (b) )
#endif

#ifndef MAX
#define MAX(a,b) ( (a) > (b) ? (a) : (b) )
#endif

#define ST7735_HEIGHT 160
#define ST7735_WIDTH  128
#define ST7735_MAX_DIM MAX(ST7735_HEIGHT, ST7735_WIDTH)

/**
 * LCD_ST7735 is a simple driver for the ST7735 LCD controller.
 * It provides basic drawing primitives sa well as text and font capabilities.
 * The driver is currently hardcoded to support 65K colors using a 565 RGB pixel
 * format.
 */
class LCD_ST7735 {
   public:
    /** Orientation of the display */
    enum Orientation {
        /** No rotation of the display image*/
        Rotate0,
        /** Rotate the display image 90 degrees */
        Rotate90,
        /** Rotate the display image 180 degrees */
        Rotate180,
        /** Rotate the display image 270 degrees */
        Rotate270
    };

    /** Type of color filter of the panel */
    enum PanelColorFilter {
        /** RGB color filter panel */
        RGB = 0,

        /** BGR color filter panel */
        BGR = 8,
    };

   public:
    /**Creates an instance of the LCD_ST7735 driver
     * @param csPin SPI chip select pin
     * @param rsPin Pin used to put the display controller into data mode
     * @param rstPin Pin used to reset the display controller
     * @param colorFilter Type of color filter of the panel, BGR (default) or RGB
     */
    LCD_ST7735(PinName csPin, PinName rsPin, PinName rstPin,
               PanelColorFilter colorFilter = BGR);

    /** Initialize the display */
    void initDisplay();

    /** Set the orientation of the display
     * @param orientation Orientation of the display.
     * @param flip Flips the display direction
     */
    void setOrientation(Orientation orientation, bool flip);

    /** Get the width of the display given the current orientation */
    int getWidth();

    /** Get the height of the display given the current orientation */
    int getHeight();

    /** Clear the screen
     * @param color The color used to clear the screen. Defaults to black if not
     * passed.
     */
    void clearScreen(uint16_t color = 0x0000);

    /**  Set a pixel on the display to the specified color
     * @param x The X coordinate of the pixel (0..127)
     * @param y The Y coordinate of the pixel (0..159)
     * @param color Color to set the pixel to.
     */
    void setPixel(int x, int y, uint16_t color);

    /** Draw a line on the display
     * @param x1 The X coordinate of the starting point on the line
     * @param y1 The Y coordinate of the starting point on the line
     * @param x2 The X coordinate of the end point on the line
     * @param y2 The Y coordinate of the end point on the line
     * @param color The color used to draw the pixel
     */
    void drawLine(int x1, int y1, int x2, int y2, uint16_t color);

    /** Draw a rectangle on the display
     * @param x1 The X coordinate of the upper left corner
     * @param y1 The Y coordinate of the upper left corner
     * @param x2 The X coordinate of the lower right corner
     * @param y2 The Y coordinate of the lower right corner
     * @param color The color used to draw the rectangle
     */
    void drawRect(int x1, int y1, int x2, int y2, uint16_t color);

    /** Draw a circle on the display
     * @param x The X coordinate of the center of the circle
     * @param y The Y coordinate of the center of the circle
     * @param r The radius of the circle
     * @param color The color used to draw the circle
     */
    void drawCircle(int x, int y, int r, uint16_t color);

    /** Draw an ellipse on the display
     * @param x The X coordinate of the center of the ellipse
     * @param y The Y coordinate of the center of the ellipse
     * @param rx The X radius of the ellipse
     * @param ry The X radius of the ellipse
     * @param color The color used to draw the ellipse
     */
    void drawEllipse(int x, int y, int rx, int ry, uint16_t color);

    /** Draw a filled rectangle on the display
     * @param x1 The X coordinate of the upper left corner
     * @param y1 The Y coordinate of the upper left corner
     * @param x2 The X coordinate of the lower right corner
     * @param y2 The Y coordinate of the lower right corner
     * @param fillColor The color used to fill the rectangle
     */
    void fillRect(int x1, int y1, int x2, int y2, uint16_t fillColor);

    /** Draw a filled rectangle on the display
     * @param x1 The X coordinate of the upper left corner
     * @param y1 The Y coordinate of the upper left corner
     * @param x2 The X coordinate of the lower right corner
     * @param y2 The Y coordinate of the lower right corner
     * @param borderColor The color used to draw the rectangle frame
     * @param fillColor The color used to fill the rectangle
     */
    void fillRect(int x1, int y1, int x2, int y2, uint16_t borderColor,
                  uint16_t fillColor);

    /** Draw a filled circle on the display
     * @param x The X coordinate of the center of the circle
     * @param y The Y coordinate of the center of the circle
     * @param borderColor The color used to draw the circumference of the circle
     * @param fillColor The color used to fill the circle
     */
    void fillCircle(int x, int y, int r, uint16_t borderColor,
                    uint16_t fillColor);

    /** Draw a filled ellipse on the display
     * @param x The X coordinate of the center of the ellipse
     * @param y The Y coordinate of the center of the ellipse
     * @param rx The X radius of the ellipse
     * @param ry The X radius of the ellipse
     * @param borderColor The color used to draw the circumference of the circle
     * @param fillColor The color used to fill the circle
     */
    void fillEllipse(int x, int y, int rx, int ry, uint16_t borderColor,
                     uint16_t fillColor);

    /** Draw a bitmap on the screen
     * @param x The X coordinate location to draw the bitmap.
     * @param y The Y coordinate location to draw the bitmap.
     * @param pbmp Pointer to the bitmap.
     * @note The bitmap is an single dimensional uint8_t (unsigned short) array.
     * The first to elements of the array indicate the width and height of the
     * bitmap repectively. The rest of the entries int the array make up the
     * pixel data for the array.
     */
    void drawBitmap(int x, int y, const uint16_t *pbmp);

    /** Extracts a portion of a bitmap and draws it on the screen
     * @param x The X coordinate location to draw the bitmap.
     * @param y The Y coordinate location to draw the bitmap.
     * @param pbmp Pointer to the bitmap.
     * @param srcX X offset into the source bitmap of the portion to extract
     * @param srcY Y offset into the source bitmap of the portion to extract
     * @param srcWidth Width of the bitmap portion to draw
     * @param srcHeight Height of the bitmap portion to draw
     * @note The bitmap is an single dimensional uint8_t (unsigned short) array.
     * The first to elements of the array indicate the width and height of the
     * bitmap repectively. The rest of the entries int the array make up the
     * pixel data for the array.
     */
    void drawBitmap(int x, int y, const uint16_t *pbmp, int srcX, int srcY,
                    int srcWidth, int srcHeight);

    /** Set the foreground color used to render text
     * @param color Color used when drawing text to the display
     * @note The color can be changed multiple times to render text in various
     * colors on the display
     */
    void setForegroundColor(uint16_t color);

    /** Set the background color used to render text
     * @param color Color used when drawing background portions of the text
     * @note The color can be changed multiple times to render text with various
     * background colors on the display
     */
    void setBackgroundColor(uint16_t color);

    /** Draw a string to the screen using the currently active foreground and
     * background colors
     * @param pFont Pointer to the font used to render the string to the display
     * @param x The X coordinate location to draw the string.
     * @param y The Y coordinate location to draw the string.
     * @param pString ASCIIZ string to draw to the display.
     */
    void drawString(const uint8_t *pFont, int x, int y, const char *pString);

    /** Measure the width and height of the string when rendered with the
     * specified font
     * @param pFont Pointer to the font used to measure the string
     * @param pString ASCIIZ string to measure.
     * @param width Reference to the variable that will contain the width
     * @param height Reference to the variable that will contain the height
     */
    void measureString(const uint8_t *pFont, const char *pString,
                       uint8_t &width, uint8_t &height);

   protected:
    void writeCommand(uint8_t cmd);
    void write(uint8_t cmd, uint8_t* data, uint16_t dataLen);
    void write(uint8_t cmd, uint8_t data);
    void write16(uint8_t cmd, uint16_t data);

    void beginBatchCommand(uint8_t cmd);
    void writeBatchData(uint8_t data);
    void writeBatchData(uint8_t dataHigh, uint8_t dataLow);
    void writeBatchData(uint16_t data);
    void endBatchCommand();

    void clip(int x, int y, int w, int h);
    void clipRect(int x1, int y1, int x2, int y2);

   private:
    void drawVertLine(int x1, int y1, int y2, uint16_t color);
    void drawHorizLine(int x1, int y1, int x2, uint16_t color);
    void drawChar(const uint8_t *pFont, int x, int y, char c, uint8_t w,
                  uint8_t h, uint8_t offset, uint8_t leftPad, uint8_t rightPad,
                  uint8_t topPad, uint8_t bottomPad);

   private:
    void swap(int &a, int &b);

   private:
    void reset();

   private:
    int _width;
    int _height;
    Orientation _orientation;
    bool _flip;
    uint16_t _foregroundColor;
    uint16_t _backgroundColor;

   private:
    DigitalOut _cs;
    DigitalOut _rs;
    DigitalOut _rst;
    PanelColorFilter _colorFilter;

   protected:
    static const uint8_t CMD_SWRESET = 0x01;
    static const uint8_t CMD_SLPOUT = 0x11;
    static const uint8_t CMD_DISPON = 0x29;
    static const uint8_t CMD_CASET = 0x2a;
    static const uint8_t CMD_RASET = 0x2b;
    static const uint8_t CMD_RAMWR = 0x2c;

    static const uint8_t CMD_MADCTL = 0x36;
    static const uint8_t CMD_COLMOD = 0x3a;

    static const uint8_t CMD_FRMCTR1 = 0xb1;
    static const uint8_t CMD_FRMCTR2 = 0xb2;
    static const uint8_t CMD_FRMCTR3 = 0xb3;
    static const uint8_t CMD_INVCTR = 0xb4;
    static const uint8_t CMD_INVOFF = 0x20;
    static const uint8_t CMD_INVON = 0x21;

    static const uint8_t CMD_PWCTR1 = 0xc0;
    static const uint8_t CMD_PWCTR2 = 0xc1;
    static const uint8_t CMD_PWCTR3 = 0xc2;
    static const uint8_t CMD_PWCTR4 = 0xc3;
    static const uint8_t CMD_PWCTR5 = 0xc4;
    static const uint8_t CMD_VMCTR1 = 0xc5;

    static const uint8_t CMD_GAMCTRP1 = 0xe0;
    static const uint8_t CMD_GAMCTRN1 = 0xe1;

    static const uint8_t CMD_NORON = 0x13;
    static const uint8_t CMD_EXTCTRL = 0xf0;
};
