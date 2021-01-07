#include "mbed.h"
#include "PinDetect.h"
#include "LCD_ST7735.h"
#include "Color565.h"
#include "perftest.h"

#define THRESHOLD 0.008

DigitalOut  led1(LED_BLUE);
PinDetect   btn(USER_BUTTON);
LCD_ST7735 disp(
    PA_4,       // SS/CS
    PC_14,      // RS/DS/DC
    PC_15,      // RST
    LCD_ST7735::RGB
);

void btn_pressed() {
    led1 = !led1;
    if (led1) {
        disp.fillRect(
            disp.getWidth() / 2 - 16,
            disp.getHeight() / 2 - 16,
            disp.getWidth() / 2 + 16,
            disp.getHeight() / 2 + 16,
            Color565::Fuchsia
        );
    } else {
        disp.fillRect(
            disp.getWidth() / 2 - 16,
            disp.getHeight() / 2 - 16,
            disp.getWidth() / 2 + 16,
            disp.getHeight() / 2 + 16,
            Color565::Aqua
        );
    }
}

int main() {
    // Request the shared queue
    EventQueue *queue = mbed_event_queue();

    led1 = 0;

    disp.initDisplay();

    btn.mode(PullUp);
    btn.setSamplesTillAssert(3);
    btn.attach_asserted(queue->event(btn_pressed));

    testDisplayST7735(disp);

    btn.setSampleFrequency();
    queue->dispatch_forever();
}
