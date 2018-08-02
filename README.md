# LED-Beleuchtung

## Contents
- [Installation Instructions](#installation-instructions)

## Installation Instructions

1. In the directory where you want to clone this repository, execute:
```
git clone --single-branch -b chroma https://github.com/FIUS/LED-Beleuchtung
cd LED-Beleuchtung
git submodule update --init
cd arduino-esp32/tools
python2 get.py
cd ../..
cp makeConfig.template.mk makeConfig.mk
```
2. Configure *makeConfig.mk* using a text editor.

3. Build the project using `make`. If you also want to flash the build to the ESP32 board, use `make flash` instead.
