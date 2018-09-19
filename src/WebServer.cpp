#include "WebServer.hpp"

#include <fstream>

WebServer::WebServer() {
    SPIFFS.begin();
    std::string filename = "/spiffs/http/index.html";
    std::ifstream file{filename};
    if (!file) {
        throw std::runtime_error("Failed to open file " + filename);
    }
    _answer = "HTTP/1.1 200 OK\n"
              "Content-Type: text/html\n"
              "Connection: close\n"
              "\n";
    _answer.insert(_answer.end(), std::istreambuf_iterator<char>{file},
                   std::istreambuf_iterator<char>{});
}

std::experimental::string_view
WebServer::Process(std::experimental::string_view message) {
    if (_callback) {
        _callback();
    }
    return _answer;
}

void WebServer::Wip_SetCallback(std::function<void()> callback) {
    _callback = callback;
}
