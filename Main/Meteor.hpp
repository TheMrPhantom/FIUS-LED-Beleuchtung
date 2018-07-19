#pragma once

class Meteor {
  public:
    Meteor(LedStrip *strip);
    void Update();

  private:
    void Respawn(int meteorNumber);
    void Initialize();

    static const int meteorCount = 7; // Number of meteors
    int meteorSpeed[meteorCount]; // The speed of the meteors
    int meteorPos[meteorCount]; // The position [0, Num Pixels * 100]
    int meteorDir[meteorCount]; // -1 = backward 1= forward
    int meteorSwitch[meteorCount]; // Attribute to stop meteors from switching to often
    RgbColor meteorColor[meteorCount]; // The colors of the meteors
    int meteorLife[meteorCount]; // The life points of the meteor
    long timeDif;
    LedStrip *strip;

};
