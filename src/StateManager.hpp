#pragma once

#include "LedStrip.hpp"
#include "states/MeteorState.hpp"
#include "states/RotatedRainbowState.hpp"
#include "states/SleepState.hpp"
#include "states/SmoothLightState.hpp"
#include "states/Christmas.hpp"
#include "states/SortState.hpp"
#include "states/WhiteState.hpp"
#include "util/FrameTimer.hpp"

class StateManager {
  public:
    StateManager();
    void Update();
    void NextState();

  private:
    void InitState();

    LedStrip led_strip_;
    FrameTimer frame_timer_;

    int32_t current_state_index_ = 7; // SmoothLight
    std::unique_ptr<State> current_state_;

    const std::vector<std::function<std::unique_ptr<State>()>> state_factories_{
        [&]() { return std::make_unique<WhiteState>(led_strip_); },
        [&]() { return std::make_unique<RotatedRainbowState>(led_strip_); },
        [&]() {
            return std::make_unique<SortState>(led_strip_, 8, BubbleSort{});
        },
        [&]() {
            return std::make_unique<SortState>(led_strip_, 1, MergeSort{});
        },
        [&]() {
            return std::make_unique<SortState>(led_strip_, 1, PMergeSort{});
        },
        [&]() {
            return std::make_unique<SortState>(led_strip_, 4, QuickSort{});
        },
        [&]() { return std::make_unique<MeteorState>(led_strip_); },
        [&]() { return std::make_unique<SmoothLightState>(led_strip_); },
        [&]() { return std::make_unique<Christmas>(led_strip_); },
        [&]() { return std::make_unique<SleepState>(led_strip_); }};

    static constexpr const int32_t kPixelCount = 850;
};