static const int meteorCount = 7; // Number of meteors
static const int beaconSpawnTime = 60000;
CRGB backgroundColor;
int meteorSpeed[meteorCount];  // The speed of the meteors
int meteorPos[meteorCount];    // The position [0, Num Pixels * 100]
int meteorDir[meteorCount];    // -1 = backward 1= forward
int meteorSwitch[meteorCount]; // Attribute to stop meteors from switching
// to often
CRGB meteorColor[meteorCount]; // The colors of the meteors
int meteorLife[meteorCount];   // The life points of the meteor
long timeDif;
long beaconTime;
const int MAX_SPEED = 30;
int beacon;
int beaconSaveSpawn;
byte beaconColor;

void MeteorStateUpdate() {

  MeteorStateMoveMeteors();
  MeteorStateCheckReadEnd();
  MeteorStateCheckBeacon();
  MeteorStatePaintHead();
  MeteorStateFadeMeteors();
  MeteorStatePrintVoid();
  MeteorStatePrintBeacon();
}


void MeteorStateMoveMeteors() {
  for (int i = 0; i < meteorCount; i++) {
    if (meteorDir[i] == -1) {
      meteorPos[i] -= meteorSpeed[i];
    } else {
      meteorPos[i] += meteorSpeed[i];
    }
    for (int x = 0; x < meteorCount; x++) {
      if (((abs(meteorPos[x] / 100 - meteorPos[i] / 100) < 3) ||
           (abs(meteorPos[x] / 100 - meteorPos[i] / 100) < 5 &&
            (meteorLife[x] > 20 || meteorLife[i] > 20))) &&
          x != i) {
        if (meteorSwitch[i] == 0 || meteorSwitch[x] == 0) {
          if (meteorDir[i] != meteorDir[x]) {
            if (meteorDir[x] == -1) {
              meteorDir[x] = 1;
            } else {
              meteorDir[x] = -1;
            }
            if (meteorDir[i] == -1) {
              meteorDir[i] = 1;
            } else {
              meteorDir[i] = -1;
            }
            meteorSwitch[i] = 5;
            meteorSwitch[x] = 5;

            meteorLife[i] -= 1;
            meteorLife[x] -= 1;

            meteorSpeed[i] = meteorLife[i] * 10;
            meteorSpeed[x] = meteorLife[x] * 10;

            MeteorStateRespawn(x);
            MeteorStateRespawn(i);
          } else {
            meteorLife[i] += meteorLife[x];
            if (meteorLife[i] > MAX_SPEED) {
              meteorLife[i] = MAX_SPEED;
            }
            meteorSpeed[i] = meteorLife[i] * 10;
            meteorLife[x] = 0;
            MeteorStateRespawn(x);
          }
        }
      }
    }
    if (beacon != -1) {
      if (beaconSaveSpawn > 0) {
        if (abs(meteorPos[i] / 100 - beacon) < 6) {
          if (meteorDir[i] == -1) {
            meteorDir[i] = 1;
          } else {
            meteorDir[i] = -1;
          }
          meteorSwitch[i] = 5;
        }
      }
    }
  }

  for (int x = 0; x < meteorCount; x++) {
    if (meteorSwitch[x] > 0) {
      meteorSwitch[x]--;
    }
  }
}

void MeteorStateCheckReadEnd() {
  for (int x = 0; x < meteorCount; x++) {
    if (meteorPos[x] / 100 > NUM_LEDS - 2 ||
        meteorPos[x] / 100 < 2) {
      if (meteorDir[x] == -1) {
        meteorDir[x] = 1;
      } else {
        meteorDir[x] = -1;
      }
    }
  }
}

void MeteorStateCheckBeacon() {
  if (beacon != -1) {
    if (beaconSaveSpawn < 0) {
      for (int i = 0; i < meteorCount; i++) {
        if (abs(meteorPos[i] / 100 - beacon) < 5) {
          meteorLife[i] = MAX_SPEED;
          meteorSpeed[i] = meteorLife[i] * 10;
          beacon = -1;
          break;
        }
      }
    } else {
      beaconSaveSpawn--;
    }
  }
}

