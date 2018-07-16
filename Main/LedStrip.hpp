#pragma once
#include <NeoPixelBus.h>

class LedStrip {
  public:
    LedStrip(uint16_t pixelCount, uint8_t pin);
    LedStrip(const LedStrip &) = delete;
    uint16_t PixelCount();
    RgbColor GetColor(uint16_t pixel);
    void SetColor(uint16_t pixel, RgbColor color);
    void SetColors(RgbColor color);
    void SetColors(uint16_t first_pixel, uint16_t last_pixel, RgbColor color);
    void Gradient(RgbColor first_color, RgbColor last_color);
    void Gradient(uint16_t first_pixel, uint16_t last_pixel,
                  RgbColor first_color, RgbColor last_color);
    void Mirror();
    void Mirror(uint16_t first_pixel, uint16_t last_pixel);
    void Rotate(int16_t offset);
    void Rotate(uint16_t first_pixel, uint16_t last_pixel, int16_t offset);
    void Update();

  private:
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip_;
    bool stateChanged_ = false;
};
