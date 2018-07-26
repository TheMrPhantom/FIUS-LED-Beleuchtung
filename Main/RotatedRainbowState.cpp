#include "RotatedRainbowState.hpp"

RotatedRainbowState::RotatedRainbowState(LedStrip &led_strip)
    : led_strip_{led_strip} {
    const int32_t third = led_strip_.PixelCount() / 3;
    led_strip_.Gradient(CRGB::Red, CRGB::Green, 0, third);
    led_strip_.Gradient(CRGB::Green, CRGB::Blue, third, third);
    led_strip_.Gradient(CRGB::Blue, CRGB::Red, 2 * third);
}

void RotatedRainbowState::Update() { led_strip_.Rotate(4); }
