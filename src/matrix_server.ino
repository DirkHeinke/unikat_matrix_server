#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <FastLED.h>
#include <Ticker.h>

#define LED_PIN     0
#define NUM_LEDS    1
#define BRIGHTNESS  64
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define REFRESH_RATE 1    // one char every X second

CRGB leds[NUM_LEDS];
ESP8266WebServer server(80);
Ticker renew_display;
const char* ssid = "NixGeht";
const char* password = "allesdoofundso!";
String show_text;
bool show_enabled = true;
int current_char = 0;
// TODO create hash table
// characters[];
// bool[] A= "
// 01110
// 10001
// 10001
// 10001
// 11111
// 10001
// 10001"

void setup() {
  // Setup serial
  Serial.begin(115200);
  // start WiFi
  WiFi.begin(ssid, password);
  Serial.println("");
   // Wait for connection
   while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
   }
   Serial.println("");
   Serial.print("Connected to ");
   Serial.println(ssid);
   Serial.print("IP address: ");
   Serial.println(WiFi.localIP());

   // Setup Server
   server.on("/", handleRoot);
   server.begin();

   // Setup FastLed
   FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
   FastLED.setBrightness(BRIGHTNESS);

   // Setup Ticker
   renew_display.attach(REFRESH_RATE, printNextChar);
}

void printNextChar() {
  if(show_enabled) {
    Serial.println(show_text[current_char]);
    // TODO call display_char
    current_char++;
    if(current_char >= show_text.length()) {
      current_char = 0;
    }
  }
}

void handleRoot() {
  Serial.println("Client connected");
  if(server.hasArg("text")) {
    show_text = server.arg("text") + " ";
    current_char = 0;
    Serial.println("Text to display renewed:");
    Serial.println(show_text);
  }
  // TODO currently showing and on/off
  server.send(200, "text/html", "<form action='/' method='POST'><input type='text' name='text'><input type='submit'></form>");
}

/*
* draws next image
*/
void display_char(char current_char) {
  // get values from hash table

  //leds[0] = CRGB::White;
  FastLED.show();
}

void loop() {
  server.handleClient();
}
