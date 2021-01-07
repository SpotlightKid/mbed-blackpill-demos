#include <stdarg.h>

#include "mbed.h"
#include "LCD_ST7735.h"
#include "Color565.h"

void bottomPrintf_ST7735(LCD_ST7735 &disp, const char *szFormat, ...);

int testDisplayST7735(LCD_ST7735 &disp);