void MeteorStatePaintHead() {
  for (int i = 0; i < meteorCount; i++) {
    int pos = meteorPos[i] / 100;
    CRGB color = meteorColor[i];

    leds[pos] = color;

    if (meteorDir[i] == -1) {
      if (pos + 1 < NUM_LEDS)

        leds[pos + 1] = color;
      if (pos + 2 < NUM_LEDS && meteorSpeed[i] > 20)

        leds[pos + 2] = color;
    } else {
      if (pos - 1 > 0)

        leds[pos - 1] = color;
      if (pos - 2 > 0 && meteorSpeed[i] > 20)
        leds[pos - 2] = color;
    }
  }
}

void MeteorStateFadeMeteors() {
  for (int i = 0; i < NUM_LEDS; i++) {
    CRGB c = leds[i];
    if (c.r != 10 && c.g != 10 && c.b != 10) {
      c.subtractFromRGB(5);

      leds[i] = c;
    } else {

      leds[i] = backgroundColor;
    }
  }
}

void MeteorStateRespawn(int meteorNumber) {
  if (meteorLife[meteorNumber] == 0) {
    meteorLife[meteorNumber] = 10;

    if (random(0, 2) == 0) {
      meteorPos[meteorNumber] = NUM_LEDS * 100 - 500;
    } else {
      meteorPos[meteorNumber] = 500;
    }

    if (random(0, 2) == 0) {
      meteorColor[meteorNumber] =
        CHSV(random(0, 255), 1, random(25, 130));
    } else {
      meteorColor[meteorNumber] = CRGB(
                                    random(30, 255), random(30, 255), random(30, 255));
    }

    meteorSpeed[meteorNumber] = meteorLife[meteorNumber] * 10;
  }
}

void MeteorStateInitialize() {
  timeDif = 0;
  beaconTime = 0;
  beacon = -1;
  backgroundColor = CRGB(10, 10, 10);
  /* Initializes the meteor attributes */
  for (int i = 0; i < meteorCount; i++) {
    meteorPos[i] = random(0, NUM_LEDS * 100);
  }

  for (int i = 0; i < meteorCount; i++) {
    meteorLife[i] = 10;
  }

  for (int i = 0; i < meteorCount; i++) {
    meteorSpeed[i] = meteorLife[i] * 10;
  }

  for (int i = 0; i < meteorCount; i++) {
    meteorSwitch[i] = 0;
  }

  for (int i = 0; i < meteorCount; i++) {
    meteorDir[i] = 0;
    do {
      meteorDir[i] = random(-1, 2);
    } while (meteorDir[i] == 0);
  }

  for (int i = 0; i < meteorCount; i++) {
    if (random(0, 2) == 0) {
      meteorColor[i] = CHSV(random(0, 255), 1, random(26, 130));
    } else {
      meteorColor[i] = CRGB(random(30, 255), random(30, 255),
                            random(30, 255));
    }
  }
}

void MeteorStatePrintVoid() {
  for (int i = 0; i < NUM_LEDS; i++) {
    CRGB c = leds[i];
    if (c.r == 0 && c.g == 0 && c.b == 0) {

      leds[i] = backgroundColor;
    }
  }
}

void MeteorStatePrintBeacon() {
  if (millis() - beaconTime > beaconSpawnTime) {
    beaconTime = millis();
    beacon = random(10, NUM_LEDS - 10);
    beaconSaveSpawn = 600;
  }

  if (beacon != -1) {
    if (beaconSaveSpawn < 0) {
      for (int i = beacon - 5; i < beacon + 5; i++) {

        leds[i] = MeteorStateColorByID(beaconColor);
      }
    } else {
      for (int i = beacon - 5; i < beacon + 5; i++) {

        leds[i] = MeteorStateColorByID(beaconColor);
      }
      leds[beacon - 6] = CRGB::White;
      leds[beacon - 7] = CRGB::White;
      leds[beacon + 5] = CRGB::White;
      leds[beacon + 6] = CRGB::White;

    }
    beaconColor += 4;
  }
}

CRGB MeteorStateColorByID  (byte colorNumber) {
  colorNumber = 255 - colorNumber;
  if (colorNumber < 85) {
    return CRGB(255 - colorNumber * 3, 0, colorNumber * 3);
  }
  if (colorNumber < 170) {
    colorNumber -= 85;
    return CRGB(0, colorNumber * 3, 255 - colorNumber * 3);
  }
  colorNumber -= 170;
  return CRGB(colorNumber * 3, 255 - colorNumber * 3, 0);
}
