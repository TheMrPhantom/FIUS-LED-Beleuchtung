#pragma once
#include <FastLED.h>
#undef min
#undef max

#include <vector>

class LedStrip {
  public:
    LedStrip(int32_t pixelCount);
    LedStrip(const LedStrip &) = delete;
    int32_t PixelCount();
    CRGB GetColor(int32_t pixel) const;
    void SetColor(CRGB color, int32_t pixel);
    void SetColors(CRGB color, int32_t first_pixel = 0);
    void SetColors(CRGB color, int32_t first_pixel, int32_t count);
    void Gradient(CRGB first_color, CRGB last_color, int32_t first_pixel = 0);
    void Gradient(CRGB first_color, CRGB last_color, int32_t first_pixel,
                  int32_t count);
    void Mirror();
    void Mirror(int32_t first_pixel = 0);
    void Mirror(int32_t first_pixel, int32_t count);
    void Rotate(int32_t offset, int32_t first_pixel = 0);
    void Rotate(int32_t offset, int32_t first_pixel, int32_t count);
    void Update();

  private:
    std::vector<CRGB> pixels_;
    bool stateChanged_ = true;
};
