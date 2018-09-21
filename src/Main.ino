#include "Core.hpp"
#include "wifi/WifiServerAdapter.hpp"

#include <experimental/optional>

std::experimental::optional<WifiServerAdapter> wifi_server_adapter;
std::experimental::optional<Core> core;

void setup() {
    Serial.begin(115200); // Init serial output
    core.emplace();
    wifi_server_adapter.emplace(core->Server());
}

void loop() {
    wifi_server_adapter->Update();
    core->Update();
}
