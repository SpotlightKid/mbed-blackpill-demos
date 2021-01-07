#include "perftest.h"
#if 0
    #include "font_IBM.h"
    #define FONT font_ibm
#else
    #include "font_OEM.h"
    #define FONT font_oem
#endif

using namespace std::chrono;

void bottomPrintf_ST7735(LCD_ST7735 &disp, const char *szFormat, ...) {
    char szBuffer[256];
    va_list args;
    uint8_t width, height;

    va_start(args, szFormat);
    vsprintf(szBuffer, szFormat, args);
    va_end(args);
    disp.measureString(FONT, szBuffer, width, height);
    disp.drawString(FONT,
                    disp.getWidth() / 2 - width  / 2,
                    disp.getHeight() - height, szBuffer);
}

/*
 * Test ST7735 library functions
 */
int testDisplayST7735(LCD_ST7735 &disp) {
    Timer tPerformance;
    char szTest[] = "123TestText456";
    int iPerfUs = 0;
    int iStartUs = 0;
    int elapsedUs;
    uint8_t width, height;

    tPerformance.start();  // start the timer
    //disp.setOrientation(LCD_ST7735::Rotate270, false);
    disp.setForegroundColor(Color565::Blue);
    disp.setBackgroundColor(Color565::Black);
    disp.clearScreen(Color565::Blue);
    wait_us(1000000);

    // text
    iStartUs = tPerformance.elapsed_time().count();
    disp.clearScreen();
    bottomPrintf_ST7735(disp, "drawString...");
    disp.measureString(FONT, szTest, width, height);
    for (int i = 1; i < 100; i++) {
        disp.drawString(FONT, disp.getWidth() / 2 - width / 2, i,
                        szTest);
    }
    elapsedUs = tPerformance.elapsed_time().count() - iStartUs;
    bottomPrintf_ST7735(disp, "drawString:%u", elapsedUs);
    iPerfUs += elapsedUs;
    wait_us(500000);

    // rectangles
    iStartUs = tPerformance.elapsed_time().count();
    disp.clearScreen();
    bottomPrintf_ST7735(disp, "drawRect...");
    for (int i = 1; i < 100; i++) {
        disp.drawRect(0, 0, i, i, Color565::Blue);
    }
    elapsedUs = tPerformance.elapsed_time().count() - iStartUs;
    bottomPrintf_ST7735(disp, "drawRect:%u",elapsedUs);
    iPerfUs += elapsedUs;
    wait_us(500000);

    // filled rectangles
    iStartUs = tPerformance.elapsed_time().count();
    disp.clearScreen();
    bottomPrintf_ST7735(disp, "fillRect...");
    for (int i = 1; i < 100; i += 2) {
        disp.fillRect(0, 0, i, i, Color565::Yellow, Color565::Blue);
    }
    elapsedUs = tPerformance.elapsed_time().count() - iStartUs;
    bottomPrintf_ST7735(disp, "fillRect:%u", elapsedUs);
    iPerfUs += elapsedUs;
    wait_us(500000);

    // circles
    iStartUs = tPerformance.elapsed_time().count();
    disp.clearScreen();
    bottomPrintf_ST7735(disp, "drawCircle...");
    for (int i = 1; i < 100; i++) {
        disp.drawCircle(i, i, i / 2, Color565::Blue);
    }
    elapsedUs = tPerformance.elapsed_time().count() - iStartUs;
    bottomPrintf_ST7735(disp, "drawCircle:%u", elapsedUs);
    iPerfUs += elapsedUs;
    wait_us(500000);

    // filled circles
    iStartUs = tPerformance.elapsed_time().count();
    disp.clearScreen();
    bottomPrintf_ST7735(disp, "fillCircle...");
    for (int i = 1; i < 100; i += 2) {
        disp.fillCircle(i, i, i / 2, Color565::Yellow, Color565::Blue);
    }
    elapsedUs = tPerformance.elapsed_time().count() - iStartUs;
    bottomPrintf_ST7735(disp, "fillCircle:%u", elapsedUs);
    iPerfUs += elapsedUs;
    wait_us(500000);

    // lines
    iStartUs = tPerformance.elapsed_time().count();
    disp.clearScreen();
    bottomPrintf_ST7735(disp, "drawLine...");
    for (int i = 0; i < 100; i++) {
        disp.drawLine(0, 0, i, 100, Color565::Blue);
    }
    elapsedUs = tPerformance.elapsed_time().count() - iStartUs;
    bottomPrintf_ST7735(disp, "drawLine:%u", elapsedUs);
    iPerfUs += elapsedUs;
    wait_us(500000);

    disp.clearScreen();
    bottomPrintf_ST7735(disp, "Total: %ums", iPerfUs / 1000);
    wait_us(500000);

    return (iPerfUs);
}
