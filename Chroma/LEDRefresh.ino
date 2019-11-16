int lastRefresh = 0;
CRGB ledsTemp[100];

void refreshLED() {

  if (1000 / 60 < millis()) {
    /*Perform refresh*/
    lastRefresh = millis();
    if (shouldInitialize) {
      stateMethods[animationType * 2]();
      shouldInitialize = false;
    } else {
      stateMethods[animationType * 2 + 1]();
    }
    magicOverride();
    /*Display changes*/
    FastLED.show();
  }
}

void magicOverride() {
  Serial.println(printerStart);
  Serial.println(printerTemp);
  Serial.println(printerOn);
  Serial.println();
  if (printerStart > 0) {
    if (printerTemp < 0) {
      printerOn = !printerOn;
      Serial.println("Lala");
      if (printerOn) {
        for (int i = 852; i < 883; i++) {
          ledsTemp[i - 852] = leds[i];
          leds[i] = rainbowColor(0);
        }
      } else {
        for (int i = 852; i < 883; i++) {
          leds[i] = ledsTemp[i - 852];
        }
      }
      printerTemp = 23;

    } else {
      printerTemp--;
    }
    printerStart--;
  } else {
    if (printerOn) {
      printerOn=false;
      for (int i = 852; i < 883; i++) {
        leds[i] = ledsTemp[i - 852];
      }
    }
  }

}
