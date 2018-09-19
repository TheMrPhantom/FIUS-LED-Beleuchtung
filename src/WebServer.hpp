#pragma once

#include "SPIFFS.h"
#undef min
#undef max

#include <experimental/string_view>

class WebServer {
  public:
    WebServer();

    std::experimental::string_view
    Process(std::experimental::string_view message);
    void Wip_SetCallback(std::function<void()> callback);

  private:
    std::string _answer;
    std::function<void()> _callback;
};
