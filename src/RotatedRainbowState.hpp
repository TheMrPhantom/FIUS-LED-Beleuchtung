#pragma once

#include "State.hpp"

class LedStrip;

class RotatedRainbowState : public State {
  public:
    RotatedRainbowState(LedStrip &led_strip);
    void Update() override;

  private:
    LedStrip &led_strip_;
};
