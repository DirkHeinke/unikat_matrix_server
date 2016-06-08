#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <FastLED.h>
#include <Ticker.h>
#include <LedTable.h>

#define LED_PIN     0
#define NUM_LEDS    245
// #define NUM_LEDS    35
#define BRIGHTNESS  32
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define REFRESH_RATE 4    // one char every X second
#define LETTER_COLOR CRGB::Cyan

CRGB leds[NUM_LEDS];
ESP8266WebServer server(80);
Ticker renew_display;
const char* ssid = "UNIKAT";
const char* password = "ardu1n0s";
String show_text = "UNIKAT ";
bool show_enabled = true;
int current_char = 0;

void handleRoot();
void getNextChar();
void display_char(char current_char);

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
   renew_display.attach(REFRESH_RATE, getNextChar);

 }

/**
 * Handle requests on /
 */
void handleRoot() {
  Serial.println("Client connected");

  // TODO currently showing and on/off button, speed?
  server.send(200, "text/html", "<form action='/' method='POST'><input type='text' name='text'><input type='submit'></form>");

  if(!server.hasArg("text")) {
    return;
  }

  show_text = server.arg("text") + " ";
  current_char = 0;
  Serial.println("Text to display renewed:");
  Serial.println(show_text);
}

/**
 * get next char from text to display
 */
void getNextChar() {
  if(!show_enabled) {
    return;
  }
  Serial.println(show_text[current_char]);
  display_char(show_text[current_char]);

  current_char++;
  // loop if end of string is reached
  if(current_char >= show_text.length()) {
    current_char = 0;
  }
}

/**
* draw next image
*/
void display_char(char current_char) {
  if(!show_enabled) {
    return;
  }

  // get values from hash table
  const char* data = lookupChar(current_char);
  if(!data) {
    Serial.println("Data not available.");
    return;
  }

  // show data
  int pixel = 0;
  FastLED.clearData();
  for(int i = 0; i < 35; i++) {

    if(data[i] == '1') {
      leds[pixel*7] = LETTER_COLOR;
      leds[pixel*7+1] = LETTER_COLOR;
      leds[pixel*7+2] = LETTER_COLOR;
      leds[pixel*7+3] = LETTER_COLOR;
      leds[pixel*7+4] = LETTER_COLOR;
      leds[pixel*7+5] = LETTER_COLOR;
      leds[pixel*7+6] = LETTER_COLOR;
    }
    pixel = pixel + 7;
    if(pixel > 34) {
      pixel = pixel - 34;
    }
  }
  FastLED.show();
}

/**
 * Main Loop
 */
void loop() {
  server.handleClient();
}
