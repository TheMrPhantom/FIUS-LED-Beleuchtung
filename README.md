# How-To Animationen für LEDs
1. Clone das Projekt
2. Importiere das Projekt in die Arduino IDE
3. Erstelle einen neuen Tab für die Animation\
Der Name muss wie folgt aussehen: Method\<nummer\>.ini, wobei \<nummer\> durch die letze Nummer der Methoden + 1 ersetzt werden muss.
4. Im neuen Tab **muss** eine Methode erstellt werden, die wie folgt aussieht:
```c++
void method1(){
  
}
```
Wobei auch hier wieder die Nummer ersetzt werden muss.

## Mögliche Befehle der LED Libary ([Adafruit Neopixel](https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library-use))

### Farbe für eine spezielle LED setzten
```c++
/*
* n = Die ID der LED (mit 0 Index)
* red = Der Rot-Anteil der Farbe (0-255)
* green = Der Grün-Anteil der Farbe (0-255)
* blue = Der Blau-Anteil der Farbe (0-255)
*/
strip.setPixelColor(n, red, green, blue);
```

### Alternativ ist auch folgendes möglich
```c++
/*
* n = Die ID der LED (mit 0 Index)
* red = Der Rot-Anteil der Farbe (0-255)
* green = Der Grün-Anteil der Farbe (0-255)
* blue = Der Blau-Anteil der Farbe (0-255)
* color = Die Farbe, die gesetzt werden soll
*/
uint32_t color = strip.Color(red, green, blue);
strip.setPixelColor(n, color);
```
### Farbe anzeigen
Die Farbe wird nicht mit jeder Änderung aktualisiert, sondern muss mit folgendem Befehl an die LEDs geschickt werden
```c++
refreshLED();
```
### Farbwert einer LED bekommen
```c++
/*
* n = Der Index der LED
* color = Die Farbe der LED
*/
uint32_t color = strip.getPixelColor(n);
```
### Anzahl aller LEDs bekommen
```c++
/*
* n = Die Anzahl der LEDs
*/
uint16_t n = strip.numPixels();
```
## Nicht zu benutzende Methoden der Libary
### Allgemeinde Helligkeit verändern
```c++
/*
* n = Die Helligkeit des LED Streifens
*/
strip.setBrightness(n);
```
