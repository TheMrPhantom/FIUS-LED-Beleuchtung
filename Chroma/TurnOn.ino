int onState = 0;

void TurnOnStateUpdate() {
  if (onState < 255) {
    CRGB white = rainbowColor(0, 0, onState);
    CRGB color = rainbowColor(40, 255, onState*0.8);
    for (int i = 0; i < NUM_LEDS; i++) {
      if (i > 22 && i < 115 || i > 192 && i < 300) {
        leds[i] = white;
      } else {
        leds[i] = color;
      }
    }
    onState+=3;
  }
}

void TurnOnStateInitialize() {
  onState = 0;
}
