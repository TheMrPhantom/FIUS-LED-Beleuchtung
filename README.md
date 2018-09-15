
# LED-Beleuchtung

# Contents
- [LED-Beleuchtung](#led-beleuchtung)
- [Contents](#contents)
- [Installation Instructions](#installation-instructions)
    - [Windows](#windows)
    - [NixOS (or other Linux distros with the Nix package manager installed)](#nixos-or-other-linux-distros-with-the-nix-package-manager-installed)
    - [Ubuntu/Debian](#ubuntu-debian)
    - [Arch Linux](#arch-linux)
    - [OpenSUSE](#opensuse)
- [Building and Flashing](#building-and-flashing)
    - [Quick Start](#quick-start)
    - [Reference](#reference)
- [clang-format](#clang-format)
    - [Visual Studio Code](#visual-studio-code)

# Installation Instructions

## Windows

1. Install the Windows Subsystem for Linux and your Linux distribution of choice.
2. Follow the installation instructions for that distribution.

## NixOS (or other Linux distros with the Nix package manager installed)

If git isn't installed, install it via:
```
nix-env -i git
```
In the directory where you want to clone this repository, execute:
```
git clone https://github.com/FIUS/LED-Beleuchtung
```
Now, whenever you want to work on the project, `cd` into LED-Beleuchtung and load the development environment via:
```
nix-shell
```
(This installs the dependencies described in default.nix and opens a shell where they're available, without polluting your user space. You can close that shell simply via `exit`. If you want to use a shell other than bash inside 
*nix-shell*, for example *fish*, try `nix-shell --command fish`.)

## Ubuntu/Debian

In the directory where you want to clone this repository, execute:
```
sudo apt-get install git make python-serial && \
git clone https://github.com/FIUS/LED-Beleuchtung
```

## Arch Linux

In the directory where you want to clone this repository, execute:
```
sudo pacman -S git make python-pyserial && \
git clone https://github.com/FIUS/LED-Beleuchtung
```

## OpenSUSE

In the directory where you want to clone this repository, execute:
```
sudo zypper install git-core make python-pyserial && \
git clone https://github.com/FIUS/LED-Beleuchtung
```

# Building and Flashing

## Quick Start
1. Connect the ESP32 board via USB.
2. Execute `make`
3. Set the USB device path as `UPLOAD_PORT` in your *makeConfig.mk* file.
4. Execute `sudo make bootloader flash listen`.

## Reference
- `make` or `make all` builds the project. When executed for the first time, it downloads the neccessary compilers and tools beforehand and creates a file called *makeConfig.mk*.
- `make bootloader` flashes the bootloader to the ESP32 board.
- `make flash` flashes the most recent build to the ESP32 board.
- `make listen` prints the output received from the ESP32 board. This can be cancelled with `ctrl`+`c`. Don't worry when *make* says it failed &ndash; that's normal.
- `make reboot` reboots the ESP32 board.
- `make erase` erases the flash memory of the ESP32 board (erases the bootloader and everything else).
- `make clean` removes the build directory, causing a full rebuild when executing `make` or `make all` the next time.

In *makeConfig.mk*, you have to specify the upload port which will be accesed by `make bootloader`, `make flash`, `make listen`, `make reboot` and `make erase`. In case your user doesn't have sufficient access rights, execute those commands with the `sudo ` prefix.

Different options can be combined. Example: `sudo make all flash listen` rebuilds, flashes the build and prints the output.

When making alterations to the build, the bootloader doesn't need to be reflashed.

# clang-format

This project's code is formatted using *clang-format*. The specific style is defined in `_clang-format`. Many editors have *clang-format* extensions.

## Visual Studio Code

The C++ extension `ms-vscode.cpptools` already comes with *clang-format* and formatting can by default be done via `ctrl`+`shift`+`I`. Alternatively, you can open your user settings (`ctrl`+`,`) and add:

```
    "editor.formatOnSave": true,
```
