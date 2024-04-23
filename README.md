# RGB-Controller-on-ESP-with-P9813
esp32, esp8266, P9813, 

# Зависимости
1. Arduino IDE 2.3.2
2. Blynk (у меня не работала установка черезе библиотеку Arduino IDE, поэтому для установки распаковал [релиз](https://github.com/blynkkk/blynk-library/releases), т.е. Blynk, BlynkESP8266_Lib и т.д., в `Документы\Arduino\libraries`)
3. FastLed (через библиотеку Arduino IDE)
4. В менеджер плат нужно указать `https://arduino.esp8266.com/stable/package_esp8266com_index.json` или `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`, потом скачать соответствующие платы
5. Драйвер для загрузчика на плате. Многие платы используют CH340. Скачать и установить его.

Настройки для загрузчика
![image](https://github.com/Persivan/RGB-Controller-on-ESP-with-P9813/assets/39733368/5747c209-c514-479e-80da-bbf8afbc1a3a)

Пример сборки для esp8266
![image](https://github.com/Persivan/RGB-Controller-on-ESP-with-P9813/assets/39733368/9b9a49c1-b00e-4bf1-94eb-7878c2e961b0)
