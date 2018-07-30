#include "SortState.hpp"
#include "LedStrip.hpp"
#include "Util.hpp"

#include <algorithm>

namespace {
void Display(LedStrip &led_strip, const std::vector<int32_t> &ids) {
    int32_t next_pixel = 0;
    for (int32_t group = 0; group < ids.size(); ++group) {
        int32_t pixel = next_pixel;
        next_pixel = led_strip.PixelCount() * (group + 1) /
                     static_cast<int32_t>(ids.size());
        led_strip.SetColors(ids[group], pixel, next_pixel - pixel);
    }
}
} // namespace

SortStateBase::SortStateBase(LedStrip &led_strip, int32_t group_size)
    : ids_(led_strip.PixelCount() / group_size), led_strip_{led_strip},
      group_size_{group_size}, my_task_{xTaskGetCurrentTaskHandle()} {
    for (auto &color : ids_) {
        int32_t num = RandomInt(0, 510);
        if (num < 255) {
            color = ((255 - num) << 16) | (num << 8);
        } else {
            num -= 255;
            color = ((255 - num) << 8) | num;
        }
    }
    xTaskCreate(
        [](void *ptr) {
            ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
            auto &base = *static_cast<SortStateBase *>(ptr);
            base.Sort();
            base.co_task_ = nullptr;
            xTaskNotifyGive(base.my_task_);
            vTaskDelete(nullptr);
        },
        "", 2048, this, 2, &co_task_);
}

void SortStateBase::Update() {
    if (co_task_) {
        xTaskNotifyGive(co_task_);
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        Display(led_strip_, ids_);
    }
}

SortStateBase::~SortStateBase() {
    if (co_task_) {
        vTaskDelete(co_task_);
    }
}

void SortStateBase::FinishedStep() {
    xTaskNotifyGive(my_task_);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
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
    auto diff = end - begin;
    auto mid = begin + diff / 2;
    if (diff > 2) {
        Sort(begin, mid);
        Sort(mid, end);
    }
    std::vector<int32_t> merge(begin, end);
    std::for_each(begin, end, [](auto &c) {
        CRGB color{c};
        color /= 8;
        c = (color.r << 16) | (color.g << 8) | color.b;
    });
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
