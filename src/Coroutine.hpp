#pragma once

#include "Util.hpp"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <functional>
#include <stdexcept>

template <class T> using Yields = std::function<void(T)>;

template <class T, int32_t kStackSize> class Coroutine {
  public:
    Coroutine() noexcept = default;

    template <class Function, class... Args>
    Coroutine(Function routine, const Args &... args)
        : data_{std::make_unique<Data>()} {
        data_->co_task = [routine = std::move(routine), args..., &data = *data_,
                          my_handle = xTaskGetCurrentTaskHandle()]() {
            ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
            auto yield = [&yielded = data.yielded, my_handle](T val) {
                yielded = std::move(val);
                xTaskNotifyGive(my_handle);
                ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
            };
            routine(std::move(yield), std::move(args)...);
            data.co_handle = nullptr;
            xTaskNotifyGive(my_handle);
            vTaskDelete(nullptr);
        };
        auto proxy = [](void *ptr) { static_cast<Data *>(ptr)->co_task(); };
        if (pdPASS != xTaskCreate(proxy, "", kStackSize, data_.get(), 1,
                                  &data_->co_handle)) {
            throw std::runtime_error{
                "Failed to allocate memory for a Coroutine"};
        }
    }

    ~Coroutine() noexcept {
        if (data_ && data_->co_handle) {
            vTaskDelete(data_->co_handle);
        }
    }

    Coroutine(const Coroutine &) = delete;
    Coroutine(Coroutine &&) noexcept = default;
    Coroutine &operator=(const Coroutine &) = delete;
    Coroutine &operator=(Coroutine &&) noexcept = default;

    explicit operator bool() const { return data_ != nullptr; }

    bool Run() {
        if (!data_ || !data_->co_handle) {
            return false;
        }
        xTaskNotifyGive(data_->co_handle);
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        return data_->co_handle != nullptr;
    }

    const T Get() { return std::move(data_->yielded); }

  private:
    struct Data {
        T yielded;
        std::function<void()> co_task;
        TaskHandle_t co_handle;
    };

    std::unique_ptr<Data> data_;
};
