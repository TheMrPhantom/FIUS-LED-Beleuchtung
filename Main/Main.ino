#include "FrameTimer.hpp"
#include "LedStrip.hpp"
#include "MeteorState.hpp"
#include "RotatedRainbowState.hpp"
#include "SleepState.hpp"
#include "SortState.hpp"
#include "Util.hpp"
#include "WhiteState.hpp"
#include "WifiGateway.hpp"

#include <vector>

const int32_t kPixelCount = 850;

std::unique_ptr<LedStrip> led_strip;
std::unique_ptr<WifiGateway> wifi_gateway;
std::unique_ptr<State> current_state;

std::vector<std::function<std::unique_ptr<State>()>> kStateFactories{
    [&]() { return make_unique<WhiteState>(*led_strip); },
    [&]() { return make_unique<RotatedRainbowState>(*led_strip); },
    [&]() { return make_unique<BubbleSortState>(*led_strip, 8); },
    [&]() { return make_unique<MergeSortState>(*led_strip, 1); },
    [&]() { return make_unique<MeteorState>(*led_strip); },
    [&]() { return make_unique<SleepState>(*led_strip); }};
int32_t current_state_index = 3; // Merge Sort

void InitState() { current_state = kStateFactories[current_state_index](); }

void IncrementState() {
    ++current_state_index;
    current_state_index %= kStateFactories.size();
    InitState();
}

void setup() {
    Serial.begin(115200);
    led_strip = make_unique<LedStrip>(kPixelCount);
    wifi_gateway = make_unique<WifiGateway>(IncrementState);
    InitState();
}

void loop() {
    static FrameTimer timer{33};

    wifi_gateway->Update();
    while (timer.NextFrame()) {
        static int32_t frame = 0;
        ++frame;
        if (frame % 100 == 0) {
            Serial.print("State update: ");
            timer.Debug();
        }
        current_state->Update();
    }
    led_strip->Update();
}
