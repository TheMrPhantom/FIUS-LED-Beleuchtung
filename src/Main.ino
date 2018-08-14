#include "FrameTimer.hpp"
#include "LedStrip.hpp"
#include "MeteorState.hpp"
#include "RotatedRainbowState.hpp"
#include "SleepState.hpp"
#include "SortState.hpp"
#include "Util.hpp"
#include "WhiteState.hpp"
#include "WifiGateway.hpp"

#include <experimental/optional>
#include <vector>

const int32_t kPixelCount = 850;

std::experimental::optional<LedStrip> led_strip;
std::experimental::optional<WifiGateway> wifi_gateway;
std::unique_ptr<State> current_state;

std::vector<std::function<std::unique_ptr<State>()>> kStateFactories{
    [&]() { return std::make_unique<WhiteState>(*led_strip); },
    [&]() { return std::make_unique<RotatedRainbowState>(*led_strip); },
    [&]() { return std::make_unique<SortState>(*led_strip, 8, BubbleSort{}); },
    [&]() { return std::make_unique<SortState>(*led_strip, 1, MergeSort{}); },
    [&]() { return std::make_unique<SortState>(*led_strip, 1, PMergeSort{}); },
    [&]() { return std::make_unique<SortState>(*led_strip, 4, QuickSort{}); },
    [&]() { return std::make_unique<MeteorState>(*led_strip); },
    [&]() { return std::make_unique<SleepState>(*led_strip); }};
int32_t current_state_index = 4; // PMerge Sort

void InitState() { current_state = kStateFactories[current_state_index](); }

void AnswerRequest(Request &req) {
    Serial.println("Message:");
    Serial.print(req.message());
    ++current_state_index;
    current_state_index %= kStateFactories.size();
    InitState();
    req.answer("HTTP/1.1 200 OK\n"
               "Content-Type: text/html\n"
               "Connection: close\n"
               "\n"
               "Hello, World!.\n");
}

void setup() {
    Serial.begin(115200);
    led_strip.emplace(kPixelCount);
    wifi_gateway.emplace();
    InitState();
}

void loop() {
    static FrameTimer timer{33};

    if (auto req = wifi_gateway->NextRequest()) {
        AnswerRequest(req);
    }
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
