#pragma once

#include "State.hpp"

class LedStrip;

class SleepState : public State {
  public:
    SleepState(LedStrip &);
    void Update() override;
};
