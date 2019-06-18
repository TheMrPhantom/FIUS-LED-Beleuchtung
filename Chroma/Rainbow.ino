int offset = 0;
void RainbowStateUpdate() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = rainbowColor((i/2 + offset) % 256);
  }
  offset++;
  offset %= 256;
}

void RainbowStateInitialize() {
for (int i = 0; i < NUM_LEDS; i++) {
    CRGB temp = leds[i];
    leds[i] = CRGB(0,0,0);
  }
}
