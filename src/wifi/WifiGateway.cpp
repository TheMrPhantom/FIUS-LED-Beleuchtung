#include "WifiGateway.hpp"

static const char *const kWifiSsid = "Chroma";
static const char *const kWifiPassword = "partyraum";

WifiRequestHandle::WifiRequestHandle(WiFiClient client)
    : client_{std::move(client)} {
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

WifiRequestHandle::WifiRequestHandle(WifiRequestHandle &&other) noexcept
    : client_{std::move(other.client_)}, message_{std::move(other.message_)} {
    other.client_ = WiFiClient{};
}

WifiRequestHandle &WifiRequestHandle::
operator=(const WifiRequestHandle &other) {
    auto copy{other};
    swap(*this, copy);
    return *this;
}

WifiRequestHandle &WifiRequestHandle::
operator=(WifiRequestHandle &&other) noexcept {
    client_ = std::move(other.client_);
    message_ = std::move(other.message_);
    other.client_ = WiFiClient{};
    return *this;
}

void swap(WifiRequestHandle &lhs, WifiRequestHandle &rhs) {
    using std::swap;
    swap(lhs.client_, rhs.client_);
    swap(lhs.message_, rhs.message_);
}

WifiRequestHandle::operator bool() const noexcept { return !message_.empty(); }

std::experimental::string_view WifiRequestHandle::Message() const {
    if (!*this) {
        throw std::logic_error{"Called WifiRequestHandle::message on an empty "
                               "WifiRequestHandle object"};
    }
    return message_;
}

void WifiRequestHandle::Answer(std::experimental::string_view ans) {
    if (!*this) {
        throw std::logic_error{"Called WifiRequestHandle::answer on an empty "
                               "WifiRequestHandle object"};
    }
    if (client_.print(ans.data()) != ans.size()) {
        throw std::runtime_error{"Failed to answer WifiRequestHandle"};
    }
}

WifiGateway::WifiGateway() : wifi_server_{80} {
    WiFi.softAP(kWifiSsid, kWifiPassword);
    wifi_server_.begin();
    Serial.print("WiFi connected. IP address: ");
    Serial.println(WiFi.softAPIP());
}

WifiGateway::~WifiGateway() { wifi_server_.end(); }

WifiRequestHandle WifiGateway::NextRequest() {
    return WifiRequestHandle{wifi_server_.available()};
}
