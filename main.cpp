#include "mbed.h"
#include "mbed_events.h"

DigitalOut  led1(LED_BLUE);
InterruptIn button(BUTTON1, PullUp);
EventQueue  queue;

void on_button_released(void) {
    printf("Button released\r\n");
    led1 = !led1;
}

int main() {
    led1 = 1;
    button.rise(queue.event(on_button_released));
    queue.dispatch_forever();
}
