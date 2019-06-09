int fading = 0;
bool onStateChanged = false;

void TurnOffStateUpdate() {
  if (onStateChanged) {
    if (fading > 0) {
      for (int i = 0; i < NUM_LEDS; i++) {
        int r = leds[i].r / 2;
        int g = leds[i].g / 2;
        int b = leds[i].b / 2;
        leds[i] = CRGB(r, g, b);
      }
      fading--;
    } else {
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB(0, 0, 0);
      }
      onStateChanged = false;
    }

  }
}

void TurnOffStateInitialize() {
  fading = 20;
  onStateChanged = true;
}
