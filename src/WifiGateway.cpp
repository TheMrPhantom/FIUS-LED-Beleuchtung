#include "WifiGateway.hpp"

static const char *const kWifiSsid = "Chroma";
static const char *const kWifiPassword = "partyraum";

Request::Request(WiFiClient client) : client_{std::move(client)} {
    bool newline = true;
    while (client_.available()) {
        char c = client_.read();
        if (c == '\r') {
            continue;
        }
        bool emptyline = newline;
        newline = (c == '\n');
        if (emptyline && newline) {
            break;
        }
        message_ += c;
    }
    if (!*this) {
        client_ = WiFiClient{};
    }
}

Request::Request(Request &&other) noexcept
    : client_{std::move(other.client_)}, message_{std::move(other.message_)} {
    other.client_ = WiFiClient{};
}

Request &Request::operator=(const Request &other) {
    auto copy{other};
    swap(*this, copy);
    return *this;
}

Request &Request::operator=(Request &&other) noexcept {
    client_ = std::move(other.client_);
    message_ = std::move(other.message_);
    other.client_ = WiFiClient{};
    return *this;
}

void swap(Request &lhs, Request &rhs) {
    using std::swap;
    swap(lhs.client_, rhs.client_);
    swap(lhs.message_, rhs.message_);
}

Request::operator bool() const noexcept { return !message_.empty(); }

std::experimental::string_view Request::message() const {
    if (!*this) {
        throw std::logic_error{
            "Called Request::message on an empty Request object"};
    }
    return message_;
}

void Request::answer(std::experimental::string_view ans) {
    if (!*this) {
        throw std::logic_error{
            "Called Request::answer on an empty Request object"};
    }
    if (client_.print(ans.data()) != ans.size()) {
        throw std::runtime_error{"Failed to answer request"};
    }
}

WifiGateway::WifiGateway() : wifi_server_{80} {
    WiFi.softAP(kWifiSsid, kWifiPassword);
    wifi_server_.begin();
    Serial.print("WiFi connected. IP address: ");
    Serial.println(WiFi.softAPIP());
}

WifiGateway::~WifiGateway() { wifi_server_.end(); }

Request WifiGateway::NextRequest() { return Request{wifi_server_.available()}; }
