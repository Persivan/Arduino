#define RX_PIN 13                                          // Rx pin which the MHZ19 Tx pin is attached to
#define TX_PIN 15                                          // Tx pin which the MHZ19 Rx pin is attached to
#define BAUDRATE 9600                                      // Device to MH-Z19 Serial baudrate (should not be changed)

// Web app
const char* ssid = "...";
const char* password = "...";

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "MHZ19.h"


ESP8266WebServer server(28900);

// MH-Z
int CO2;
int8_t Temp;
MHZ19 myMHZ19;                                             // Constructor for library
#if defined(ESP32)
HardwareSerial mySerial(2);                                // On ESP32 we do not require the SoftwareSerial library, since we have 2 USARTS available
#else
#include <SoftwareSerial.h>                                //  Remove if using HardwareSerial or non-uno compatible device
SoftwareSerial mySerial(RX_PIN, TX_PIN);                   // (Uno example) create device to MH-Z19 serial
#endif

unsigned long getDataTimer = 0;


void setup() {
  // Web
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("WiFi connected");
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);

  server.begin();
  Serial.println("Server started");

  // MH-Z
  mySerial.begin(BAUDRATE);                               // (Uno example) device to MH-Z19 serial start
  myMHZ19.begin(mySerial);                                // *Serial(Stream) reference must be passed to library begin().

  myMHZ19.autoCalibration();                              // Turn auto calibration ON (OFF autoCalibration(false))
}

void loop() {
  server.handleClient();
  if (millis() - getDataTimer >= 5000) {
    getCO2andTemp();
    getDataTimer = millis();
  }
}

void handleRoot() {
  server.send(200, "text/plain", createResponse());
}

void getCO2andTemp() {
  CO2 = myMHZ19.getCO2();
  Temp = myMHZ19.getTemperature();
  String debug = String(CO2) + ", " + String(Temp);
  Serial.println();
}

//Вернуть красивую строку для вывода
String createResponse() {
  String response = "Duhometer 1.1 [";
  response += String(CO2);
  response += ", ";
  response += String(Temp);
  response += "] - [CO2 (ppm), Temperature (C)]";
  return response;
}
