// Светодиодная лента
#define DATA_PIN_0 25
#define CLOCK_PIN_0 26
#define NUM_LEDS 2  // Adjust this based on the number of LEDs in your strip

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "...";
char pass[] = "...";

#define BLYNK_PRINT Serial

/* Fill in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "..."
#define BLYNK_TEMPLATE_NAME "..."
#define BLYNK_AUTH_TOKEN "..."


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <FastLED.h>

// Глобальные переменные
int red = 0;
int green = 0;
int blyat = 0;
int brightness = 0;

CRGB leds[NUM_LEDS];



void setup() {
  // Debug console
  Serial.begin(9600);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  FastLED.addLeds<P9813, DATA_PIN_0, CLOCK_PIN_0, RGB>(leds, NUM_LEDS);
}

void loop() {
  Blynk.run();
  fill_solid(leds, NUM_LEDS, CRGB(red, green, blyat));
  FastLED.setBrightness(brightness);
  FastLED.show();
  delay(50);  // Adjust the delay based on your preference and the desired speed of the animation
}


BLYNK_WRITE(V0){
  red = param.asInt();
}

BLYNK_WRITE(V1){
  green = param.asInt();
}

BLYNK_WRITE(V2){
  blyat = param.asInt();
}

BLYNK_WRITE(V3){
  brightness = param.asInt();
}

BLYNK_WRITE(V4){
  int buttonState = param.asInt();
  if (buttonState) {
    // Set values to 255 when the button is pressed
    red = 255;
    green = 255;
    blyat = 255;
    brightness = 255;

    // Update sliders
    Blynk.virtualWrite(V0, red);
    Blynk.virtualWrite(V1, green);
    Blynk.virtualWrite(V2, blyat);
    Blynk.virtualWrite(V3, brightness);
  } else {
    // Set values to 255 when the button is pressed
    red = 255;
    green = 255;
    blyat = 255;
    brightness = 0;

    // Update sliders
    Blynk.virtualWrite(V0, red);
    Blynk.virtualWrite(V1, green);
    Blynk.virtualWrite(V2, blyat);
    Blynk.virtualWrite(V3, brightness);
  }
}
