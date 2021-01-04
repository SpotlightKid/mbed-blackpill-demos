#!/bin/bash

PROJECT="$(basename $(pwd))"
TOOLCHAIN="${TOOLCHAIN:-GCC_ARM}"
TARGET="${TARGET:-BLACKPILL_F411CE}"
DFU_DEVICE="${DEVICE:-0483:df11}"

if [[ -n "$PROFILE" ]]; then
    BINARY="BUILD/${TARGET}/${TOOLCHAIN}-${PROFILE^^}/${PROJECT}.bin"
else
    BINARY="BUILD/${TARGET}/${TOOLCHAIN}/${PROJECT}.bin"
fi

echo -n "Write firmware '${BINARY}' via DFU to ${DFU_DEVICE} (Y/n)? "
read ret
if [[ -z "$ret" || "${ret,}" = "y" || "${ret,,}" = "yes" ]]; then
    exec dfu-util -a 0 -s 0x08000000:leave -d "${DFU_DEVICE}" -D "${BINARY}"
else
    echo "Cancelled."
fi
