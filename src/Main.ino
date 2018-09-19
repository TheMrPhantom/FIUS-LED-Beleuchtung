#include "Core.hpp"
#include "WebServer.hpp"
#include "WifiServerAdapter.hpp"

#include <experimental/optional>

std::experimental::optional<WebServer> web_server;
std::experimental::optional<WifiServerAdapter> wifi_server_adapter;
std::experimental::optional<Core> core;

void setup() {
    Serial.begin(115200); // Init serial output
    web_server.emplace();
    wifi_server_adapter.emplace(*web_server);
    core.emplace(*web_server);
}

void loop() {
    wifi_server_adapter->Update();
    core->Update();
}
