#define TOUCH_PIN T2 // connected to 2

#include "LedStrip.hpp"
#include "Util.hpp"
#include "WifiGateway.hpp"

const uint16_t kPixelCount = 1055;
const uint8_t kPin = 4;

std::unique_ptr<LedStrip> ledStrip;
std::unique_ptr<WifiGateway> wifiGateway;

int32_t ticksLeftActive = 0;
int32_t ticksActive = 0;

void setup() {
    Serial.begin(115200);
    auto callback = []() { ticksLeftActive = 150; };
    ledStrip = make_unique<LedStrip>(kPixelCount, kPin);
    wifiGateway = make_unique<WifiGateway>(callback);
}

void UpdateLeds() {
    if (ticksLeftActive > 0) {
        if (ticksActive == 0) {
            RgbColor red{255, 0, 0};
            RgbColor green{0, 255, 0};
            RgbColor blue{0, 0, 255};
            uint16_t s1 = kPixelCount / 3;
            uint16_t s2 = 2 * kPixelCount / 3;
            ledStrip->Gradient(0, s1 - 1, red, green);
            ledStrip->Gradient(s1, s2 - 1, green, blue);
            ledStrip->Gradient(s2, kPixelCount - 1, blue, red);
        } else {
            ledStrip->Rotate(12);
        }
        --ticksLeftActive;
        ++ticksActive;
    } else {
        ticksActive = 0;
        ledStrip->SetColors(RgbColor{100, 100, 100});
    }
}

void loop() {
    wifiGateway->Update();
    UpdateLeds();
    delay(20);
}
