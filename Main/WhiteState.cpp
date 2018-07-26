#include "WhiteState.hpp"

WhiteState::WhiteState(LedStrip &led_strip) {
    led_strip.SetColors(0, led_strip.PixelCount() - 1, CRGB{10, 10, 10});
}

void WhiteState::Update() {}
