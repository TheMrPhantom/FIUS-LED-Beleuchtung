#pragma once

#include <cstdint>

class FrameTimer {
  public:
    FrameTimer(int32_t frame_time);
    int32_t FrameCount();
    bool NextFrame();
    void Debug();

  private:
    const int32_t target_frame_time_;
    int64_t target_frame_end_;
    int32_t last_frame_time_ = -1;
    int64_t last_frame_end_ = -1;
    int64_t first_frame_end_;
    int64_t frame_count_ = 0;
};
