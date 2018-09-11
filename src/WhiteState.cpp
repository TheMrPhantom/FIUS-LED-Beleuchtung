#include "WhiteState.hpp"
#include "LedStrip.hpp"

WhiteState::WhiteState(LedStrip &led_strip) {
    led_strip.SetColors(CRGB{100, 100, 100});
}

void WhiteState::Update() {}
