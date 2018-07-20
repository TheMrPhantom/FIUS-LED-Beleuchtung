#pragma once

class Meteor {
  public:
    static const int MAX_SPEED = 30;
    Meteor(LedStrip *strip);
    void Update();

  private:
    void Respawn(int meteorNumber);
    void PrintVoid();
    void Initialize();

    static const int meteorCount = 7; // Number of meteors
    RgbColor backgroundColor;
    int meteorSpeed[meteorCount]; // The speed of the meteors
    int meteorPos[meteorCount]; // The position [0, Num Pixels * 100]
    int meteorDir[meteorCount]; // -1 = backward 1= forward
    int meteorSwitch[meteorCount]; // Attribute to stop meteors from switching to often
    RgbColor meteorColor[meteorCount]; // The colors of the meteors
    int meteorLife[meteorCount]; // The life points of the meteor
    RgbColor colorByID(byte colorNumber);
    long timeDif;
    LedStrip *strip;
    int beacon;
};
