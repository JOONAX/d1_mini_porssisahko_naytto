#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>

// OLED-näytön pinnien määrittely
#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define ADAFRUIT_ASCII96 0  //use the full-fat 255 letters
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Wi-Fi asetukset
const char* ssid = "WLAN";
const char* password = "PASSWORD";

//SHA1 sormenjälki api.porssisahko.net SSL-sertifikaattiin
const char fingerprint[] PROGMEM = "F6 BF 35 1C C0 0E EC 0C AD BB 36 83 A3 A4 B8 98 19 DF 44 E7";

// NTP-palvelimen tiedot
const char* ntpServer = "pool.ntp.org";
const long gmtOffset = 3 * 3600;  // Aikavyöhyke: GMT+3 (3 tuntia)

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpServer, gmtOffset);

// Muuntaa ajan (epoch time) päivämäärän muotoon "YYYY-MM-DD"
String epochToDateString(unsigned long epochTime) {
  time_t t = (time_t)epochTime;
  struct tm* ptm = localtime(&t);
  int year = ptm->tm_year + 1900;
  int month = ptm->tm_mon + 1;
  int day = ptm->tm_mday;

  String dateString = String(year) + "-" + leadingZero(month) + "-" + leadingZero(day);
  return dateString;
}

// Lisää nollan yksinumeroisiin eteen kuukausiin ja päiviin
String leadingZero(int number) {
  if (number < 10) {
    return "0" + String(number);
  } else {
    return String(number);
  }
}

void setup() {

  //Sarjaportin alustus
  Serial.begin(115200);

  // Alusta OLED-näyttö
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  // Yhdistetään Wi-Fi-verkkoon
  Serial.print("Yhdistetään Wi-Fi verkkoon ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Alustetaan NTPClient
  timeClient.begin();
}

// Tällä saadaan toimimaan käynnistyksessä sähkön hinnan tarkistus ja sen jälkeen tasatunnein
bool initialRequestMade = false;

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    // Päivitetään aika
    timeClient.update();

    // Haetaan aika (epoch time)
    unsigned long epochTime = timeClient.getEpochTime();

    // Tarkista, onko tasatunti tai onko käynnistyksessä haettu jo tiedot
    int currentMinute = timeClient.getMinutes();
    if (!initialRequestMade || currentMinute == 0) {
      // Muunnetaan aika päivämääräksi
      String formattedDate = epochToDateString(epochTime);

      // Haetaan nykyinen tunti
      int currentHour = timeClient.getHours();

      //Asetukset pyyntöä varten
      WiFiClientSecure client;
      client.setFingerprint(fingerprint);
      HTTPClient http;

      // Pörssisähkön API osoite + YYYY-MM-DD muodossa päivämäärä + kuluva tunti
      String url = "https://api.porssisahko.net/v1/price.json?date=" + String(formattedDate) + "&hour=" + String(currentHour);
      Serial.println(url);

      // Lähetetään HTTP GET -pyyntö
      http.useHTTP10(true);
      http.begin(client, url);
      http.GET();

      // Parsi vastaus
      DynamicJsonDocument doc(32);
      deserializeJson(doc, http.getStream());

      // Lue arvot
      double price = doc["price"].as<double>();
      Serial.println(doc["price"].as<double>());

      // Näytä hinta OLED-näytöllä
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.println("Hinta nyt:");
      display.println(price, 2);
      display.println("snt/kWh");
      display.setTextSize(1);
      display.println();
      display.println(String(formattedDate) + " tunti: " + String(currentHour));
      display.display();

      // Katkaise yhteys
      http.end();

      // Muutetaan totuusarvomuuttuja falsesta trueksi
      initialRequestMade = true;
    }
  }
  // Odota 1 minuutti ennen seuraavaa tarkistusta
  delay(60000);
}
