#include "mbed.h"
#include "mbed_events.h"
#include "Joystick.h"
#include "PinDetect.h"

#define THRESHOLD 0.008

DigitalOut  led1(LED_BLUE);
PinDetect   btn(USER_BUTTON);
Joystick    stk(A1, A4);

Vector2D old_coord = {0.0f, 0.0f};

void btn_pressed(void) {
    printf("Button pressed\r\n");
    led1 = !led1;
}

void read_joystick(void) {
    Vector2D coord = stk.get_coord();
    if (fabs(coord.x - old_coord.x) > THRESHOLD || fabs(coord.y - old_coord.y) > THRESHOLD) {
        printf("(%d, %d)\n", (int)(coord.x * 512), (int)(coord.y * 512));
        old_coord.x = coord.x;
        old_coord.y = coord.y;
    }
}

int main() {
    // Request the shared queue
    EventQueue *queue = mbed_event_queue();

    btn.mode(PullUp);
    btn.attach_asserted(queue->event(btn_pressed));
    btn.setSamplesTillAssert(3);
    btn.setSampleFrequency();

    stk.init();
    queue->call_every(20ms, read_joystick);
    queue->dispatch_forever();
}
