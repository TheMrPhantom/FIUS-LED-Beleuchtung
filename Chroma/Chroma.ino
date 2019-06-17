#include <FastLED.h>
#include <WiFi.h>
#include "WLAN_CONF.hpp"
String debugInfo;
typedef void (*StateMethods) ();

void TurnOffStateInitialize(void);
void TurnOffStateUpdate(void);

void ColorStateInitialize(void);
void  ColorStateUpdate(void);

void  MeteorStateInitialize(void);
void  MeteorStateUpdate(void);

void  RainbowStateInitialize(void);
void   RainbowStateUpdate(void);

void  SmoothLightStateInitialize(void);
void  SmoothLightStateUpdate(void);

void  ChristmasStateInitialize(void);
void  ChristmasStateUpdate(void);

void SparkInitialize(void);
void SparkUpdate(void);

void RainbowTrailInitialize(void);
void RainbowTrailUpdate(void);

void SparkTrailInitialize(void);
void SparkTrailUpdate(void);

void TurnOnStateInitialize(void);
void TurnOnStateUpdate(void);

StateMethods stateMethods[] = {
  TurnOffStateInitialize,
  TurnOffStateUpdate,

  ColorStateInitialize,
  ColorStateUpdate,

  MeteorStateInitialize,
  MeteorStateUpdate,

  RainbowStateInitialize,
  RainbowStateUpdate,

  SmoothLightStateInitialize,
  SmoothLightStateUpdate,

  ChristmasStateInitialize,
  ChristmasStateUpdate,

  SparkInitialize,
  SparkUpdate,

  RainbowTrailInitialize,
  RainbowTrailUpdate,

  SparkTrailInitialize,
  SparkTrailUpdate,

  TurnOnStateInitialize,
  TurnOnStateUpdate
};

struct HttpResponse {
  String httpCode;
  String html;
};

const int NUM_LEDS = 900;
const int ENDPOINT_COUNT = 5;
const int STATE_COUNT = 10;

const int DATA_PIN = 4;

CRGB leds[NUM_LEDS];
String endpoints[ENDPOINT_COUNT];

int status = WL_IDLE_STATUS;

WiFiServer server(80);

//Status variables
bool isActive = true;
CRGB color = CRGB(0, 50, 180);
int animationType = 8;

bool onColorChanged = false;
bool shouldInitialize = true;
//End variables


void setup() {

  /*Defining endpoints*/
  endpoints[0] = "color";
  endpoints[1] = "animationType";
  endpoints[2] = "get";
  endpoints[3] = "doorOpen";
  endpoints[4] = "doorClosed";

  /*Initializing LEDs and serial port*/

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  Serial.begin(9600);
  FastLED.show();

  /*Connecting to Wifi or starting accesspoint*/
  setupWlan();

}

void loop() {
  refreshPage();
  refreshLED();
}

void setProgress(int character, int startIndex, int color) {

  CHSV hsv( color, 255, 100);
  CRGB rgb;
  hsv2rgb_rainbow( hsv, rgb);

  leds[startIndex] = rgb;

  FastLED.show();
}

void setupWlan() {

  /*Trying to connect to wifi*/
  for (int i = 0; i < 5 && status != WL_CONNECTED; i++) {
    Serial.print("Trying to connect to SSID: ");
    Serial.println(ssid);

    status = WiFi.begin(ssid, pass);

    Serial.println(status);
    Serial.println(WiFi.localIP());

    int startindex = 0;
    setProgress('o', startindex + i, 100);


    delay(5000);
  }

  /*Clear leds*/
  for (int j = 0; j < NUM_LEDS; j++) {
    leds[j] = CRGB::Black;
  }
  FastLED.show();

  /*If could'nt connect to wifi then open acesspoint*/
  if (status != WL_CONNECTED) {
    WiFi.softAP("chroma", "PeterDerWolf");
    Serial.println("A");
    setProgress('A', 0, CRGB::Green);
  } else {
    Serial.println("W");
    setProgress('W', 0, CRGB::Yellow);
  }
  Serial.println("started");
  /*Start webserver*/
  server.begin();
  Serial.println("started2");
  delay(5000);
}

