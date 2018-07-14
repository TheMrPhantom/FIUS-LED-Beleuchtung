#include "LedController.hpp"

static const auto kNumPixels = 1055;
static const auto kPin = 4;

namespace {

RgbColor Wheel(byte pos) {
    if (pos < 85) {
        byte b = static_cast<byte>(pos * 3);
        return RgbColor{~b, 0, b};
    }
    if (pos < 170) {
        pos -= 85;
        byte b = static_cast<byte>(pos * 3);
        return RgbColor{0, b, ~b};
    }
    pos -= 170;
    byte b = static_cast<byte>(pos * 3);
    return RgbColor{b, ~b, 0};
}

} // namespace

LedController::LedController() : strip_{kNumPixels, kPin} {
    strip_.Begin();
    RefreshLeds(); // Initialize all pixels to 'off'
}

LedController::~LedController() {}

void LedController::Update() {
    --ticksLeftActive_;
    bool changed = false;
    if (ticksLeftActive_ > 0) {
        static byte time = 0;
        time += 16;
        for (uint16_t pixel = 0; pixel < kNumPixels; ++pixel) {
            byte pos = static_cast<byte>(pixel * 256 / kNumPixels + time);
            changed |= ChangePixelColor(pixel, Wheel(pos));
        }
    } else {
        auto color = RgbColor{100, 100, 100};
        for (uint16_t pixel = 0; pixel < kNumPixels; ++pixel) {
            changed |= ChangePixelColor(pixel, color);
        }
    }
    if (changed) {
        RefreshLeds();
    }
}

void LedController::SetTicksLeftActive(int ticks) { ticksLeftActive_ = ticks; }

void LedController::RefreshLeds() { strip_.Show(); }

bool LedController::ChangePixelColor(uint16_t pixel, RgbColor color) {
    if (strip_.GetPixelColor(pixel) == color) {
        return false;
    }
    strip_.SetPixelColor(pixel, color);
    return true;
}
