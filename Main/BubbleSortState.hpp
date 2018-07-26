#pragma once

#include "State.hpp"

class BubbleSortState : public State {
  public:
    BubbleSortState(LedStrip &led_strip);
    void Update() override;

  private:
    void Display();
    CRGB IdToColor(int32_t id);

    static const int32_t kGroupSize = 8;

    LedStrip &led_strip_;
    std::vector<int32_t> ids_;
    int32_t current_group_ = 0;
    int32_t max_group_;
};
