#pragma once

#include "LedStrip.hpp"
#include "Util.hpp"

#include <memory>
#include <tuple>
#include <utility>

class State {
  public:
    virtual ~State();
    virtual void Update() = 0;
};
