#pragma once

#include "State.hpp"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <cstdint>
#include <vector>

class LedStrip;

class SortStateBase : public State {
  public:
    SortStateBase(LedStrip &led_strip, int32_t group_size);
    void Update() override;

  protected:
    void FinishedStep();
    virtual void Sort() = 0;

    std::vector<int32_t> ids_;

  private:
    LedStrip &led_strip_;
    const int32_t group_size_;
    TaskHandle_t next_task_;
};

class BubbleSortState : public SortStateBase {
  public:
    using SortStateBase::SortStateBase;

  protected:
    void Sort() override;
};

class MergeSortState : public SortStateBase {
  public:
    using SortStateBase::SortStateBase;

  protected:
    void Sort() override;

  private:
    void Sort(std::vector<int32_t>::iterator begin,
              std::vector<int32_t>::iterator end);
};
