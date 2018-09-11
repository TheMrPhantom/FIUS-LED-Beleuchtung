#include "WhiteState.hpp"
#include "LedStrip.hpp"

WhiteState::WhiteState(LedStrip &led_strip) {
    led_strip.SetColors(CRGB{200, 200, 200});
}

void WhiteState::Update() {}
