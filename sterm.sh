#!/bin/bash

exec mbed sterm -p ${PORT:-/dev/ttyUSB0} -b ${BAUD:-115200}

