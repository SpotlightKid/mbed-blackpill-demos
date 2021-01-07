Using the Black Pill STM32F411CEU6 with mbed
============================================

This guide explains how to create a simple example "blinky" program for an [Black Pill] development
board with an STM32F411CEU6 Cortex-M4 microcontroller using the mbed os 6 embedded firmware
framework. We'll be working with the command line on Arch Linux and use the ARM version of the GCC
C/C++ compiler.

The Black Pill is compatible in many ways with the `NUCLEO-F411RE` mbed platform, but of course pin
labels and assignments differ.


## Toolbox setup

Please refer to the file [notes-mbed-arch.md](./notes-mbed-arch.md) for instructions on how
install & set up the mbed command development tools on Arch Linux.


## Writing the firmware

To write the firmware binary produced by mbed compiler to the board...


### With an USB serial adapter

Connections (for "USB to TTL" adapter board):

    USB serial adapter      STM32F411 (Black Pill)
    GND                     GND
    3V3                     3V3
    RX(D)                   (P)A9
    TX(D)                   (P)A10
    RST                     <not connected>

Press and hold the BOOT0 button, then press the NRST button and let go of it first, then the BOOT0
button.

Then run `stm32flash`:

    stm32flash -v -g 0x0 -w ./BUILD/NUCLEO_F411RE/GCC_ARM/*.bin -b 115200 /dev/ttyUSB0

Successful execution should output something like this:

    stm32flash 0.5

    http://stm32flash.sourceforge.net/

    Using Parser : Raw BINARY
    Interface serial_posix: 115200 8E1
    Version      : 0x31
    Option 1     : 0x00
    Option 2     : 0x00
    Device ID    : 0x0431 (STM32F411xx)
    - RAM        : 128KiB  (12288b reserved by bootloader)
    - Flash      : 512KiB (size first sector: 1x16384)
    - Option RAM : 16b
    - System RAM : 30KiB
    Write to memory
    Erasing memory
    Wrote and verified address 0x08007628 (100.00%) Done.

    Starting execution at address 0x08000000... done.


### With an ST-Link V2 (or clone)

This is much faster than the serial method.

Connections:

    ST-Link V2  STM32F411 (Black Pill) SWD connector
    GND         GND
    SWCLK       SWCLK (PA14)
    SWDIO       SWDIO (PA13)
    3V3         3V3

Flash firmware:

    st-flash --reset write ./BUILD/NUCLEO_F411RE/GCC_ARM/*.bin 0x8000000


### With a USB-C to USB-A cable

This doesn't need any extra hardware besides the USB cable that probably already came with your
Black Pill board. Firmware transfer speed is a bit lower than via ST-Link but much faster than via
serial interface and the overall procedure is probably the fastest, because hooking up the USB
cable just needs as few seconds.

To get the board in DFU mode, connect the USB cable to your computer and the board's USB-C port and
then use the same procedure as activating the serial bootloader: Press and hold the BOOT0 button,
then press NRST button and let go of it first, then the BOOT0 button.

You can check whether this was successfull by running the `lsusb` command. The output should
constain a line ending with:

    ID 0483:df11 STMicroelectronics STM Device in DFU Mode

We need the listed ID `0483:df11` for our DFU firmware upload command, but since this is always the
same for boards with this microprocessor, we can hardcode it into our command options:

    dfu-util -a 0 -s 0x08000000:leave -d 0483:df11 -D ./BUILD/NUCLEO_F411RE/GCC_ARM/*.bin

Once the firmware is uploaded, `dfu-util` tells the board to leave DFU mode and so the board will
reset itself and our firmware program will start.


## Create a new Blinky project (bare-metal profile)

```
mbed new --verbose --program --scm git --no-requirements Blackpill_baremetal_blinky
```

Configure the target board and compilation toolchain:

    cd Blackpill_baremetal_blinky
    mbed target NUCLEO_F411RE
    mbed toolchain GCC_ARM

Edit `mbed_app.json` and replace its content with the following:

```json
{
    "requires": ["bare-metal"],
    "target_overrides": {
        "*": {
            "target.c_lib": "small"
        }
    }
}
```

Create `main.cpp` with the following content:

```cpp
#include "mbed.h"

// Blinking rate in microseconds
#define BLINKING_RATE_US 500000

// Initialise the digital pin PC_13 (blue onboard LED) as an output
DigitalOut led(PC_13);

int main()
{
    while (true) {
        led = !led;
        wait_us(BLINKING_RATE_US);
    }
}
```

Compile everything:

    mbed compile

The resulting firmware binary will be the file
`./BUILD/NUCLEO_F411RE/GCC_ARM/Blackpill_baremetal_blinky.bin`

Write the firmware to the board with one of the methods described above.

The blue user LED on the Black Pill board should now blink at a frequency of 1 Hz.


## Create a new Blinky project (RTOS profile)

```
mbed new --verbose --program --scm git --no-requirements Blackpill_rtos_blinky
```

Configure the target board and compilation toolchain:

    cd Blackpill_rtos_blinky
    mbed target NUCLEO_F411RE
    mbed toolchain GCC_ARM

Delete the file `mbed_app.json`.

Create `main.cpp` with the following content:

```cpp
#include "mbed.h"

// Initialise the digital pin PC_13 (blue onboard LED) as an output
DigitalOut led(PC_13);

int main()
{
    while (true) {
        // Toggle LED
        led = !led;
        // Sleep thread for 200 milliseconds
        ThisThread::sleep_for(200);
    }
}
```

Compile everything:

    mbed compile

The resulting firmware binary will be the file
`./BUILD/NUCLEO_F411RE/GCC_ARM/Blackpill_rtos_blinky.bin`

Write the firmware to the board with one of the methods described above.

The blue user LED on the Black Pill board should now blink at a frequency of 2.5 Hz.


## Commiting a project to a new Git repository

Create the file `.gitignore` with the following content:

```
__pycache__/
BUILD/
```

Stage all files (minus ignored ones) for the first commit:

    git add -A

And commit them:

    git commit -m "Initial commit for project 'Blackpill_rtos_blinky'"


## Pins

Pins PA_11 and PA_12 are used by USB and cannot be used as GPIO pins if the
USBDevice library is used.


## Links

* <https://stm32-base.org/boards/STM32F411CEU6-WeAct-Black-Pill-V2.0>
* <https://www.st.com/en/microcontrollers-microprocessors/stm32f411ce.html>
* <https://os.mbed.com/users/hudakz/code/Blackpill_Hello/>
* <https://developer.mbed.org/platforms/ST-Nucleo-F411RE/>
* <https://sourceforge.net/p/stm32flash/wiki/Home/>
* <https://github.com/stlink-org/stlink>
* <http://dfu-util.sourceforge.net/>

* <https://os.mbed.com/docs/mbed-os/v6.6/porting/porting-custom-boards.html>
* <https://os.mbed.com/docs/mbed-os/v6.6/program-setup/adding-and-configuring-targets.html>
* <https://os.mbed.com/teams/ST/wiki/Automatic-clock-configuration>
