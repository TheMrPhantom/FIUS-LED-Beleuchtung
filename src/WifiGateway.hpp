#pragma once

#include <WiFi.h>

#include <experimental/optional>
#include <experimental/string_view>
#include <functional>
#include <queue>
#include <string>

class WifiRequestHandle {
  public:
    WifiRequestHandle() noexcept = default;
    explicit WifiRequestHandle(WiFiClient client);
    WifiRequestHandle(const WifiRequestHandle &) = default;
    WifiRequestHandle(WifiRequestHandle &&other) noexcept;
    WifiRequestHandle &operator=(const WifiRequestHandle &other);
    WifiRequestHandle &operator=(WifiRequestHandle &&other) noexcept;
    friend void swap(WifiRequestHandle &lhs, WifiRequestHandle &rhs);
    explicit operator bool() const noexcept;
    std::experimental::string_view Message() const;
    void Answer(std::experimental::string_view ans);

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
    WifiRequestHandle NextRequest();

  private:
    WiFiServer wifi_server_;
};
