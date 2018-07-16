#include "LedStrip.hpp"
#include "Util.hpp"

LedStrip::LedStrip(uint16_t pixelCount, uint8_t pin) : strip_{pixelCount, pin} {
    strip_.Begin();
    strip_.Show(); // Initialize all pixels to 'off'
}

void LedStrip::SetColor(uint16_t pixel, RgbColor color) {
    if (GetColor(pixel) != color) {
        strip_.SetPixelColor(pixel, color);
        stateChanged_ = true;
    }
}

uint16_t LedStrip::PixelCount() { return strip_.PixelCount(); }

RgbColor LedStrip::GetColor(uint16_t pixel) {
    return strip_.GetPixelColor(pixel);
}

void LedStrip::SetColors(RgbColor color) {
    SetColors(0, PixelCount() - 1, color);
}

void LedStrip::SetColors(uint16_t first_pixel, uint16_t last_pixel,
                         RgbColor color) {
    for (uint16_t pixel = first_pixel; pixel <= last_pixel; ++pixel) {
        SetColor(pixel, color);
    }
}

void LedStrip::Gradient(RgbColor first_color, RgbColor last_color) {
    Gradient(0, PixelCount() - 1, first_color, last_color);
}

void LedStrip::Gradient(uint16_t first_pixel, uint16_t last_pixel,
                        RgbColor first_color, RgbColor last_color) {
    uint16_t diff = last_pixel - first_pixel;
    uint16_t i = 0;
    uint16_t j = diff;
    for (uint16_t pixel = first_pixel; pixel <= last_pixel; ++pixel) {
        RgbColor color{first_color.R * i / diff + last_color.R * j / diff,
                       first_color.G * i / diff + last_color.G * j / diff,
                       first_color.B * i / diff + last_color.B * j / diff};
        SetColor(pixel, color);
        ++i;
        --j;
    }
}

void LedStrip::Mirror() { Mirror(0, PixelCount() - 1); }

void LedStrip::Mirror(uint16_t first_pixel, uint16_t last_pixel) {
    uint16_t i = 0;
    uint16_t j = last_pixel - first_pixel;
    uint16_t changes = (j + 1) / 2;
    while (i < changes) {
        uint16_t left_pixel = first_pixel + i;
        uint16_t right_pixel = last_pixel - j;
        RgbColor left_color = GetColor(left_pixel);
        RgbColor right_color = GetColor(right_pixel);
        SetColor(left_pixel, right_color);
        SetColor(right_pixel, left_color);
        ++i;
        --j;
    }
}

void LedStrip::Rotate(int16_t offset) { Rotate(0, PixelCount() - 1, offset); }

void LedStrip::Rotate(uint16_t first_pixel, uint16_t last_pixel,
                      int16_t offset) {
    uint16_t count = last_pixel - first_pixel + 1;
    while (offset < 0) {
        offset += count;
    }
    while (offset >= count) {
        offset -= count;
    }
    uint16_t bound = last_pixel - gcd(count, static_cast<uint16_t>(offset));
    for (uint16_t start = last_pixel; start > bound; --start) {
        uint16_t dst = start;
        RgbColor temp = GetColor(dst);
        while (true) {
            uint16_t src = dst - offset;
            if (src < first_pixel) {
                src += count;
            }
            if (src == start) {
                break;
            }
            SetColor(dst, GetColor(src));
            dst = src;
        }
        SetColor(dst, temp);
    }
}

void LedStrip::Update() {
    if (stateChanged_) {
        stateChanged_ = false;
        strip_.Show();
    }
}
