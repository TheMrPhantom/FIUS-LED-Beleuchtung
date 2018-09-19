#include "FrameTimer.hpp"
#include "LedStrip.hpp"
#include "MeteorState.hpp"
#include "RotatedRainbowState.hpp"
#include "SleepState.hpp"
#include "SmoothLightState.hpp"
#include "SortState.hpp"
#include "Util.hpp"
#include "WebServer.hpp"
#include "WhiteState.hpp"
#include "WifiGateway.hpp"

#include <experimental/optional>
#include <fstream>
#include <iostream>
#include <vector>

const int32_t kPixelCount = 850;

std::experimental::optional<LedStrip> led_strip;
std::experimental::optional<WifiGateway> wifi_gateway;
std::experimental::optional<WebServer> web_server;
std::unique_ptr<State> current_state;

std::vector<std::function<std::unique_ptr<State>()>> kStateFactories{
    [&]() { return std::make_unique<WhiteState>(*led_strip); },
    [&]() { return std::make_unique<RotatedRainbowState>(*led_strip); },
    [&]() { return std::make_unique<SortState>(*led_strip, 8, BubbleSort{}); },
    [&]() { return std::make_unique<SortState>(*led_strip, 1, MergeSort{}); },
    [&]() { return std::make_unique<SortState>(*led_strip, 1, PMergeSort{}); },
    [&]() { return std::make_unique<SortState>(*led_strip, 4, QuickSort{}); },
    [&]() { return std::make_unique<MeteorState>(*led_strip); },
    [&]() { return std::make_unique<SmoothLightState>(*led_strip); },
    [&]() { return std::make_unique<SleepState>(*led_strip); }};
int32_t current_state_index = 7; // SmoothLight

void InitState() { current_state = kStateFactories[current_state_index](); }

void HandleWifiRequest() {
    if (auto req = wifi_gateway->NextRequest()) {
        try {
            auto message = req.message();
            std::cout << "New request: " << message << '\n';
            req.answer(web_server->Process(message));
            std::cout << "Answered request.\n";
        } catch (std::exception &e) {
            std::cerr << '\n' << e.what() << "\n\n";
        }
    }
}

void setup() {
    Serial.begin(115200);
    led_strip.emplace(kPixelCount);
    wifi_gateway.emplace();
    SPIFFS.begin();
    web_server.emplace();
    web_server->Wip_SetCallback([&]() {
        ++current_state_index;
        current_state_index %= kStateFactories.size();
        InitState();
    });
    InitState();
}

void loop() {
    static FrameTimer timer{33};

    HandleWifiRequest();
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
