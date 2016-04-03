#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <FastLED.h>
#include <Ticker.h>
#include <HashMap.h>

#define LED_PIN     0
#define NUM_LEDS    35
#define BRIGHTNESS  32
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define REFRESH_RATE 4    // one char every X second
#define LETTER_COLOR CRGB::Cyan

CRGB leds[NUM_LEDS];
ESP8266WebServer server(80);
Ticker renew_display;
const char* ssid = "NixGeht";
const char* password = "allesdoofundso!";
String show_text = "UNIKAT ";
bool show_enabled = true;
int current_char = 0;
// TODO create hash table
#define HASH_SIZE 68
HashType<char, char*> hashRawArray[HASH_SIZE];
HashMap<char, char*> hashMap = HashMap<char, char*>(hashRawArray, HASH_SIZE);

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

   // Initialise HashMap
   hashMap[0]('A', "\
01110\
10001\
10001\
10001\
11111\
10001\
10001\
");
  hashMap[1]('B', "\
11110\
10001\
10001\
11110\
10001\
10001\
11110\
");
  hashMap[2]('C', "\
01110\
10001\
10000\
10000\
10000\
10001\
01110\
");
  hashMap[3]('D', "\
11110\
10001\
10001\
10001\
10001\
10001\
11110\
");
  hashMap[4]('E', "\
11111\
10000\
10000\
11110\
10000\
10000\
11111\
");
  hashMap[5]('F', "\
11111\
10000\
10000\
11110\
10000\
10000\
10000\
");
  hashMap[6]('G', "\
01110\
10001\
10000\
10011\
10001\
10001\
01111\
");
  hashMap[7]('H', "\
10001\
10001\
10001\
11111\
10001\
10001\
10001\
");
  hashMap[8]('I', "\
01110\
00100\
00100\
00100\
00100\
00100\
01110\
");
  hashMap[9]('J', "\
00111\
00010\
00010\
00010\
00010\
10010\
01100\
");
  hashMap[10]('K', "\
10001\
10010\
10100\
11000\
10100\
10010\
10001\
");
hashMap[11]('L', "\
10000\
10000\
10000\
10000\
10000\
10000\
11111\
");
hashMap[12]('M', "\
10001\
11011\
10101\
10101\
10001\
10001\
10001\
");
hashMap[13]('N', "\
10001\
10001\
11001\
10101\
10011\
10001\
10001\
");
hashMap[14]('O', "\
01110\
10001\
10001\
10001\
10001\
10001\
01110\
");
hashMap[15]('P', "\
11110\
10001\
10001\
11110\
10000\
10000\
10000\
");
hashMap[16]('Q', "\
01110\
10001\
10001\
10001\
10101\
10010\
01101\
");
hashMap[17]('R', "\
11110\
10001\
10001\
11110\
10100\
10010\
10001\
");
hashMap[18]('S', "\
01110\
10001\
10000\
01110\
00001\
10001\
01110\
");
hashMap[19]('T', "\
11111\
00100\
00100\
00100\
00100\
00100\
00100\
");
hashMap[20]('U', "\
10001\
10001\
10001\
10001\
10001\
10001\
01110\
");
hashMap[21]('V', "\
10001\
10001\
10001\
10001\
10001\
01010\
00100\
");
hashMap[22]('W', "\
10001\
10001\
10001\
10101\
10101\
10101\
01010\
");
hashMap[23]('X', "\
10001\
10001\
01010\
00100\
01010\
10001\
10001\
");
hashMap[24]('Y', "\
10001\
10001\
10001\
01010\
00100\
00100\
00100\
");
hashMap[25]('Z', "\
11111\
00001\
00010\
00100\
01000\
10000\
11111\
");
hashMap[26](' ', "\
00000\
00000\
00000\
00000\
00000\
00000\
00000\
");
}

void printNextChar() {
  if(show_enabled) {
    Serial.println(show_text[current_char]);
    // TODO call display_char
    display_char(show_text[current_char]);
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
* draw next image
*/
void display_char(char current_char) {
  if(show_enabled) {
    // get values from hash table
    if(hashMap.getValueOf(current_char)) {
      char* data = hashMap.getValueOf(current_char);
      FastLED.clearData();
      for(int i = 0; i < NUM_LEDS; i++) {
        if(data[i] == '1') {

          leds[i] = LETTER_COLOR;
        }
      }
      FastLED.show();
    } else {
      Serial.println("Data not available.");
    }
  }
}

void loop() {
  server.handleClient();
}
