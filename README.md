Black Pill ST7735 LCD Example
=============================

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

If firmware doesn't start successfully, dis- and reconnect USB.


## Connections

    ST7735      STM32F411 (Black Pill) SWD connector
    VCC         3V3 (breadboard)
    GND         GND (breadboard)
    CLK         SPI_CLK (PA_5)
    SDA         SPI_MOSI (PA_7)
    RS          PC_14 (or any digital out pin)
    RST         PC_15 (or any digital out pin)
    CS          SPI_SS (PA_4)


## Notes

* SPI only works with either bare-metal profile ("small" C lib can be used) but
  not "event" feature OR mbed OS profile, but not with bare-metal and event
  queue.
* SPI frequency may be at 100 Mhz (no USB) or 96 Mhz (with USB) divided by
  2, 4, 8, 16 etc.
* SPI mode is 3 (polarity 1, phase 1).
* Before SPI instance is configured (`SPI::format`, `SPI::frequency`), the
  chip-select (CS) pin must be set HIGH!


## References

* <https://os.mbed.com/users/taylorza/code/LCD_ST7735/> (used as the base for code
  in the `LCD_7735` sub-directory, but heavily modified).
* <https://github.com/afiskon/stm32-st7735>
* <https://os.mbed.com/docs/mbed-os/v6.6/apis/spi.html>
* <https://os.mbed.com/docs/mbed-os/v6.6/mbed-os-api-doxy/classmbed_1_1_s_p_i.html>
* <https://os.mbed.com/teams/ST/wiki/SPI-output-clock-frequency>
