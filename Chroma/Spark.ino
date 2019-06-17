int temp = 0;

void SparkUpdate() {
  if (random(0, 100) > 50) {
    int led = random(0, NUM_LEDS);
    int h = random(0, 256);
    int s = random(0, 256);
    int v = random(0, 256);
    leds[led] = CHSV(h, s, v);
  }

  for (int i = 0; i < NUM_LEDS; i++) {
    CRGB temp = leds[i];
    leds[i] = CRGB(temp.r * 0.99, temp.g * 0.99, temp.b * 0.99);
  }
}

void SparkInitialize() {
  for (int i = 0; i < NUM_LEDS; i++) {
    CRGB temp = leds[i];
    leds[i] = CRGB(0, 0, 0);
  }
}
