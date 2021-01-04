#!/bin/bash

if [[ -f .mbed ]]; then
    source .mbed
fi

PROJECT="$(basename $(pwd))"

if [[ -n "${PROFILE}" ]]; then
    if [[ -e "${PROFILE}" ]]; then
        PROFILE_OPT="--profile=${PROFILE}"
    elif [[ -e "${PROFILE}.json" ]]; then
        PROFILE_OPT="--profile=${PROFILE}.json"
    elif [[ -e "mbed-os/tools/profiles/${PROFILE}.json" ]]; then
        PROFILE_OPT="--profile=${PROFILE}"
    fi
fi

exec mbed compile -m ${TARGET:-BLACKPILL_F411CE} -t ${TOOLCHAIN:-GCC_ARM} ${PROFILE_OPT} "$@"
