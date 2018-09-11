#include "WhiteState.hpp"
#include "LedStrip.hpp"

WhiteState::WhiteState(LedStrip &led_strip) {
    led_strip.SetColors(CRGB{150, 150, 150});
}

void WhiteState::Update() {}
