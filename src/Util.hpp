#pragma once

#include <memory>
#include <random>

template <class T> T Gcd(T a, T b) {
    while (b != 0) {
        T t = a % b;
        a = b;
        b = t;
    }
    return a;
}

template <class T> T RandomInt(T min, T max) {
    static std::mt19937 rng{std::random_device{}()};
    return std::uniform_int_distribution<T>{min, max}(rng);
}
