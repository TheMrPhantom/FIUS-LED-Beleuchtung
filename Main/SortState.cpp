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

void BubbleSort::Sort(Yields<int32_t> yield,
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

namespace {
void Merge(Yields<int32_t> yield, std::vector<int32_t>::iterator begin,
           std::vector<int32_t>::iterator end) {
    std::vector<int32_t> merge(begin, end);
    std::for_each(begin, end, [](auto &c) {
        CRGB color{static_cast<uint32_t>(c)};
        color /= 8;
        c = (color.r << 16) | (color.g << 8) | color.b;
    });
    auto left = merge.begin();
    auto left_end = merge.begin() + (end - begin) / 2;
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
}; // namespace

void MergeSort::Sort(Yields<int32_t> yield,
                     std::vector<int32_t>::iterator begin,
                     std::vector<int32_t>::iterator end) {
    auto diff = end - begin;
    if (diff > 2) {
        auto mid = begin + diff / 2;
        Sort(yield, begin, mid);
        Sort(yield, mid, end);
    }
    Merge(yield, begin, end);
}

namespace {
void PMergeSortLevel(Yields<int32_t> yield,
                     std::vector<int32_t>::iterator begin,
                     std::vector<int32_t>::iterator end, int32_t depth,
                     bool quick) {
    if (depth == 0) {
        if (quick) {
            QuickSort::Sort(yield, begin, end);
        } else {
            Merge(yield, begin, end);
        }
        return;
    }
    auto diff = end - begin;
    if (diff <= 2) {
        return;
    }
    auto mid = begin + diff / 2;
    Coroutine<int32_t, 2048> left = [=](Yields<int32_t> yield_left) {
        PMergeSortLevel(yield_left, begin, mid, depth - 1, quick);
    };
    Coroutine<int32_t, 2048> right = [=](Yields<int32_t> yield_right) {
        PMergeSortLevel(yield_right, mid, end, depth - 1, quick);
    };
    while (true) {
        bool b1 = left.Run();
        bool b2 = right.Run();
        if (!b1 && !b2)
            break;
        yield(0);
    }
}
} // namespace

void PMergeSort::Sort(Yields<int32_t> yield,
                      std::vector<int32_t>::iterator begin,
                      std::vector<int32_t>::iterator end) {
    // int32_t depth = 0;
    // for (int32_t c = end - begin - 1; c; c >>= 1) {
    //    ++depth;
    //}
    int32_t depth = 5;
    PMergeSortLevel(yield, begin, end, depth, true);
    while (--depth >= 0) {
        PMergeSortLevel(yield, begin, end, depth, false);
    }
}

void QuickSort::Sort(Yields<int32_t> yield,
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
