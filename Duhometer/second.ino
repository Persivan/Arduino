// CO2
#include <SoftwareSerial.h>;
SoftwareSerial mySerial(7, 6); // GPIO13 D7  and GPIO15 D8 
// MySerial(RX,TX) - RX-TX платы

byte cmd[9] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79};
char response[9]; // здесь будет ответ


// LED
#include <FastLED.h>
#define NUM_LEDS  16
#define LED_PIN   10
CRGB leds[NUM_LEDS];




void setup() {
delay(1000);
Serial.begin(9600); //это наш монитор
mySerial.begin(9600); //а это датчик MH-Z19(14)
FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
FastLED.setBrightness(100);
delay(1000);
}

void loop()
{
  mySerial.write(cmd,9);//запрос PPM CO2
  mySerial.readBytes(response,9);

  int responseHigh = (byte) response[2];
  int responseLow = (byte) response[3];
int ppm = (256*responseHigh)+responseLow;
// ну и по мануалу из ответа считаем PPM

Serial.println(ppm);


byte color[3] ={0, 0, 0};
  if (ppm < 400) {
    color[0] = 0;
    color[1] = 0;
    color[2] = 255;
  }
  else if (ppm >= 400 && ppm < 800) {
    color[0] = 0;
    color[1] = 255;
    color[2] = 0;
  }
  else if (ppm >= 800 && ppm < 1200) {
    color[0] = 150;
    color[1] = 150;
    color[2] = 0;
  }
  else if (ppm >= 1200 && ppm < 1600) {
    color[0] = 200;
    color[1] = 100;
    color[2] = 0;
  }
  else if (ppm >= 1600 && ppm < 2000) {
    color[0] = 255;
    color[1] = 0;
    color[2] = 0;
  }
  else {
    color[0] = 0;
    color[1] = 0;
    color[2] = 255;
  }
Serial.print(color[0]);
  Serial.print(" ");
  Serial.print(color[1]);
  Serial.print(" ");
  Serial.print(color[2]);
  Serial.println(" ");
// Функция рассчитвающая цвет и количество зажженых светодиодов
for (int i = 0; i < NUM_LEDS; i++) {
  leds[i].r = color[1]; // Красный это зеленый! 
  leds[i].g = color[0]; // ЗЕЛЕНЫЙ ЭТО КРАСНЫЙ!!!
  leds[i].b = color[2]; // синий это синий 
}
FastLED.show();


delay(10000);
}
