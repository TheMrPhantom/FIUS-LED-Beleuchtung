#include "StateManager.hpp"

StateManager::StateManager() : led_strip_(kPixelCount), frame_timer_(33) {
    InitState();
}

void StateManager::Update() {
    while (frame_timer_.NextFrame()) {
        if (frame_timer_.FrameCount() % 100 == 0) {
            Serial.print("State update: ");
            frame_timer_.Debug();
        }
        current_state_->Update();
    }
    led_strip_.Update();
}

void StateManager::NextState() {
    ++current_state_index_;
    current_state_index_ %= state_factories_.size();
    InitState();
}

void StateManager::InitState() {
    current_state_ = state_factories_[current_state_index_]();
}
