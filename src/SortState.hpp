#pragma once

#include "Coroutine.hpp"
#include "LedStrip.hpp"
#include "State.hpp"
#include "Util.hpp"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <cstdint>
#include <vector>

class LedStrip;

class SortState : public State {
  public:
    template <class Algo>
    SortState(LedStrip &led_strip, int32_t group_size, Algo)
        : ids_(led_strip.PixelCount() / group_size), led_strip_{led_strip},
          group_size_{group_size}, coroutine_{Algo::Sort, ids_.begin(),
                                              ids_.end()} {
        for (auto &color : ids_) {
            int32_t num = RandomInt(0, 510);
            if (num < 255) {
                color = ((255 - num) << 16) | (num << 8);
            } else {
                num -= 255;
                color = ((255 - num) << 8) | num;
            }
        }
    }

    void Update();

  private:
    void Display();

    std::vector<int32_t> ids_;
    LedStrip &led_strip_;
    const int32_t group_size_;
    Coroutine<int32_t, 2048> coroutine_;
};

struct BubbleSort {
    static void Sort(Yields<int32_t> yield,
                     std::vector<int32_t>::iterator begin,
                     std::vector<int32_t>::iterator end);
};

struct MergeSort {
    static void Sort(Yields<int32_t> yield,
                     std::vector<int32_t>::iterator begin,
                     std::vector<int32_t>::iterator end);
};

struct PMergeSort {
    static void Sort(Yields<int32_t> yield,
                     std::vector<int32_t>::iterator begin,
                     std::vector<int32_t>::iterator end);
};

struct QuickSort {
    static void Sort(Yields<int32_t> yield,
                     std::vector<int32_t>::iterator begin,
                     std::vector<int32_t>::iterator end);
};
