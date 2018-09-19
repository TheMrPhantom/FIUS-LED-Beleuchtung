#include "WifiServerAdapter.hpp"

#include <iostream>

WifiServerAdapter::WifiServerAdapter(WebServer &web_server)
    : web_server_(web_server) {}

void WifiServerAdapter::Update() {
    if (auto req = wifi_gateway_.NextRequest()) {
        try {
            auto message = req.Message();
            std::cout << "New request: " << message << '\n';
            req.Answer(web_server_.Process(message));
            std::cout << "Answered request.\n";
        } catch (std::exception &e) {
            std::cerr << '\n' << e.what() << "\n\n";
        }
    }
}
