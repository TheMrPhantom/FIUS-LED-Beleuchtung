#pragma once

#include "WebServer.hpp"
#include "WifiGateway.hpp"

class WifiServerAdapter {
  public:
    WifiServerAdapter(WebServer &web_server);
    void Update();

  private:
    WebServer &web_server_;
    WifiGateway wifi_gateway_;
};