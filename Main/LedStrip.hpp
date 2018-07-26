#pragma once
#include <FastLED.h>

#include <vector>

class LedStrip {
  public:
    LedStrip(int32_t pixelCount);
    LedStrip(const LedStrip &) = delete;
    int32_t PixelCount();
    CRGB GetColor(int32_t pixel);
    void SetColor(int32_t pixel, CRGB color);
    void SetColors(CRGB color);
    void SetColors(int32_t first_pixel, int32_t last_pixel, CRGB color);
    void Gradient(CRGB first_color, CRGB last_color);
    void Gradient(int32_t first_pixel, int32_t last_pixel, CRGB first_color,
                  CRGB last_color);
    void Mirror();
    void Mirror(int32_t first_pixel, int32_t last_pixel);
    void Rotate(int32_t offset);
    void Rotate(int32_t first_pixel, int32_t last_pixel, int32_t offset);
    void Update();

  private:
    std::vector<CRGB> pixels_;
    bool stateChanged_ = true;
};
