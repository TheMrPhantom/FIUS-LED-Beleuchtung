void ColorStateUpdate() {
  if (onColorChanged) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = color;
    }
    onColorChanged = false;
  }
}

void ColorStateInitialize() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = color;
  }
}
