#pragma once
#include <NeoPixelBus.h>

class LedStrip {
  public:
    LedStrip(int32_t pixelCount, uint8_t pin);
    LedStrip(const LedStrip &) = delete;
    int32_t PixelCount();
    RgbColor GetColor(int32_t pixel);
    void SetColor(int32_t pixel, RgbColor color);
    void SetColors(RgbColor color);
    void SetColors(int32_t first_pixel, int32_t last_pixel, RgbColor color);
    void Gradient(RgbColor first_color, RgbColor last_color);
    void Gradient(int32_t first_pixel, int32_t last_pixel, RgbColor first_color,
                  RgbColor last_color);
    void Mirror();
    void Mirror(int32_t first_pixel, int32_t last_pixel);
    void Rotate(int32_t offset);
    void Rotate(int32_t first_pixel, int32_t last_pixel, int32_t offset);
    void Update();

  private:
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip_;
    bool stateChanged_ = false;
};
