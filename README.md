
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

### Quick Start
1. Connect the ESP32 board via USB.
2. Set the USB device path as `UPLOAD_PORT` in your *makeConfig.mk* file.
3. Execute `make all bootloader flash listen`.

### Reference
- `make` or `make all` builds the project.
- `make bootloader` flashes the bootloader to the ESP32 board.
- `make flash` flashes the most recent build to the ESP32 board.
- `make listen` prints the output received from the ESP32 board.
- `make clean` removes the build directory, causing a full rebuild when executing `make` or `make all` the next time.

When making alterations to the build, the bootloader doesn't need to be reflashed. Rebuilding and reflashing can therefore be done via `make all flash` or `make all flash listen` (depending on whether you want to print the output).