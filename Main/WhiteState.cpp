#include "WhiteState.hpp"

WhiteState::WhiteState(LedStrip &led_strip) {
    led_strip.SetColors(CRGB{10, 10, 10});
}

void WhiteState::Update() {}
