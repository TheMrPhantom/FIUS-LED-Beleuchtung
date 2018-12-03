#include "Christmas.hpp"

#include "LedStrip.hpp"
#include "util/Util.hpp"

Christmas::Christmas(LedStrip &led_strip) : strip(led_strip) { Initialize(); }

void Christmas::Update() {
    for (int i = strip.PixelCount(); i > 0; i--) {
        strip.SetColor(strip.GetColor(i), i + 1);
    }

    if (cstate < lightSize) {
        strip.SetColor(red, 0);
    } else {
        strip.SetColor(green, 0);
    }

    strip.Update();

    cstate++;
    cstate %= lightSize * 2;
}

void Christmas::Initialize() {
    for (int i = 0; i < strip.PixelCount(); i++) {
        if (i % (lightSize * 2) < lightSize) {
            strip.SetColor(red, i);
        } else {
            strip.SetColor(green, i);
        }
    }
    strip.Update();
}
