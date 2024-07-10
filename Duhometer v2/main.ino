#include <SoftwareSerial.h>
#include <FastLED.h>
#include "DHT.h"
#define _LCD_TYPE 1 // Кринж, но эту строчку придется оставить тут
#include "LCD_1602_RUS_ALL.h"

// Конфигурация
#define NUM_LEDS 3          // Количество ARGB светодиодов на пине
#define LED_PIN 10          // Пин ARGB ленты
#define LED_BRIGHTNESS 100  // Яркость светодиодов 0-255
#define CO2_PIN_TX 7        // Пин датчика CO2 (TX пин от датчика)
#define CO2_PIN_RX 6        // Пин датчика CO2 (RX пин от датчика)
#define TEMP_AND_HUMI_PIN 9 // Пин датчика температуры и влажности

// Служебные глобальные переменные
// Датчик CO2 MH-Z19
SoftwareSerial serialCO2(CO2_PIN_TX, CO2_PIN_RX);
byte cmd[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
char response[9]; // здесь будет ответ
// ARGB светодиодная лента WS2812
CRGB leds[NUM_LEDS];
// Температура и влажность DHT22
DHT dht(TEMP_AND_HUMI_PIN, DHT22);
// Дисплей lcm1602 на i2c
LCD_1602_RUS lcd(0x27, 16, 2);

// Глобальные переменные, используемые в коде
byte color[3] = {0, 0, 0}; // Цвет ARGB ленты
int ppm = 0;               // Значение CO2
float temp = 0;            // Значение температуры
float humidity = 0;        // Значение влажности
bool error = 0;            // Флаг ошибки

void setup()
{
  Serial.begin(9600); // это наш монитор
  Serial.println("Duhometer ver. 0.2, build 006");
  serialCO2.begin(9600); // а это датчик CO2
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(LED_BRIGHTNESS);
  dht.begin();
  lcd.init();
  lcd.backlight();
}

void loop()
{
  // Запрос показаний с датчиков
  askPPM();             // CO2
  askTempAndhumidity(); // Температура и влажность

  // Преобразование значений
  ppmToColor(); // Конвертим значение с датчика CO2 в цвет

  // Установка показаний на выводы
  setColor();       // Установка цвета ленты
  setDisplayText(); // Установка значений на дисплей
  Serial.println("{" +
                 String("ppm: ") + padStringToLength(String(ppm), 4) + ", " +
                 String("red: ") + String(color[0]) + ", " +
                 String("green: ") + String(color[1]) + ", " +
                 String("blue: ") + String(color[2]) + ", " +
                 String("temp: ") + padStringToLength(String(temp), 4) + ", " +
                 String("humidity: ") + padStringToLength(String(humidity), 4) +
                 "}");

  delay(10000); // Задержка 10 секунд
}


void ppmToColor()
{
  if (ppm >= 400 && ppm < 800) // Зеленый, хорошо
  {
    color[0] = 0;
    color[1] = 255;
    color[2] = 0;
  }
  else if (ppm >= 800 && ppm < 1200) // Желтый, норм
  {
    color[0] = 150;
    color[1] = 150;
    color[2] = 0;
  }
  else if (ppm >= 1200 && ppm < 1600) // Оранжевый, норм
  {
    color[0] = 200;
    color[1] = 100;
    color[2] = 0;
  }
  else if (ppm >= 1600) // Красный, пллохо
  {
    color[0] = 255;
    color[1] = 0;
    color[2] = 0;
  }
  else
    error = 1; // Синий, что-то пошло не так
}


void askPPM()
{
  serialCO2.write(cmd, 9);
  serialCO2.readBytes(response, 9);
  int responseHigh = (byte)response[2];
  int responseLow = (byte)response[3];
  ppm = (256 * responseHigh) + responseLow;
}


void askTempAndhumidity()
{
  temp = dht.readTemperature();
  humidity = dht.readHumidity();
  if (isnan(temp) || isnan(humidity))
  {
    error = 1;
    Serial.println(F("Failed to read from DHT sensor!"));
  }
}


void setDisplayText()
{
  lcd.setCursor(0, 0);
  lcd.print(padStringToLength(String(temp), 4) + String(" °C    ") + padStringToLength(String(humidity), 4) + "%");
  lcd.setCursor(0, 1);
  lcd.print(padStringToLength(String(ppm), 4) + " ppm  " + getStatus());
}


void setColor()
{
  if (error) // Синий, если что-то пошло не так
  {
    color[0] = 0;
    color[1] = 0;
    color[2] = 255;
  }
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i].r = color[1]; // Красный это зеленый!
    leds[i].g = color[0]; // ЗЕЛЕНЫЙ ЭТО КРАСНЫЙ!!!
    leds[i].b = color[2]; // синий это синий
  }
  FastLED.show();
}


// Подгоняет строку под заданный размер (не работает с кириллицей)
// Пример 1: "abc", 5 => "  abc"
// Пример 2: "abcdefg", 5 => "abcde"
String padStringToLength(String input, byte length)
{
  String output = "";
  if (input.length() >= length)
  {
    for (int i = 0; i < length; i++)
      output += input[i];
  }
  else
  {
    output = input;
    for (int i = input.length(); i < length; i++)
      output = " " + output;
  }
  return output;
}


// Все, что более 6 символов не будет умещаться на экране
String getStatus()
{
  if (ppm > 1600) return "Духота";
  if (ppm > 1200) return " Душно";
  if (temp > 26) return " Жарко";
  if (temp < 22) return "Холодно";
  if (humidity > 60) return "Влажно";
  if (humidity < 30) return "  Сухо";
  if (ppm > 800) return "  Норм";
  return "Хорошо";
}
