#!/bin/bash

PROJECT="$(basename $(pwd))"
TOOLCHAIN="${TOOLCHAIN:-GCC_ARM}"
TARGET="${TARGET:-BLACKPILL_F411CE}"

if [[ -n "$PROFILE" ]]; then
    BINARY="BUILD/${TARGET}/${TOOLCHAIN}-${PROFILE^^}/${PROJECT}.bin"
else
    BINARY="BUILD/${TARGET}/${TOOLCHAIN}/${PROJECT}.bin"
fi

echo -n "Write firmware '${BINARY}' via ${PORT:-/dev/ttyUSB0} (Y/n)? "
read ret
if [[ -z "$ret" || "${ret,}" = "y" || "${ret,,}" = "yes" ]]; then
    exec stm32flash -v -g 0x0 ${BAUD:+-b $BAUD} -w ${BINARY} ${PORT:-/dev/ttyUSB0}
else
    echo "Cancelled."
fi
