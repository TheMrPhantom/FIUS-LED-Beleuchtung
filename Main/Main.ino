#include <Adafruit_NeoPixel.h>
#define TOUCH_PIN T2 //connected to 2
#define PIN 4
int touch_value = 100;
long t = 0;
boolean statusChanged;
int state = 0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(1000, PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  statusChanged = true;
  state = 3;
Serial.begin(9600);
  strip.begin();
  refreshLED(); // Initialize all pixels to 'off'

}

void loop()
{
  if (checkStatus() || state == 3 || statusChanged) {
    statusChanged = false;
    switch (state) {
      case 0:
        for (int i = 0; i < 400; i++) {
          strip.setPixelColor(i, 255, 255, 0);
        }
        refreshLED();
        break;
      case 1:
        for (int i = 0; i < 400; i++) {
          strip.setPixelColor(i, 255, 255, 0);
        }
        for (int i = 192; i < 300; i++) {
          strip.setPixelColor(i, 255, 255, 255);
        }
        refreshLED();
        break;
      case 2:
        for (int i = 0; i < 400; i++) {
          strip.setPixelColor(i, 255, 255, 0);
        }
        for (int i = 20; i < 120; i++) {
          strip.setPixelColor(i, 255, 255, 255);
        }
        refreshLED();
        break;
      case 3:
        rainbowCycle(20);
        break;
    }
  }
}

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  int counter = 0;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    counter++;
    if (counter > 100) {
      if (checkStatus()) {
        break;
      }
      counter = 0;
    }
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    refreshLED();
    delay(wait);

  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

boolean checkStatus() {


  if (millis() - t > 1000) {
    touch_value = touchRead(TOUCH_PIN);
    if (touch_value < 15 ) {
      t = millis();
      state++;

      state %= 4;


      statusChanged = true;
      return true;
    }
  }
  return false;

}


void refreshLED() {
  delay(10);
  portDISABLE_INTERRUPTS();
  delay(10);
  strip.show();
  portENABLE_INTERRUPTS();
}


