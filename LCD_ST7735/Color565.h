///////////////////////////////////////////////////////////////////////////////
// LCD_ST7735 - Driver for ST7735 LCD display controller
// Author: Chris Taylor (taylorza)

#ifndef __COLOR565_H__
#define __COLOR565_H__
/** Color palete support for 16bit RGB colors with 565 color format
*/
class Color565
{
public:
    /**White*/
    static const uint16_t White;
    
    /**Silver*/
    static const uint16_t Silver;
    
    /**Gray*/
    static const uint16_t Gray;
    
    /**Black*/
    static const uint16_t Black;
    
    /**Red*/
    static const uint16_t Red;
    
    /**Maroon*/
    static const uint16_t Maroon;
    
    /**Yellow*/
    static const uint16_t Yellow;
    
    /**Olive*/
    static const uint16_t Olive;
    
    /**Lime*/
    static const uint16_t Lime;
    
    /**Green*/
    static const uint16_t Green;
    
    /**Aqua*/
    static const uint16_t Aqua;
    
    /**Teal*/
    static const uint16_t Teal;
    
    /**Blue*/
    static const uint16_t Blue;
    
    /**Navy*/
    static const uint16_t Navy;
    
    /**Fuchsia*/
    static const uint16_t Fuchsia;
    
    /**Purple*/
    static const uint16_t Purple;

    /**Returns a 565 RGB color built from individual RGB color components
     * @param r Red component
     * @param g Green component
     * @param b Blue component
    */    
    static uint16_t fromRGB(uint8_t r, uint8_t g, uint8_t b)
    {
        return ((r & 0xf8) << 8) | ((g & 0xfc) << 3) | (b >> 3)  ;
    }
private:
    Color565(){};  
};
#endif // __COLOR565_H__