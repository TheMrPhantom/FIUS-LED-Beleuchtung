#pragma once

#include <WiFi.h>

#include <experimental/optional>
#include <functional>
#include <queue>
#include <string>

class Request {
  public:
    explicit Request(WiFiClient client);
    Request(const Request &) = default;
    Request(Request &&other) noexcept;
    Request &operator=(const Request &other);
    Request &operator=(Request &&other) noexcept;
    friend void swap(Request &lhs, Request &rhs);
    explicit operator bool() const noexcept;
    const char *message() const noexcept;
    void answer(const char *ans) noexcept;

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
