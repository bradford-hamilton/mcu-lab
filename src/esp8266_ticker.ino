#include <Wire.h>
#include "SSD1306Wire.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <ArduinoJson.h>

const char* ssid = "ssid";
const char* pass = "pass";

SSD1306Wire display(0x3c, SDA, SCL);

void connect_to_wifi()
{
  Serial.print("Connecting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void initialize_display()
{
  display.init();
  display.setContrast(255);
  display.setColor(WHITE);
  display.flipScreenVertically();
}

void begin_serial()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();
}

float round_to_money(float num)
{
  float multiplyer = 100.00;
  return round(num * multiplyer) / multiplyer;
}

void setup()
{
  begin_serial();
  connect_to_wifi();
  initialize_display();
}

void loop()
{
  std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
  client->setInsecure(); // Ignore SSL certificate validation for now
  HTTPClient http_client;

  Serial.print("[HTTPS] begin...\n");

  if (http_client.begin(*client, "https://data.binance.com/api/v3/ticker/24hr?symbol=BTCUSDT")) {
    Serial.print("[HTTPS] GET...\n");

    int httpCode = http_client.GET();

    if (httpCode > 0) {
      Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        const char* payload = http_client.getString().c_str();
        DynamicJsonDocument payload_json(1024);
        deserializeJson(payload_json, payload);

        const char* symbol = payload_json["symbol"];
        const float last_price = round_to_money(payload_json["lastPrice"]);
        char frame_buffer[40];
        snprintf(frame_buffer, sizeof(frame_buffer), "%s: %.2f", symbol, last_price);

        display.clear();
        display.drawRect(0, 0, display.width(), display.height());
        display.drawString(2, 2, frame_buffer);
        display.display();
      }
    } else {
      Serial.printf("[HTTPS] GET... failed, error: %s\n", http_client.errorToString(httpCode).c_str());
    }
    http_client.end();
  } else {
    Serial.printf("[HTTPS] Unable to connect\n");
  }

  // 10 second delay for now
  delay(10000);
}

// "https://data.binance.com/api/v3/ticker/24hr?symbol=BTCUSDT"
//
// {
//   "symbol": "BTCUSDT",
//   "priceChange": "3.07000000",
//   "priceChangePercent": "0.011",
//   "weightedAvgPrice": "27970.51053763",
//   "prevClosePrice": "28161.32000000",
//   "lastPrice": "28164.39000000",
//   "lastQty": "0.00475000",
//   "bidPrice": "28164.39000000",
//   "bidQty": "1.16537000",
//   "askPrice": "28164.40000000",
//   "askQty": "7.80054000",
//   "openPrice": "28161.32000000",
//   "highPrice": "28444.44000000",
//   "lowPrice": "27200.24000000",
//   "volume": "65803.16635000",
//   "quoteVolume": "1840548157.80188850",
//   "openTime": 1680547786034,
//   "closeTime": 1680634186034,
//   "firstId": 3069673290,
//   "lastId": 3071083943,
//   "count": 1410654
// }
