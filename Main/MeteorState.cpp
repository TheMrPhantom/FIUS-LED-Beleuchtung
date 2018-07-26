#include "MeteorState.hpp"
#include "Util.hpp"

MeteorState::MeteorState(LedStrip &led_strip) : strip(led_strip) {
    Initialize();
}

void MeteorState::Update() {
    if (millis() - timeDif < 17) {
        return;
    } else {
        timeDif = millis();
    }

    MoveMeteors();
    CheckReadEnd();
    CheckBeacon();
    PaintHead();
    FadeMeteors();
    PrintVoid();
    PrintBeacon();
}

void MeteorState::MoveMeteors() {
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

                        Respawn(x);
                        Respawn(i);
                    } else {
                        meteorLife[i] += meteorLife[x];
                        if (meteorLife[i] > MeteorState::MAX_SPEED) {
                            meteorLife[i] = MeteorState::MAX_SPEED;
                        }
                        meteorSpeed[i] = meteorLife[i] * 10;
                        meteorLife[x] = 0;
                        Respawn(x);
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

void MeteorState::CheckReadEnd() {
    for (int x = 0; x < meteorCount; x++) {
        if (meteorPos[x] / 100 > strip.PixelCount() - 2 ||
            meteorPos[x] / 100 < 2) {
            if (meteorDir[x] == -1) {
                meteorDir[x] = 1;
            } else {
                meteorDir[x] = -1;
            }
        }
    }
}

void MeteorState::CheckBeacon() {
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

void MeteorState::PaintHead() {
    for (int i = 0; i < meteorCount; i++) {
        int pos = meteorPos[i] / 100;
        CRGB color = meteorColor[i];
        strip.SetColor(color, pos);
        if (meteorDir[i] == -1) {
            if (pos + 1 < strip.PixelCount())
                strip.SetColor(color, pos + 1);
            if (pos + 2 < strip.PixelCount() && meteorSpeed[i] > 20)
                strip.SetColor(color, pos + 2);
        } else {
            if (pos - 1 > 0)
                strip.SetColor(color, pos - 1);
            if (pos - 2 > 0 && meteorSpeed[i] > 20)
                strip.SetColor(color, pos - 2);
        }
    }
}

void MeteorState::FadeMeteors() {
    for (int i = 0; i < strip.PixelCount(); i++) {
        CRGB c = strip.GetColor(i);
        if (c.r != 10 && c.g != 10 && c.b != 10) {
            c.subtractFromRGB(5);
            strip.SetColor(c, i);
        } else {
            strip.SetColor(backgroundColor, i);
        }
    }
}

void MeteorState::Respawn(int meteorNumber) {
    if (meteorLife[meteorNumber] == 0) {
        meteorLife[meteorNumber] = 10;

        if (RandomInt(0, 2) == 0) {
            meteorPos[meteorNumber] = strip.PixelCount() * 100 - 500;
        } else {
            meteorPos[meteorNumber] = 500;
        }

        if (RandomInt(0, 2) == 0) {
            meteorColor[meteorNumber] =
                CHSV(RandomInt(0, 255), 1, RandomInt(25, 130));
        } else {
            meteorColor[meteorNumber] = CRGB(
                RandomInt(30, 255), RandomInt(30, 255), RandomInt(30, 255));
        }

        meteorSpeed[meteorNumber] = meteorLife[meteorNumber] * 10;
    }
}

void MeteorState::Initialize() {
    timeDif = 0;
    beaconTime = 0;
    beacon = -1;
    backgroundColor = CRGB(10, 10, 10);
    /* Initializes the meteor attributes */
    for (int i = 0; i < meteorCount; i++) {
        meteorPos[i] = RandomInt(0, strip.PixelCount() * 100);
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
            meteorDir[i] = RandomInt(-1, 2);
        } while (meteorDir[i] == 0);
    }

    for (int i = 0; i < meteorCount; i++) {
        if (RandomInt(0, 2) == 0) {
            meteorColor[i] = CHSV(RandomInt(0, 255), 1, RandomInt(26, 130));
        } else {
            meteorColor[i] = CRGB(RandomInt(30, 255), RandomInt(30, 255),
                                  RandomInt(30, 255));
        }
    }
}

void MeteorState::PrintVoid() {
    for (int i = 0; i < strip.PixelCount(); i++) {
        CRGB c = strip.GetColor(i);
        if (c.r == 0 && c.g == 0 && c.b == 0) {
            strip.SetColor(backgroundColor, i);
        }
    }
}

void MeteorState::PrintBeacon() {
    if (millis() - beaconTime > beaconSpawnTime) {
        beaconTime = millis();
        beacon = RandomInt(10, strip.PixelCount() - 10);
        beaconSaveSpawn = 600;
    }

    if (beacon != -1) {
        if (beaconSaveSpawn < 0) {
            for (int i = beacon - 5; i < beacon + 5; i++) {
                strip.SetColor(colorByID(beaconColor), i);
            }
        } else {
            for (int i = beacon - 5; i < beacon + 5; i++) {
                strip.SetColor(colorByID(beaconColor), i);
            }
            strip.SetColor(CRGB::White, beacon - 6);
            strip.SetColor(CRGB::White, beacon - 7);
            strip.SetColor(CRGB::White, beacon + 5);
            strip.SetColor(CRGB::White, beacon + 6);
        }
        beaconColor += 4;
    }
}

CRGB MeteorState::colorByID(byte colorNumber) {
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
