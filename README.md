
# LED-Beleuchtung

## Contents
- [Installation Instructions](#installation-instructions)
-- [Windows](#windows)
-- [Ubuntu/Debian](#ubuntudebian)
- [Building and Flashing](#building-and-flashing)

## Installation Instructions

### Windows

1. Install the Windows Subsystem for Linux and your Linux distribution of choice.
2. Follow the installation instructions for that distribution.

### Ubuntu/Debian

In the directory where you want to clone this repository, execute:
```
sudo apt-get install git make python-serial && \
git clone -b chroma https://github.com/FIUS/LED-Beleuchtung && \
cd LED-Beleuchtung && \
./configure
```

## Building and Flashing

`make` builds the project.

Before flashing the build to the ESP32 board, follow these instructions:
1. Connect the ESP32 board via USB.
2. Set the USB device path as `UPLOAD_PORT` in your *makeConfig.mk* file.

`make flash` flashes the most recent build to the ESP32 board. If you get `Permission denied: '<UPLOAD_PORT>'`, execute `sudo chmod 666 <UPLOAD_PORT>` and try `make flash` again.

`make && make flash` does build and flash.