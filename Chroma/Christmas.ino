const CRGB red = CRGB(120, 0, 0);
const CRGB green = CRGB(40, 120, 0);
const CRGB spark = CRGB(255, 255, 0);
const int sparkSize = 5;
const int lightSize = 10;
int cstate = 0;

void ChristmasStateUpdate() {
  if (cstate < lightSize) {

    leds[0] = red;
  } else if (cstate == lightSize * 2 - 1 || cstate == lightSize * 2 - 2 || cstate == lightSize * 2 - 3 || cstate == lightSize || cstate == lightSize + 1 || cstate == lightSize - 1) {

    leds[0] = spark;
  } else {

    leds[0] = green;
  }

  for (int i = NUM_LEDS; i >= 0; i--) {
    leds[i + 1] = leds[i];

  }

  cstate++;
  cstate %= lightSize * 2;
}

void ChristmasStateInitialize() {
  for (int i = 0; i < NUM_LEDS; i++) {
    if (i % (lightSize * 2) < lightSize) {
      leds[i] = red;

    } else {
      leds[i] = green;

    }
  }
  
}
