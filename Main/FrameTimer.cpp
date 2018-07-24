#include "FrameTimer.hpp"

#include <HardwareSerial.h>

FrameTimer::FrameTimer(int32_t frame_time) : target_frame_time_{frame_time} {}

bool FrameTimer::NextFrame() {
    if (last_frame_end_ == -1) {
        last_frame_end_ = first_frame_end_ = millis();
        target_frame_end_ = last_frame_end_ + target_frame_time_;
        return true;
    }
    int64_t m = millis();
    if (m >= target_frame_end_) {
        if (m - 100 > target_frame_end_) {
            target_frame_end_ = m - 100;
        }
        ++frame_count_;
        last_frame_time_ = m - last_frame_end_;
        last_frame_end_ = m;
        target_frame_end_ += target_frame_time_;
        return true;
    }
    return false;
}

void FrameTimer::Debug() {
    if (frame_count_ != 0) {
        Serial.print("Avg frame time: ");
        Serial.print(static_cast<double>(last_frame_end_ - first_frame_end_) /
                     frame_count_);
        Serial.println("ms");
    }
}
