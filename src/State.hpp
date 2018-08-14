#pragma once

class State {
  public:
    virtual ~State();
    virtual void Update() = 0;
};
