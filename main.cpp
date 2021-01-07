#include "mbed.h"
//~#include "Joystick.h"
#include "PinDetect.h"
#include "LCD_ST7735.h"
#include "Color565.h"

#define THRESHOLD 0.008

DigitalOut  led1(LED_BLUE);
PinDetect   btn(USER_BUTTON);
//~Joystick    stk(A1, A2);
//~Vector2D old_coord = {0.0f, 0.0f};
LCD_ST7735 disp(
    SPI_MOSI,   // MOSI
    NC,         // MISO (unused)
    SPI_SCK,    // SCK/CLK
    PA_4,       // SS/CS
    PC_14,      // RS/DS/DC
    PC_15,      // RST
    LCD_ST7735::RGB
);

void btn_pressed() {
    printf("Button pressed\r\n");
    led1 = !led1;
    if (led1) {
        disp.fillRect(32, 32, 96, 96, Color565::Fuchsia);
    } else {
        disp.fillRect(32, 32, 96, 96, Color565::Aqua);
    }
}

//~void read_joystick(void) {
    //~Vector2D coord = stk.get_coord();
    //~if (fabs(coord.x - old_coord.x) > THRESHOLD || fabs(coord.y - old_coord.y) > THRESHOLD) {
        //~printf("(%d, %d)\n", (int)(coord.x * 512), (int)(coord.y * 512));
        //~old_coord.x = coord.x;
        //~old_coord.y = coord.y;
    //~}
//~}

int main() {
    // Request the shared queue
    EventQueue *queue = mbed_event_queue();

    led1 = 0;

    disp.setBackgroundColor(Color565::Yellow);
    disp.setBackgroundColor(Color565::Red);
    disp.initDisplay();
    disp.clearScreen(Color565::Black);
    disp.fillRect(32, 32, 96, 96, Color565::Aqua);

    btn.mode(PullUp);
    btn.setSamplesTillAssert(3);
    btn.attach_asserted(queue->event(btn_pressed));

    //~stk.init();

    btn.setSampleFrequency();
    //~queue->call_every(20ms, read_joystick);
    queue->dispatch_forever();
}
