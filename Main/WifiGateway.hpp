#pragma once

#include <WiFi.h>
#include <functional>

class WifiGateway {
  public:
    WifiGateway(std::function<void()> Callback);
    ~WifiGateway();
    WifiGateway(const WifiGateway &) = delete;
    void Update();

  private:
    WiFiServer wifiServer_;
    std::function<void()> Callback_;
};
