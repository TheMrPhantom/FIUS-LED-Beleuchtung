#pragma once

#include "State.hpp"

class WhiteState : public State {
  public:
    WhiteState(LedStrip &led_strip) {
        led_strip.SetColors(0, led_strip.PixelCount() - 1,
                            RgbColor{10, 10, 10});
    }
    void Update() override {}
};
