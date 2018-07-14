#define TOUCH_PIN T2 // connected to 2

#include "LedController.hpp"
#include "Util.hpp"
#include "WifiGateway.hpp"

std::unique_ptr<LedController> ledController;
std::unique_ptr<WifiGateway> wifiGateway;

void setup() {
    Serial.begin(115200);
    ledController = make_unique<LedController>();
    wifiGateway = make_unique<WifiGateway>(*ledController);
}

void loop() {
    wifiGateway->Update();
    ledController->Update();
    delay(20);
}