void refreshPage() {

  WiFiClient client = server.available();

  bool check = false;
  HttpResponse resp;
  if (client) {
    Serial.println("dd");
    /*Client connected to webserver*/
    String parsingString = "";


    bool currentLineIsBlank = true;
    HttpResponse httpResponse;
    httpResponse.httpCode = "HTTP/1.1 405 METHOD NOT ALLOWED";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        parsingString += c;
        int startIndex = parsingString.indexOf("POST");


        if (parsingString.lastIndexOf("HTTP/1.1") != -1 && !check) {
          /*We received the whole url*/
          check = true;

          int endIndex = parsingString.lastIndexOf("HTTP/1.1");
          parsingString = parsingString.substring(startIndex, endIndex);
          parsingString = parsingString.substring(7, parsingString.length() - 1);
          if (parsingString.indexOf("favicon.ico") == -1) {
            httpResponse = reactOnHTTPCall(parsingString);

          }

        }

        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header

          client.println(httpResponse.httpCode);
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println();

          client.println(httpResponse.html);

          break;
        }
        if (c == '\n') {
          currentLineIsBlank = true;
        } else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }

    delay(1);

    client.stop();

  }
}

HttpResponse reactOnHTTPCall(String message) {
  //debugInfo += message + "\n";
  String temp = "";
  String output = "HTTP/1.1 200 OK";
  int match = -1;
  String html = "";
  //debugInfo += message + "\n";
  /*Finding out what endpoint is called*/
  for (int i = 0; i < ENDPOINT_COUNT; i++) {
    if (message.startsWith(endpoints[i])) {
      temp = message.substring(endpoints[i].length() + 1);
      Serial.println(temp);
      match = i;
    }
  }
  //debugInfo += temp + "\n";
  /*Replacing http substituted character*/
  temp.replace("%20", " ");
  //debugInfo += temp + "\n";

  /*Parsing the endpoint info*/
  if (match == 0) {
    int r = temp.substring(0, 3).toInt();
    int g = temp.substring(3, 6).toInt();
    int b = temp.substring(6, 9).toInt();

    color = CRGB(r, g, b);
    onColorChanged = true;

  } else if (match == 1) {
    animationType = temp.toInt();
    animationType %= STATE_COUNT;
    shouldInitialize = true;
  }  else if (match == 2) {

    int r = color.r;
    int g = color.g;
    int b = color.b;

    String colorOUT = "RGB(" + String(r) + "," + String(g) + "," + String(b) + ")";

    html = "{\"isActive\":" + String(isActive);
    html += ",\"animationType\":" + String(animationType) ;
    html += ",\"color\":\"" + colorOUT + "\"}";


  } else if (match == 3) {
    animationType = STATE_COUNT - 1;
    shouldInitialize = true;

  } else if (match == 4) {
    animationType = 0;
    shouldInitialize = true;

  }
  if (match == -1) {
    output = "HTTP/1.1 404 NO ENDPOINT";
    html = message;
  }
  HttpResponse resp;
  resp.httpCode = output;
  resp.html = "Endpoint tried: "+html;

  return resp;

}

CRGB rainbowColor(int i) {
  i %= 256;
  CHSV hsv(i, 255, 255);
  CRGB rgb;
  hsv2rgb_rainbow( hsv, rgb); return rgb;
}

CRGB rainbowColor(int i, int brightness) {
  i %= 256;
  CHSV hsv(i, 255, brightness);
  CRGB rgb;
  hsv2rgb_rainbow( hsv, rgb); return rgb;
}
CRGB rainbowColor(int i, int saturation, int brightness) {
  i %= 256;
  CHSV hsv(i, saturation, brightness);
  CRGB rgb;
  hsv2rgb_rainbow( hsv, rgb); return rgb;
}
void clearLED() {
  for (int i = 0; i < NUM_LEDS; i++) {
    CRGB temp = leds[i];
    leds[i] = CRGB(0, 0, 0);
  }
}
void fadeAll() {
  for (int i = NUM_LEDS - 1; i >= 0; i--) {
    leds[i].fadeToBlackBy(4);
  }
}
