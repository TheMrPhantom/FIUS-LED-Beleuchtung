#pragma once

#include "State.hpp"

#include <algorithm>

class SortStateBase : public State {
  public:
    SortStateBase(LedStrip &led_strip, int32_t group_size);
    void Update() override;

  protected:
    virtual void SortStep() = 0;

    std::vector<int32_t> ids_;

  private:
    LedStrip &led_strip_;
    const int32_t group_size_ = 8;
};

class BubbleSortState : public SortStateBase {
  public:
    using SortStateBase::SortStateBase;

  protected:
    void SortStep() override;

  private:
    int32_t current_group_ = 0;
    int32_t max_group_ = static_cast<int32_t>(ids_.size()) - 1;
};

class MergeSortState : public SortStateBase {
  public:
    using SortStateBase::SortStateBase;

  protected:
    void SortStep() override;

  private:
    int32_t position_ = 0;
    int32_t anti_depth_ = 0;
};
