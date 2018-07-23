#pragma once

#include "State.hpp"

class SleepState : public State {
  public:
    SleepState(LedStrip &) {}
    void Update() override {}
};
