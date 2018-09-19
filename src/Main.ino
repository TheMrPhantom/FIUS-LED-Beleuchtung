#include "Core.hpp"
#include "WebServer.hpp"
#include "WifiGateway.hpp"

#include <experimental/optional>
#include <fstream>
#include <iostream>
#include <vector>

std::experimental::optional<WebServer> web_server;
std::experimental::optional<WifiGateway> wifi_gateway;
std::experimental::optional<Core> core;

void HandleWifiRequest() {
    if (auto req = wifi_gateway->NextRequest()) {
        try {
            auto message = req.Message();
            std::cout << "New request: " << message << '\n';
            req.Answer(web_server->Process(message));
            std::cout << "Answered request.\n";
        } catch (std::exception &e) {
            std::cerr << '\n' << e.what() << "\n\n";
        }
    }
}

void setup() {
    Serial.begin(115200); // Init serial output
    web_server.emplace();
    wifi_gateway.emplace();
    core.emplace(*web_server);
}

void loop() {
    HandleWifiRequest();
    core->Update();
}
