
# LED-Beleuchtung

## Contents
- [Installation Instructions](#installation-instructions)
    - [Windows](#windows)
    - [NixOS](##nixos-or-other-linux-distros-with-the-nix-package-manager-installed)
    - [Ubuntu/Debian](#ubuntudebian)
    - [Arch Linux](#arch)
    - [OpenSUSE](#opensuse)
- [Building and Flashing](#building-and-flashing)

## Installation Instructions

### Windows

1. Install the Windows Subsystem for Linux and your Linux distribution of choice.
2. Follow the installation instructions for that distribution.

### NixOS (or other Linux distros with the Nix package manager installed)

If git isn't installed, install it via:
```
nix-env -i git
```
In the directory where you want to clone this repository, execute:
```
git clone -b chroma https://github.com/FIUS/LED-Beleuchtung
```
Now, whenever you want to work on the project, `cd` into LED-Beleuchtung and load the development environment via:
```
nix-shell
```
(This installs the dependencies described in default.nix and opens a shell where they're available, without polluting your user space. You can close that shell simply via `exit`.)

### Ubuntu/Debian

In the directory where you want to clone this repository, execute:
```
sudo apt-get install git make python-serial && \
git clone -b chroma https://github.com/FIUS/LED-Beleuchtung
```

### Arch Linux

In the directory where you want to clone this repository, execute:
```
sudo pacman -S git make python-pyserial && \
git clone -b chroma https://github.com/FIUS/LED-Beleuchtung
```

### OpenSUSE

In the directory where you want to clone this repository, execute:
```
sudo zypper install git-core make python-pyserial && \
git clone -b chroma https://github.com/FIUS/LED-Beleuchtung
```

## Building and Flashing

### Quick Start
1. Connect the ESP32 board via USB.
2. Set the USB device path as `UPLOAD_PORT` in your *makeConfig.mk* file.
3. Execute `make all bootloader flash listen`.

### Reference
- `make` or `make all` builds the project.
- `make bootloader` flashes the bootloader to the ESP32 board. Needs read/write access to the USB device path.
- `make flash` flashes the most recent build to the ESP32 board. Needs read/write access to the USB device path.
- `make listen` prints the output received from the ESP32 board. Needs read access to the USB device path. This can be cancelled with `ctrl`+`c`. Don't worry when *make* says it failed &ndash; that's normal.
- `make clean` removes the build directory, causing a full rebuild when executing `make` or `make all` the next time.

In case the user doesn't have sufficient access rights, the alterations `sudo make bootloader`, `sudo make flash` and `sudo make listen` are needed.

Different options can be combined. Example: `sudo make all flash listen` rebuilds, flashes the build prints the output.

When making alterations to the build, the bootloader doesn't need to be reflashed.
