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
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        FastLED.show();
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
    FastLED.addLeds<WS2812B, kPin, GRB>(pixels_.data(), pixels_.size());
    Update();
}

void LedStrip::SetColor(int32_t pixel, CRGB color) {
    if (GetColor(pixel) != color) {
        pixels_[pixel] = color;
        stateChanged_ = true;
    }
}

int32_t LedStrip::PixelCount() { return static_cast<int32_t>(pixels_.size()); }

CRGB LedStrip::GetColor(int32_t pixel) { return pixels_[pixel]; }

void LedStrip::SetColors(CRGB color) { SetColors(0, PixelCount() - 1, color); }

void LedStrip::SetColors(int32_t first_pixel, int32_t last_pixel, CRGB color) {
    for (int32_t pixel = first_pixel; pixel <= last_pixel; ++pixel) {
        SetColor(pixel, color);
    }
}

void LedStrip::Gradient(CRGB first_color, CRGB last_color) {
    Gradient(0, PixelCount() - 1, first_color, last_color);
}

void LedStrip::Gradient(int32_t first_pixel, int32_t last_pixel,
                        CRGB first_color, CRGB last_color) {
    int32_t diff = last_pixel - first_pixel;
    int32_t i = diff;
    int32_t j = 0;
    for (int32_t pixel = first_pixel; pixel <= last_pixel; ++pixel) {
        CRGB color{static_cast<uint8_t>(first_color.red * i / diff +
                                        last_color.red * j / diff),
                   static_cast<uint8_t>(first_color.green * i / diff +
                                        last_color.green * j / diff),
                   static_cast<uint8_t>(first_color.blue * i / diff +
                                        last_color.blue * j / diff)};
        SetColor(pixel, color);
        --i;
        ++j;
    }
}

void LedStrip::Mirror() { Mirror(0, PixelCount() - 1); }

void LedStrip::Mirror(int32_t first_pixel, int32_t last_pixel) {
    int32_t i = 0;
    int32_t j = last_pixel - first_pixel;
    int32_t changes = (j + 1) / 2;
    while (i < changes) {
        int32_t left_pixel = first_pixel + i;
        int32_t right_pixel = last_pixel - j;
        CRGB left_color = GetColor(left_pixel);
        CRGB right_color = GetColor(right_pixel);
        SetColor(left_pixel, right_color);
        SetColor(right_pixel, left_color);
        ++i;
        --j;
    }
}

void LedStrip::Rotate(int32_t offset) { Rotate(0, PixelCount() - 1, offset); }

void LedStrip::Rotate(int32_t first_pixel, int32_t last_pixel, int32_t offset) {
    int32_t count = last_pixel - first_pixel + 1;
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
            SetColor(dst, GetColor(src));
            dst = src;
        }
        SetColor(dst, temp);
    }
}

void LedStrip::Update() {
    if (stateChanged_) {
        stateChanged_ = false;
        LedUpdater::Update();
    }
}
