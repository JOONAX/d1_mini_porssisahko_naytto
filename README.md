# D1 mini pörssisähkö näyttö

![kuva](https://github.com/JOONAX/d1_mini_porssisahko_naytto/assets/46499585/6c6b742c-1fc5-4abd-900a-22ef82d76a8b)

## Kuvaus
Tämä näyttää ajankohtaisen pörssisähkön hinnan näytöllä. Projekti käyttää D1 mini IoT-kehitysalustaa, jossa on ESP8266-piiri. Näyttönä toimii 0,96 tuuman SSD1306 OLED-näyttö. Kehitysalusta yhdistetään langattomaan verkkoon ja se hakee pörssisähkön hinnan porssisahko.net API:sta. Hinta päivitetään ensin käynnistettäessä ja sen jälkeen tasatunnein.

## Käytetyt kirjastot
- Wire: https://github.com/esp8266/Arduino/tree/master/libraries/Wire
- Adafruit_GFX: https://github.com/adafruit/Adafruit-GFX-Library
- Adafruit_SSD1306: https://github.com/adafruit/Adafruit_SSD1306
- ESP8266WiFi: https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi
- ESP8266HTTPClient: https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266HTTPClient
- NTPClient: https://github.com/arduino-libraries/NTPClient
- WiFiUdp: https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi
- ArduinoJson: https://github.com/bblanchon/ArduinoJson

## Asennus ja käyttö
1. Asenna Arduino IDE
2. Lisää tarvittavat kirjastot Arduino IDE:een.
3. Liitä kehitysalusta tietokoneeseen ja valitse oikea laite sekä portti Arduino IDE:ssä.
4. Muokkaa Wi-Fi-verkon SSID ja salasana sekä tarvittaessa SHA1-sormenjälki api.porssisahko.net SSL-sertifikaattiin.
5. Lataa koodi kehitysalustaan.
6. Yhdistä näyttö kehitysalustaan

```
Näyttö          Kehitysalusta 
GND      ->     GND
VDD      ->     5V
SCK      ->     D1
SDA      ->     D2
```
