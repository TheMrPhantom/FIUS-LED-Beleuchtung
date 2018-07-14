#include "WifiGateway.hpp"

static const char *const kWifiSsid = "Chroma";
static const char *const kWifiPassword = "partyraum";

WifiGateway::WifiGateway(LedController &ledController)
    : wifiServer_{80}, ledController_{ledController} {
    WiFi.softAP(kWifiSsid, kWifiPassword);
    wifiServer_.begin();
    Serial.print("WiFi connected. IP address: ");
    Serial.println(WiFi.softAPIP());
}

WifiGateway::~WifiGateway() { wifiServer_.end(); }

void WifiGateway::Update() {
    if (WiFiClient client = wifiServer_.available()) {
        Serial.println("New client:");
        String currentLine;
        while (client.available()) {
            char c = client.read();
            if (c == '\r') {
                continue;
            }
            Serial.print(c);
            if (c != '\n') {
                currentLine += c;
                continue;
            }
            if (currentLine.length() != 0) {
                currentLine = "";
                continue;
            }
            ledController_.SetTicksLeftActive(80);
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");
            client.println();
            client.println("Set ticksLeftActive to 150.");
            Serial.println("Answered request.");
        }
        client.stop();
        Serial.println("Connection closed.");
    }
}
