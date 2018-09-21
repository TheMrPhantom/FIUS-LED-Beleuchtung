#pragma once

#include "State.hpp"

class LedStrip;

class WhiteState : public State {
  public:
    WhiteState(LedStrip &led_strip);
    void Update() override;
};
