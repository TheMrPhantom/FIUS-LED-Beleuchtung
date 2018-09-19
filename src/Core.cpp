#include "Core.hpp"

Core::Core(WebServer &web_server)
    : web_server_(web_server), led_strip_(kPixelCount), frame_timer_(33) {
    InitState();
    web_server.Wip_SetCallback([&]() {
        ++current_state_index_;
        current_state_index_ %= state_factories_.size();
        InitState();
    });
}

void Core::Update() {
    while (frame_timer_.NextFrame()) {
        if (frame_timer_.FrameCount() % 100 == 0) {
            Serial.print("State update: ");
            frame_timer_.Debug();
        }
        current_state_->Update();
    }
    led_strip_.Update();
}

void Core::InitState() {
    current_state_ = state_factories_[current_state_index_]();
}
