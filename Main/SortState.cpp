#include "SortState.hpp"

#include <algorithm>

void SortState::Update() {
    if (coroutine_.Run()) {
        Display();
    }
}

void SortState::Display() {
    int32_t next_pixel = 0;
    for (int32_t group = 0; group < ids_.size(); ++group) {
        int32_t pixel = next_pixel;
        next_pixel = led_strip_.PixelCount() * (group + 1) /
                     static_cast<int32_t>(ids_.size());
        led_strip_.SetColors(ids_[group], pixel, next_pixel - pixel);
    }
}

namespace {
template <class Iter>
Iter MedianIter(Iter a, Iter b, Iter c) noexcept(noexcept(*a)) {
    auto min = [](Iter a, Iter b) noexcept(noexcept(*a))->Iter {
        return *a < *b ? a : b;
    };
    auto max = [](Iter a, Iter b) noexcept(noexcept(*a))->Iter {
        return *a > *b ? a : b;
    };
    return max(min(a, b), min(max(a, b), c));
}
} // namespace

void BubbleSort::Sort(Yields_t<int32_t> yield,
                      std::vector<int32_t>::iterator begin,
                      std::vector<int32_t>::iterator end) {
    while (--end > begin) {
        for (auto iter = begin; iter < end; ++iter) {
            if (*iter > *(iter + 1)) {
                std::swap(*iter, *(iter + 1));
            }
            yield(0);
        }
    }
}

void MergeSort::Sort(Yields_t<int32_t> yield,
                     std::vector<int32_t>::iterator begin,
                     std::vector<int32_t>::iterator end) {
    auto diff = end - begin;
    auto mid = begin + diff / 2;
    if (diff > 2) {
        Sort(yield, begin, mid);
        Sort(yield, mid, end);
    }
    std::vector<int32_t> merge(begin, end);
    std::for_each(begin, end, [](auto &c) {
        CRGB color{static_cast<uint32_t>(c)};
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
                yield(0);
            }
            return;
        }
        if (right == right_end) {
            while (left != left_end) {
                *(begin++) = *(left++);
                yield(0);
            }
            return;
        }
        *(begin++) = (*left <= *right) ? *(left++) : *(right++);
        yield(0);
    }
}

void QuickSort::Sort(Yields_t<int32_t> yield,
                     std::vector<int32_t>::iterator begin,
                     std::vector<int32_t>::iterator end) {

    auto c = end - begin;
    if (c < 2) {
        return;
    }
    auto pivot = end - 1;
    std::swap(*MedianIter(begin + c / 4, begin + c / 2, begin + c * 3 / 4),
              *pivot);
    yield(0);
    auto bound = begin;
    for (auto iter = begin; iter < end - 1; ++iter) {
        if (*iter < *pivot) {
            std::swap(*iter, *(bound++));
        }
        yield(0);
    }
    std::swap(*bound, *pivot);
    yield(0);
    Sort(yield, begin, bound);
    Sort(yield, bound + 1, end);
}
