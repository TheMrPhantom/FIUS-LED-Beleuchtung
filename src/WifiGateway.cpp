#include "WifiGateway.hpp"

static const char *const kWifiSsid = "Chroma";
static const char *const kWifiPassword = "partyraum";

Request::Request(WiFiClient client) : client_(client) {
    if (!client_) {
        return;
    }
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
    Serial.println("New connection.");
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

Request::operator bool() const noexcept { return static_cast<bool>(client_); }

const char *Request::message() const noexcept { return message_.c_str(); }

void Request::answer(const char *ans) noexcept {
    client_.print(ans);
    Serial.println("Answered request.");
}

WifiGateway::WifiGateway() : wifi_server_{80} {
    WiFi.softAP(kWifiSsid, kWifiPassword);
    wifi_server_.begin();
    Serial.print("WiFi connected. IP address: ");
    Serial.println(WiFi.softAPIP());
}

WifiGateway::~WifiGateway() { wifi_server_.end(); }

Request WifiGateway::NextRequest() { return Request{wifi_server_.available()}; }
