#include "LedStrip.hpp"
#include "Util.hpp"

constexpr const uint8_t kPin = 4;

class LedUpdater {
  public:
    static void Update();

  private:
    static void Show(void *);

    LedUpdater();

    static LedUpdater kInstance;

    TaskHandle_t waiting_task_ = 0;
};

void LedUpdater::Update() {
    xTaskNotifyGive(LedUpdater::kInstance.waiting_task_);
}

void LedUpdater::Show(void *) {
    while (true) {
        if (ulTaskNotifyTake(pdTRUE, portMAX_DELAY)) {
            FastLED.show();
        }
    }
}

LedUpdater LedUpdater::kInstance{};

LedUpdater::LedUpdater() {
    xTaskCreate(Show, "Show",    // name
                2048,            // stack depth
                nullptr,         // passed parameter
                2,               // priority
                &waiting_task_); // task handle
}

LedStrip::LedStrip(int32_t pixelCount) : pixels_(pixelCount) {
    SetColors(CRGB::Black);
    FastLED.addLeds<WS2812B, kPin, GRB>(pixels_.data(), pixels_.size());
    Update();
}

int32_t LedStrip::PixelCount() { return static_cast<int32_t>(pixels_.size()); }

CRGB LedStrip::GetColor(int32_t pixel) const { return pixels_[pixel]; }

void LedStrip::SetColor(CRGB color, int32_t pixel) {
    if (GetColor(pixel) != color) {
        pixels_[pixel] = color;
        stateChanged_ = true;
    }
}

void LedStrip::SetColors(CRGB color, int32_t first_pixel) {
    SetColors(color, first_pixel, PixelCount() - first_pixel);
}

void LedStrip::SetColors(CRGB color, int32_t first_pixel, int32_t count) {
    for (int32_t pixel = first_pixel; pixel < first_pixel + count; ++pixel) {
        SetColor(color, pixel);
    }
}

void LedStrip::Gradient(CRGB first_color, CRGB last_color,
                        int32_t first_pixel) {
    Gradient(first_color, last_color, first_pixel, PixelCount() - first_pixel);
}

void LedStrip::Gradient(CRGB first_color, CRGB last_color, int32_t first_pixel,
                        int32_t count) {
    int32_t diff = count - 1;
    int32_t i = diff;
    int32_t j = 0;
    for (int32_t pixel = first_pixel; pixel < first_pixel + count; ++pixel) {
        CRGB color{static_cast<uint8_t>(first_color.red * i / diff +
                                        last_color.red * j / diff),
                   static_cast<uint8_t>(first_color.green * i / diff +
                                        last_color.green * j / diff),
                   static_cast<uint8_t>(first_color.blue * i / diff +
                                        last_color.blue * j / diff)};
        SetColor(color, pixel);
        --i;
        ++j;
    }
}

void LedStrip::Mirror(int32_t first_pixel) {
    Mirror(first_pixel, PixelCount() - first_pixel);
}

void LedStrip::Mirror(int32_t first_pixel, int32_t count) {
    int32_t i = 0;
    int32_t j = count - 1;
    while (i < j) {
        int32_t left_pixel = first_pixel + i;
        int32_t right_pixel = first_pixel + j;
        CRGB left_color = GetColor(left_pixel);
        CRGB right_color = GetColor(right_pixel);
        SetColor(right_color, left_pixel);
        SetColor(left_color, right_pixel);
        ++i;
        --j;
    }
}

void LedStrip::Rotate(int32_t offset, int32_t first_pixel) {
    Rotate(offset, first_pixel, PixelCount() - first_pixel);
}

void LedStrip::Rotate(int32_t offset, int32_t first_pixel, int32_t count) {
    int32_t last_pixel = first_pixel + count - 1;
    while (offset < 0) {
        offset += count;
    }
    while (offset >= count) {
        offset -= count;
    }
    int32_t bound = last_pixel - Gcd(count, offset);
    for (int32_t start = last_pixel; start > bound; --start) {
        int32_t dst = start;
        CRGB temp = GetColor(dst);
        while (true) {
            int32_t src = dst - offset;
            if (src < first_pixel) {
                src += count;
            }
            if (src == start) {
                break;
            }
            SetColor(GetColor(src), dst);
            dst = src;
        }
        SetColor(temp, dst);
    }
}

void LedStrip::Update() {
    if (stateChanged_) {
        stateChanged_ = false;
        LedUpdater::Update();
    }
}
