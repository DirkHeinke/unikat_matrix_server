#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <FastLED.h>
#include <Ticker.h>
#include <LedTable.h>

#define LED_PIN     0
#define NUM_LEDS    245
#define BRIGHTNESS  180
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define REFRESH_RATE 2    // one char every X second
#define LETTER_COLOR CRGB::Cyan

CRGB leds[NUM_LEDS];
ESP8266WebServer server(80);
Ticker renew_display;
const char* ssid = "UNIKAT";
const char* password = "ardu1n0s";
String show_text = "UNIKAT ";
bool show_enabled = true;
int current_char_num = 0;
ESP8266HTTPUpdateServer httpUpdater;
bool tick = false;
int tickcounter = 0;


void handleRoot();
char getNextChar();
char getCurrentChar();
void display_char(char current_char);
void update();
void tickfun();
void blackout();

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
  httpUpdater.setup(&server);
  server.on("/", handleRoot);
  server.begin();

  // Setup FastLED
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  // Setup Ticker
  renew_display.attach(REFRESH_RATE/10.0, tickfun);

 }

/**
 * Handle requests on /
 */
void handleRoot() {
  Serial.println("Client connected");


  if(server.hasArg("text")) {
      show_text = server.arg("text") + " ";
      current_char_num = 0;
      Serial.println("Text to display renewed:");
      Serial.println(show_text);
  }

  if(server.hasArg("enable")) {
    if(server.arg("enable") == "on") {
      show_enabled = true;
    } else {
      show_enabled = false;
    }
    
  } 



  server.send(200, "text/html", "<form action='/' method='POST'>"
    "<input type='text' name='text' value='" + show_text + "'><br>"
    "On: <input type='radio' name='enable' value='on' checked><br>"
    "Off: <input type='radio' name='enable' value='off'><br>"
    "<input type='submit'></form>");

}

void tickfun() {
  tick = true;
}

void update() {
  char c = getNextChar();

  display_char(c);

  current_char_num++;
  // loop if end of string is reached
  if(current_char_num >= show_text.length()) {
    current_char_num = 0;
  }
}

/**
 * get next char from text to display
 */
char getNextChar() {
  int number = current_char_num + 1;
  if(number >= show_text.length()) {
    number = 0;
  }
  return show_text[number];
}

char getCurrentChar() {
  return show_text[current_char_num];
}

/**
* draw next image
*/
void display_char(char current_char) {
  if(!show_enabled) {
    current_char = ' ';
  }

  // get values from table
  const char* data = lookupChar(current_char);
  if(!data) {
    Serial.println("Data not available. No update performed.");
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

    // cycle through all pixel 0, 7, 14, 21, 28, 1, 8, ...
    pixel = pixel + 7;
    if(pixel > 34) {
      pixel = pixel - 34;
    }
  }

  FastLED.show();
  FastLED.show();
}

void blackout() {
  FastLED.clearData();
  FastLED.show();
  FastLED.show(); 
  Serial.println("Black");
}

/**
 * Main Loop
 */
void loop() {
  server.handleClient();

  if(tick) {
    tickcounter++;

    if(tickcounter == 9) {
      Serial.println("Tick");
      Serial.println(getCurrentChar());
      Serial.println(getNextChar());
      if (getNextChar() == getCurrentChar()) {  
        
        blackout();
      blackout(); 
      }
    }
    if(tickcounter == 10) {
      update();
    }
    

    tick = false;
    tickcounter = tickcounter % 10;
  }
}
