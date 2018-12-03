#pragma once
#include "LedStrip.hpp"
#include "State.hpp"

class LedStrip;

class Christmas : public State {
  public:
    Christmas(LedStrip &led_strip);
    void Update() override;

  private:
    void Initialize();

    const CRGB red = CRGB(120, 0, 0);
    const CRGB green = CRGB(40, 120, 0);
    const CRGB spark = CRGB(255, 255, 0);
    const int sparkSize=5;
    const int lightSize=10; 
    LedStrip &strip;
    int cstate=0;
};
