#include "BubbleSortState.hpp"
#include "FrameTimer.hpp"
#include "MeteorState.hpp"
#include "RotatedRainbowState.hpp"
#include "SleepState.hpp"
#include "Util.hpp"
#include "WhiteState.hpp"
#include "WifiGateway.hpp"

#include <vector>

const int32_t kPixelCount = 850;
const uint8_t kPin = 4;
const std::array<std::unique_ptr<StateFactory>, 5> kStateFactories{
    MakeStateFactory<WhiteState>(), MakeStateFactory<RotatedRainbowState>(),
    MakeStateFactory<BubbleSortState>(), MakeStateFactory<MeteorState>(),
    MakeStateFactory<SleepState>()};

int32_t current_state_index = 2; // Meteor

std::unique_ptr<LedStrip> led_strip;
std::unique_ptr<WifiGateway> wifi_gateway;
std::unique_ptr<State> current_state;

void InitState() {
    current_state = kStateFactories[current_state_index]->create(*led_strip);
}

void IncrementState() {
    ++current_state_index;
    current_state_index %= kStateFactories.size();
    InitState();
}

void setup() {
    Serial.begin(115200);
    led_strip = make_unique<LedStrip>(kPixelCount, kPin);
    wifi_gateway = make_unique<WifiGateway>(IncrementState);
    InitState();
}

void loop() {
    static FrameTimer timer{33};
    wifi_gateway->Update();
    if (timer.NextFrame()) {
        timer.Debug();
        current_state->Update();
        led_strip->Update();
    }
}
