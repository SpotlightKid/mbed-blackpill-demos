Setup mbed CLI on Arch Linux
============================

## Installation

```console
pikaur -S \
    arm-none-eabi-gcc \
    arm-none-eabi-gdb \
    base-devel \
    git \
    mercurial \
    python-cbor \
    python-cmsis-pack-manager \
    python-mbed-cli \
    python-mbed-cloud-sdk \
    python-mbed-flasher \
    python-mbed-greentea \
    python-mbed-host-tests \
    python-mbed-icetea \
    python-mbed-ls \
    python-mbed-manifest-tool \
    python-mbed-os-tools \
    python-serial \
    stm32flash \
    stlink
```

**Notes**: `python-intelhex` needs to be >= 2.3.0 for Python 3.9 conmpatibility.


## Configuration


### System

GCC for ARM is installed into `/usr/bin`, so setting `GCC_ARM_PATH` with `mbed config` is not
neccessary.


### Bash completion

Bash completion for `mbed-cli` is set up by the file `/etc/bash_completion.d/mbed-cli.sh`, which
is installed by the package `python-mbed-cli`. Be sure to open a new shell for the file to get
loaded.


### mbed options

You can view the mbed configuration with:

    mbed config --list

If you want, you can turn on cashing of mbed repository clones with:

    mbed config --global cache on


## Links

* https://os.mbed.com/docs/mbed-os/v6.6/build-tools/install-and-set-up.html