#pragma once

#include "StateManager.hpp"
#include "WebServer.hpp"

#include <memory>
#include <vector>

class Core {
  public:
    Core();

    WebServer &Server();
    const WebServer &Server() const;

    void Update();

  private:
    StateManager state_manager_;
    WebServer web_server_;
};