int offset = 0;
void RainbowStateUpdate() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = rainbowColor((i + offset) % 256);
  }
  offset++;
  offset %= 256;
}

void RainbowStateInitialize() {

}
