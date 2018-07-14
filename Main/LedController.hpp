#pragma once
#include <NeoPixelBus.h>

class LedController {
  public:
    LedController();
    ~LedController();
    LedController(const LedController &) = delete;
    void Update();
    void SetTicksLeftActive(int ticks);

  private:
    void RefreshLeds();
	bool ChangePixelColor(uint16_t pixel, RgbColor color);

	NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip_;
    int ticksLeftActive_ = 0;
};
