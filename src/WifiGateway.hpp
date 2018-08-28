#pragma once

#include <WiFi.h>

#include <experimental/optional>
#include <experimental/string_view>
#include <functional>
#include <queue>
#include <string>

class Request {
  public:
    Request() noexcept = default;
    explicit Request(WiFiClient client);
    Request(const Request &) = default;
    Request(Request &&other) noexcept;
    Request &operator=(const Request &other);
    Request &operator=(Request &&other) noexcept;
    friend void swap(Request &lhs, Request &rhs);
    explicit operator bool() const noexcept;
    std::experimental::string_view message() const noexcept;
    void answer(std::experimental::string_view ans) noexcept;

  private:
    mutable WiFiClient client_;
    std::string message_;
};

class WifiGateway {
  public:
    WifiGateway();
    ~WifiGateway();
    WifiGateway(const WifiGateway &) = delete;
    WifiGateway(WifiGateway &&) = default;
    Request NextRequest();

  private:
    WiFiServer wifi_server_;
};
