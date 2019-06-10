int fading = 0;
bool onStateChanged = false;
bool slowFade = true;
int offState = 0;

const int fadingSize = 20;

void TurnOffStateUpdate() {
  if (onStateChanged) {
    if (fading > 0) {

      for (int i = 0; i < (NUM_LEDS / fadingSize) - fadingSize; i++) {
        if (i * fadingSize + offState > NUM_LEDS - 1) {
          fading = 0;
          break;
        }
        if (slowFade) {
          int r = leds[i * fadingSize + offState].r / 2;
          int g = leds[i * fadingSize + offState].g / 2;
          int b = leds[i * fadingSize + offState].b / 2;
          leds[i * fadingSize + offState] = CRGB(r, g, b);

        } else {
          leds[i * fadingSize + offState] = CRGB(0, 0, 0);

        }
      }
      leds[offState] = CRGB(0, 0, 0);
      offState++;
      slowFade = !slowFade;

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
  fading = NUM_LEDS;
  onStateChanged = true;
  offState = 0;
}
