#pragma once

#include "LedController.hpp"

#include <WiFi.h>

class WifiGateway {
  public:
    WifiGateway(LedController &ledController);
    ~WifiGateway();
    WifiGateway(const WifiGateway &) = delete;
    void Update();

  private:
    WiFiServer wifiServer_;
    LedController &ledController_;
};
