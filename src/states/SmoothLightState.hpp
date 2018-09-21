#pragma once
#include "LedStrip.hpp"
#include "State.hpp"

class LedStrip;

class SmoothLightState : public State {
  public:
    static const int MAX_SPEED = 30;
    SmoothLightState(LedStrip &led_strip);
    void Update() override;

  private:
    void Initialize();
    byte activeColor = 0;
    int activeLED = 0;
    int changeLength = 30;
    CRGB colorByID(byte colorNumber);
    LedStrip &strip;
};
