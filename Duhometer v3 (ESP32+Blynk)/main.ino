// Конфигурация
#define CO2_PIN_TX 16        // Пин датчика CO2 (TX пин от датчика)
#define CO2_PIN_RX 17        // Пин датчика CO2 (RX пин от датчика)
// Данные подключения Blynk
#define BLYNK_TEMPLATE_ID "..."
#define BLYNK_TEMPLATE_NAME "..."
#define BLYNK_AUTH_TOKEN "..."

#include <BlynkSimpleEsp32.h>
#include <SoftwareSerial.h>
#include <WiFi.h>
#include <WiFiClient.h>

// Данные подключения Wi-fi
char ssid[] = "...";
char pass[] = "...";

// Служебные
#define BLYNK_PRINT Serial
// Датчик CO2 MH-Z19
SoftwareSerial serialCO2(CO2_PIN_TX, CO2_PIN_RX);
byte cmd[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
char response[9]; // здесь будет ответ


// Глобальные переменные, используемые в коде
int ppm = 0;               // Значение CO2
bool error = 0;            // Флаг ошибки

void setup() {
  Serial.begin(9600); // это наш монитор
  Serial.println("Duhometer ver. 0.3, build 002");
  serialCO2.begin(9600); // а это датчик CO2
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  Blynk.run();

  // Запрос показаний с датчиков
  askPPM();             // CO2
  Serial.println("CO2: " + String(ppm) + " ppm");

  Blynk.virtualWrite(V0, ppm);
  delay(1000);  // Adjust the delay based on your preference and the desired speed of the animation
}

void askPPM()
{
  serialCO2.write(cmd, 9);
  serialCO2.readBytes(response, 9);
  int responseHigh = (byte)response[2];
  int responseLow = (byte)response[3];
  ppm = (256 * responseHigh) + responseLow;
}
