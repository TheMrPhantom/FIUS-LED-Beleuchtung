int bubbleState[] = {0, 20, 40, 60, 80, 100};
int bubbleStateCount = 6;

void BubbleSortInitialize() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = rainbowColor(random(0, 256), random(0, 256), random(0, 256));
  }
 
}

void BubbleSortUpdate() {
  for (int i = 0; i < bubbleStateCount; i++) {
    CRGB first = leds[bubbleState[i]];
    CRGB second = leds[bubbleState[i] + 1];
    int sumfirst = first.r + first.g + first.b;
    int sumsecond = second.r + second.g + second.b;
    if (sumfirst > sumsecond) {
      leds[bubbleState[i]] = second;
      leds[bubbleState[i] + 1] = first;
    }
  }
  
  for (int i = 0; i < bubbleStateCount; i++) {
    bubbleState[i]++;
    bubbleState[i] %= NUM_LEDS - 1;
  }
}
