Black Pill mbed LED Switch Example
==================================

A simple example firmware for the "Black Pill" board based on an STM32F411CEU6 MCU using mbed OS.

This basic example switches the on-board LED on and off on each USER button press.

## Compilation

```
export PROFILE=develop
./build.sh
```

## Flashing

* Connect USB
* Press and hold BOOT0, then press NRST, release NRST, release BOOT0

```
export PROFILE=develop
./flash-dfu.sh
```

and confirm with 'y'.

If firmware doesn't start successfully, dis- and reconnect USB and try again.

See also these more [detailed notes](./notes-mbed-blackpill.md).


## Connections

None.

The example used the on-board blue LED (connectd to pin PC_13) and the USER switch (connected to
pin PA_0).


## Notes

* The LED pin is active HIGH.
* You may notice some bouncing of the USER button. De-bouncing button presses will be adressed in
  the next example.

## References

* <https://github.com/ARMmbed/mbed-os-example-blinky>
* <https://os.mbed.com/users/hudakz/code/Blackpill_Hello/>
* <https://os.mbed.com/docs/mbed-os/v6.6/apis/digitalout.html>
* <https://os.mbed.com/docs/mbed-os/v6.6/apis/interruptin.html>
* <https://os.mbed.com/docs/mbed-os/v6.6/apis/eventqueue.html>
