byte activeColor = 0;
int activeLED = 0;
int changeLength = 30;

void SmoothLightStateInitialize() {}

void SmoothLightStateUpdate() {
  for (int i = 0; i < NUM_LEDS; i++) {
    if (i < activeLED) {

      leds[i] = rainbowColor(activeColor + changeLength);
    } else if (i >= activeLED && i < activeLED + 30) {
      int numberInTransition = i - activeLED;
      numberInTransition = changeLength - numberInTransition;
      leds[i] = rainbowColor(activeColor + numberInTransition);
    }
  }

  activeLED++;
  if (activeLED >= NUM_LEDS) {
    activeLED = 0;
    activeColor += changeLength;
  }
}
