#include "mbed.h"
#include "mbed_events.h"
#include "PinDetect.h"

DigitalOut  led1(LED_BLUE);
PinDetect   btn(USER_BUTTON);

void btn_pressed(void) {
    printf("Button pressed\r\n");
    led1 = !led1;
}

int main() {
    // Request the shared queue
    EventQueue *queue = mbed_event_queue();

    btn.mode(PullUp);
    btn.attach_asserted(queue->event(btn_pressed));
    btn.setSamplesTillAssert(3);
    btn.setSampleFrequency();
    queue->dispatch_forever();
}
