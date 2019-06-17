int trailPosition = 0;
int rainbowTrailcolor = 0;

void RainbowTrailInitialize() {
  for (int i = 0; i < NUM_LEDS; i++) {
    CRGB temp = leds[i];
    leds[i] = CRGB(0, 0, 0);
  }
}

void RainbowTrailUpdate() {

  leds[trailPosition] = CHSV(rainbowTrailcolor, 255, 255);


  for (int i = NUM_LEDS - 1; i >= 0; i--) {
    leds[i].fadeToBlackBy(4);

  }

  rainbowTrailcolor++;
  rainbowTrailcolor %= 256;
  trailPosition++;
  trailPosition %= NUM_LEDS;

}
