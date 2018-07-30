#include "SortState.hpp"

namespace {
void Display(LedStrip &led_strip, const std::vector<int32_t> &ids) {
    int32_t next_pixel = 0;
    for (int32_t group = 0; group < ids.size(); ++group) {
        int32_t pixel = next_pixel;
        next_pixel = led_strip.PixelCount() * (group + 1) /
                     static_cast<int32_t>(ids.size());
        auto id = ids[group];
        CRGB color;
        if (id < 0) {
            color = CRGB::Black;
        } else if (id < 255) {
            color = CRGB{static_cast<uint8_t>(255 - id),
                         static_cast<uint8_t>(id), 0};
        } else {
            id -= 255;
            color = CRGB{0, static_cast<uint8_t>(255 - id),
                         static_cast<uint8_t>(id)};
        }
        led_strip.SetColors(color, pixel, next_pixel - pixel);
    }
}
} // namespace

SortStateBase::SortStateBase(LedStrip &led_strip, int32_t group_size)
    : ids_(led_strip.PixelCount() / group_size), led_strip_{led_strip},
      group_size_{group_size} {
    for (auto &id : ids_) {
        id = RandomInt(0, 510);
    }
}

void SortStateBase::Update() {
    SortStep();
    Display(led_strip_, ids_);
}

void BubbleSortState::SortStep() {
    if (max_group_ > 0) {
        if (ids_[current_group_] > ids_[current_group_ + 1]) {
            std::swap(ids_[current_group_], ids_[current_group_ + 1]);
        }
        ++current_group_;
        if (current_group_ == max_group_) {
            current_group_ = 0;
            --max_group_;
        }
    }
}

void MergeSortState::SortStep() {
    auto right_end = merge_.end();
    auto left_end = merge_.begin() + merge_.size() / 2;
    if (left_ == left_end && right_ == right_end) {
        if (position_ & 1) {
            return;
        }
        auto begin = ids_.begin();
        auto end = ids_.end();
        int32_t bit = 1;
        while (end - begin > (2 << anti_depth_)) {
            bit <<= 1;
            if (position_ & bit) {
                begin += (end - begin) / 2;
            } else {
                end = begin + (end - begin) / 2;
            }
        }
        merge_.assign(begin, end);
        left_ = merge_.begin();
        right_end = merge_.end();
        left_end = right_ = merge_.begin() + merge_.size() / 2;
        dst_ = begin;
        std::fill(begin, end, -1);
        ++anti_depth_;
        if ((position_ ^= bit) & bit) {
            anti_depth_ = 0;
        }
    }
    if (left_ == left_end) {
        *(dst_++) = *(right_++);
    } else if (right_ == right_end || *right_ > *left_) {
        *(dst_++) = *(left_++);
    } else {
        *(dst_++) = *(right_++);
    }
}
