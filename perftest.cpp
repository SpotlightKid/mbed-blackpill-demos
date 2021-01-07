#include "perftest.h"
#include "font_IBM.h"

void bottomPrintf_ST7735(LCD_ST7735 &disp, const char *szFormat, ...) {
    char szBuffer[256];
    va_list args;

    va_start(args, szFormat);
    vsprintf(szBuffer, szFormat, args);
    va_end(args);
    disp.drawString(font_ibm,
                    disp.getWidth() / 2 - (8 + 0) * strlen(szBuffer) / 2,
                    disp.getHeight() - 8, szBuffer);
}

/*
 * Test ST7735 library functions
 */
int testDisplayST7735(LCD_ST7735 &disp) {
    Timer tPerformance;
    char szTest[] = "123TestText456";
    int iPerfMs = 0;
    int iStartUs = 0;

    tPerformance.start();  // start the timer
    disp.setOrientation(LCD_ST7735::Rotate270, false);
    disp.setForegroundColor(Color565::Blue);
    disp.setBackgroundColor(Color565::Black);
    disp.clearScreen();

    // text
    iStartUs = tPerformance.read_us();
    disp.clearScreen();
    bottomPrintf_ST7735(disp, "drawString...");
    for (int i = 1; i < 100; i++) {
        // disp.drawString(font_ibm, disp.getWidth / 2 - (8 + 0) * strlen(buf) /
        // 2, i, buf);
        disp.drawString(font_ibm, 160 / 2 - (8 + 0) * strlen(szTest) / 2, i,
                        szTest);
    }
    bottomPrintf_ST7735(disp, "drawString:%u ",
                        tPerformance.read_us() - iStartUs);
    iPerfMs += tPerformance.read_ms();
    wait_us(500000);

    // rectangles
    iStartUs = tPerformance.read_us();
    disp.clearScreen();
    bottomPrintf_ST7735(disp, "drawRect...");
    for (int i = 1; i < 100; i++) {
        disp.drawRect(0, 0, i, i, Color565::Blue);
    }
    bottomPrintf_ST7735(disp, "drawRect:%u ",
                        tPerformance.read_us() - iStartUs);
    iPerfMs += tPerformance.read_ms();
    wait_us(500000);

    // filled rectangles
    iStartUs = tPerformance.read_us();
    disp.clearScreen();
    bottomPrintf_ST7735(disp, "fillRect...");
    for (int i = 1; i < 100; i += 2) {
        disp.fillRect(0, 0, i, i, Color565::Yellow, Color565::Blue);
    }
    bottomPrintf_ST7735(disp, "fillRect:%u ",
                        tPerformance.read_us() - iStartUs);
    iPerfMs += tPerformance.read_ms();
    wait_us(500000);

    // circles
    iStartUs = tPerformance.read_us();
    disp.clearScreen();
    bottomPrintf_ST7735(disp, "drawCircle...");
    for (int i = 1; i < 100; i++) {
        disp.drawCircle(i, i, i / 2, Color565::Blue);
    }
    bottomPrintf_ST7735(disp, "drawCircle:%u ",
                        tPerformance.read_us() - iStartUs);
    iPerfMs += tPerformance.read_ms();
    wait_us(500000);

    // filled circles
    iStartUs = tPerformance.read_us();
    disp.clearScreen();
    bottomPrintf_ST7735(disp, "fillCircle...");
    for (int i = 1; i < 100; i += 2) {
        disp.fillCircle(i, i, i / 2, Color565::Yellow, Color565::Blue);
    }
    bottomPrintf_ST7735(disp, "fillCircle:%u ",
                        tPerformance.read_us() - iStartUs);
    iPerfMs += tPerformance.read_ms();
    wait_us(500000);

    // lines
    iStartUs = tPerformance.read_us();
    disp.clearScreen();
    bottomPrintf_ST7735(disp, "drawLine...");
    for (int i = 1; i < 100; i++) {
        disp.drawLine(0, 0, i, 100, Color565::Blue);
    }
    bottomPrintf_ST7735(disp, "drawLine:%u ",
                        tPerformance.read_us() - iStartUs);
    iPerfMs += tPerformance.read_ms();
    wait_us(500000);

    disp.clearScreen();
    bottomPrintf_ST7735(disp, "Total:%u ms", iPerfMs);
    wait_us(500000);

    return (iPerfMs);
}
