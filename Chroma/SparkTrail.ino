int positionL;
int positionR;
CHSV sparkTrailColor;

void SparkTrailInitialize() {
  clearLED();
  resetSparkTrail();
}

void SparkTrailUpdate() {
  fadeAll();
  positionL--;
  positionR++;
  positionL = max(positionL, 0);
  positionR = min(positionR, NUM_LEDS - 1);

  if (positionL == 0 && positionR == NUM_LEDS - 1) {
    resetSparkTrail(); 
  }

  if (positionL != 0) {
    leds[positionL] = sparkTrailColor;
  }
  if (positionR != NUM_LEDS - 1) {
    leds[positionR] = sparkTrailColor;
  }
}

void resetSparkTrail() {
  int newPos = random(0, NUM_LEDS);
  positionL = newPos;
  positionR = newPos;
  sparkTrailColor = CHSV(random(0, 256), 255, 255);
}
