# RGB-Controller-on-ESP-with-P9813
Управление 12 вольтовой RGB лентой с помощью Blynk. Микроконтроллер ESP32 или ESP8266, драйвер P9813.

**!Драйвер пищит транзисторами при работе!**

# Зависимости
1. Arduino IDE 2.3.2
2. Blynk (у меня не работала установка через менеджер библиотек Arduino IDE, поэтому для установки распаковал [релиз](https://github.com/blynkkk/blynk-library/releases), т.е. Blynk, BlynkESP8266_Lib и т.д., в `Документы\Arduino\libraries`)
3. FastLed (через менеджер Arduino IDE)
4. В менеджер плат нужно указать `https://arduino.esp8266.com/stable/package_esp8266com_index.json` или `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`, потом скачать соответствующие настройки для плат через менеджер плат в Arduino IDE
5. Драйвер для загрузчика на плате. Многие платы используют CH340. Скачать и установить его.

Настройки для загрузчика

![image](https://github.com/Persivan/RGB-Controller-on-ESP-with-P9813-main/assets/39733368/c5ce921b-8caf-4453-8572-9ef2c0e79f47)

Пример сборки для esp8266

![chrome_DFaIbWTh3C](https://github.com/Persivan/RGB-Controller-on-ESP-with-P9813-main/assets/39733368/eebc576b-c109-482c-870d-52dc4333a7a1)

