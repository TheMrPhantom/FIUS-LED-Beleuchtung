#pragma once

#include "State.hpp"

class RotatedRainbowState : public State {
  public:
    RotatedRainbowState(LedStrip &led_strip) : led_strip_{led_strip} {
        const RgbColor red{255, 0, 0};
        const RgbColor green{0, 255, 0};
        const RgbColor blue{0, 0, 255};
        const int32_t s3 = led_strip_.PixelCount();
        const int32_t s2 = 2 * s3 / 3;
        const int32_t s1 = s3 / 3;
        led_strip_.Gradient(0, s1 - 1, red, green);
        led_strip_.Gradient(s1, s2 - 1, green, blue);
        led_strip_.Gradient(s2, s3 - 1, blue, red);
    }

    void Update() override { led_strip_.Rotate(4); }

  private:
    LedStrip &led_strip_;
};
