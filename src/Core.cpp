#include "Core.hpp"

Core::Core() {
    web_server_.Wip_SetCallback([&]() { state_manager_.NextState(); });
}

WebServer &Core::Server() { return web_server_; }

const WebServer &Core::Server() const { return web_server_; }

void Core::Update() { state_manager_.Update(); }
