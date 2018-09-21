#include "Core.hpp"

Core::Core() : led_strip_(kPixelCount), frame_timer_(33) {
    web_server_.Wip_SetCallback([&]() {
        ++current_state_index_;
        current_state_index_ %= state_factories_.size();
        InitState();
    });
    InitState();
}

WebServer &Core::Server() { return web_server_; }

const WebServer &Core::Server() const { return web_server_; }

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
