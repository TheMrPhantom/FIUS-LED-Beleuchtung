#include "SortState.hpp"
#include "LedStrip.hpp"
#include "Util.hpp"

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
    xTaskCreate(
        [](void *inst) {
            ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
            static_cast<SortStateBase *>(inst)->Sort();
        },
        "", 2048, this, 2, &next_task_);
}

void SortStateBase::Update() {
    printf("Debug2\n");
    auto sort_task = next_task_;
    next_task_ = xTaskGetCurrentTaskHandle();
    xTaskNotifyGive(sort_task);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    next_task_ = sort_task;
    Display(led_strip_, ids_);
    printf("Debug5\n");
}

void SortStateBase::FinishedStep() {
    printf("Debug4\n");
    xTaskNotifyGive(next_task_);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    printf("Debug3.1\n");
}

void BubbleSortState::Sort() {
    for (int32_t end = ids_.size() - 1; end >= 1; --end) {
        for (int32_t i = 0; i < end; ++i) {
            if (ids_[i] > ids_[i + 1]) {
                std::swap(ids_[i], ids_[i + 1]);
            }
            FinishedStep();
        }
    }
}

void MergeSortState::Sort() { Sort(ids_.begin(), ids_.end()); }

void MergeSortState::Sort(std::vector<int32_t>::iterator begin,
                          std::vector<int32_t>::iterator end) {
    printf("Debug3.0\n");
    auto diff = end - begin;
    auto mid = begin + diff / 2;
    if (diff > 2) {
        Sort(begin, mid);
        Sort(mid, end);
    }
    std::vector<int32_t> merge(begin, end);
    std::fill(begin, end, -1);
    auto left = merge.begin();
    auto left_end = merge.begin() + diff / 2;
    auto right = left_end;
    auto right_end = merge.end();
    while (true) {
        if (left == left_end) {
            while (right != right_end) {
                *(begin++) = *(right++);
                FinishedStep();
            }
            return;
        }
        if (right == right_end) {
            while (left != left_end) {
                *(begin++) = *(left++);
                FinishedStep();
            }
            return;
        }
        *(begin++) = (*left <= *right) ? *(left++) : *(right++);
        FinishedStep();
    }
}
