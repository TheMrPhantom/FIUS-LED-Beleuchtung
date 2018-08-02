LIBS = $(ESP_LIBS)/WiFi libraries/FastLED
ESP_ROOT = arduino-esp32
SKETCH = Main/Main.ino
CHIP = esp32
BOARD = esp32
C_EXTRA = -std=gnu11
CPP_EXTRA = -std=gnu++17
include makeConfig.mk
include makeEspArduino.mk