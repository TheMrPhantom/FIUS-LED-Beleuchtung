#pragma once

#include "LedStrip.hpp"
#include "Util.hpp"

#include <memory>

class State {
  public:
    virtual ~State();
    virtual void Update() = 0;
};

class StateFactory {
  public:
    virtual std::unique_ptr<State> create(LedStrip &led_strip) = 0;
    virtual ~StateFactory() {}
};

template <class DerivedState> class DerivedStateFactory : public StateFactory {
  public:
    std::unique_ptr<State> create(LedStrip &led_strip) override {
        return make_unique<DerivedState>(led_strip);
    }
};

template <class DerivedState>
std::unique_ptr<DerivedStateFactory<DerivedState>> MakeStateFactory() {
    return make_unique<DerivedStateFactory<DerivedState>>();
}
