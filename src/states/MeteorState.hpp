#pragma once
#include "LedStrip.hpp"
#include "State.hpp"

class LedStrip;

class MeteorState : public State {
  public:
    static const int MAX_SPEED = 30;
    MeteorState(LedStrip &led_strip);
    void Update() override;

  private:
    void Initialize();

    void MoveMeteors();
    void CheckReadEnd();
    void CheckBeacon();
    void PaintHead();
    void FadeMeteors();
    void PrintBeacon();
    void Respawn(int meteorNumber);
    void PrintVoid();

    CRGB colorByID(byte colorNumber);

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
    LedStrip &strip;
    int beacon;
    int beaconSaveSpawn;
    byte beaconColor;
};
