# LED-Beleuchtung
## Voraussetzungen
 1. Arduino IDE
 2. ESP32 Board Informationen installiert
 3. FastLED library installiert
## Verwendung
Es muss eine Datei ```WLAN_CONF.hpp``` erstellt werden. Diese muss wie folgt aussehen:
  ```c++
  char ssid[100] = "<SSID of wlan";        
  char pass[100] = "<Password of wlan>";
  ```
Die Länge des LED Streifens kann mit der Variable
```c++
const int NUM_LEDS = 800;
```
verändert werden.
