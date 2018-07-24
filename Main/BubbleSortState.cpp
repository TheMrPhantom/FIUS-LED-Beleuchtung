#include "BubbleSortState.hpp"

BubbleSortState::BubbleSortState(LedStrip &led_strip)
    : led_strip_{led_strip}, ids_(led_strip_.PixelCount() / kGroupSize),
      max_group_{static_cast<int32_t>(ids_.size()) - 1} {
    for (auto &id : ids_) {
        id = RandomInt(0, 510);
    }
}

void BubbleSortState::Update() {
    if (max_group_ > 0) {
        if (ids_[current_group_] > ids_[current_group_ + 1]) {
            std::swap(ids_[current_group_], ids_[current_group_ + 1]);
        }
        ++current_group_;
        if (current_group_ == max_group_) {
            current_group_ = 0;
            --max_group_;
        }
        Display();
    }
}

void BubbleSortState::Display() {
    int32_t right_pixel = 0;
    for (int32_t group = 0; group < ids_.size(); ++group) {
        int32_t left_pixel = right_pixel;
        right_pixel = led_strip_.PixelCount() * (group + 1) /
                      static_cast<int32_t>(ids_.size());
        led_strip_.SetColors(left_pixel, right_pixel - 1,
                             IdToColor(ids_[group]));
    }
}

RgbColor BubbleSortState::IdToColor(int32_t id) {
    if (id < 255) {
        return RgbColor{static_cast<uint8_t>(255 - id),
                        static_cast<uint8_t>(id), 0};
    }
    id -= 255;
    return RgbColor{0, static_cast<uint8_t>(255 - id),
                    static_cast<uint8_t>(id)};
}
