#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#define TOUCH_PIN T2 //connected to 2
#define PIN 4

const char* ssid     = "IchBinDerFiusRaum";
const char* password = "^2T941o2";
WiFiServer server(80);
const int programmMaxIndex = 3;

int pgrNr = 0;

int r1 = 0;
int r2 = 0;

int g1 = 0;
int g2 = 0;

int b1 = 0;
int b2 = 0;

int divide = 50;

long t = 0;
boolean statusChanged;


Adafruit_NeoPixel strip = Adafruit_NeoPixel(1055, PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  statusChanged = true;
  pgrNr = 3;
  Serial.begin(115200);
  strip.begin();
  refreshLED(); // Initialize all pixels to 'off'
  WiFi.softAP(ssid, password);

  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.softAPIP());
  server.begin();
}

void loop()
{
  checkForWlanClient();
  if (pgrNr == 3 || statusChanged) {
    statusChanged = false;
    Serial.print("Programm Nr:");
    Serial.println(pgrNr);
    switch (pgrNr) {
      case 0:

        for (int i = 0; i < strip.numPixels(); i++) {
          strip.setPixelColor(i, r2, g2, b2);
        }

        for (int i = 0; i < (int)(405 * (divide / 100.0)); i++) {
          strip.setPixelColor(i, r1, g1, b1);
        }

        for (int i = strip.numPixels(); i > strip.numPixels() - (int)(405 * (divide / 100.0)); i--) {
          strip.setPixelColor(i, r2, g2, b2);
        }
        refreshLED();
        break;
      case 1:
        for (int i = 0; i < 400; i++) {
          strip.setPixelColor(i, 255, 255, 0);
        }
        for (int i = 192; i < 300; i++) {
          strip.setPixelColor(i, 255, 255, 255);
        }
        refreshLED();
        break;
      case 2:
        for (int i = 0; i < 400; i++) {
          strip.setPixelColor(i, 255, 255, 0);
        }
        for (int i = 20; i < 120; i++) {
          strip.setPixelColor(i, 255, 255, 255);
        }
        refreshLED();
        break;
      case 3:
        rainbowCycle(20);
        break;
    }
  }
}

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  int counter = 0;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    counter++;
    if (counter > 100) {
      if (statusChanged) {
        break;
      }
      counter = 0;
    }
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    refreshLED();
    delay(wait);
    checkForWlanClient();

  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void refreshLED() {
  delay(10);
  portDISABLE_INTERRUPTS();
  delay(10);
  strip.show();
  portENABLE_INTERRUPTS();
}

void checkForWlanClient() {

  WiFiClient client = server.available();   // listen for incoming clients

  bool hasReceivedData = false;
  String data = "";
  if (client) {                             // if you get a client,
    // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then

        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            printWebsite(client);

            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        if (currentLine.endsWith("&last=42") && !hasReceivedData) {
          data = currentLine;


          if (data.startsWith("GET /programm?")) {
            data.replace("GET /programm?programm=", "");
            data.replace("&last=42", "");
            pgrNr = data.toInt();
            hasReceivedData = true;

          } else if (data.startsWith("GET /rgbChoose?")) {
            data.replace("GET /rgbChoose?", "");
            data.replace("&last=42", "");

            r1 = split(data, 0, '&').substring(3).toInt();
            r2 = split(data, 4, '&').substring(3).toInt();

            g1 = split(data, 1, '&').substring(3).toInt();
            g2 = split(data, 5, '&').substring(3).toInt();

            b1 = split(data, 2, '&').substring(3).toInt();
            b2 = split(data, 6, '&').substring(3).toInt();

            divide = split(data, 3, '&').substring(8).toInt();
            hasReceivedData = true;
          } else if (data.startsWith("GET /syncRGB?")) {
            divide = 50;
            r2 = r1;
            g2 = g1;
            b2 = b1;
            hasReceivedData = true;
          }

        }
      }
    }
    // close the connection:
    client.stop();
  }


  if (hasReceivedData) {
    hasReceivedData = false;

    statusChanged = true;

    if (data.startsWith("GET /programm?")) {


    } else if (data.startsWith("GET /rgbChoose?")) {


    } else if (data.startsWith("GET /syncRGB?")) {

    }
  }
}

String split(String data, int index, char character) {

  int commaIndex = data.indexOf(character);
  int secondCommaIndex = data.indexOf(character, commaIndex + 1);
  for (int i = 0; i < index - 1; i++) {

    commaIndex = secondCommaIndex;
    secondCommaIndex = data.indexOf(character, commaIndex + 1);
  }

  if (index == 0) {
    data.remove(commaIndex);
  } else  {
    return data.substring(commaIndex + 1, secondCommaIndex);
  }

  return data;
}









