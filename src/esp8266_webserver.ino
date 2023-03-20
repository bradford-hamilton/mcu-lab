#include <ESP8266WebServer.h>

const char* ssid = "ssid";
const char* pass = "password";
const char* hostname = "nodemcu";
const char* root_html = R"(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<style>
  html, body {
    height: 100%;
    margin: 0;
    font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
  }
  .container {
    display: flex;
    align-items: center;
    justify-content: center;
    height: 100%;
    background: linear-gradient(135deg, #56ab2f 0%, #787ef4 100%);
  }
  .message {
    text-align: center;
    font-size: 2.5rem;
    font-weight: bold;
    color: #ffffff;
    background-color: rgba(255, 255, 255, 0.1);
    padding: 1.5rem 2rem;
    border-radius: 10px;
    box-shadow: 0 4px 15px rgba(0, 0, 0, 0.2);
  }
</style>
</head>
<body>
<div class="container">
  <div class="message">
    Toby Missy Elliot
  </div>
</div>
</body>
</html>
)";

ESP8266WebServer srv{80};

void setup()
{
  Serial.begin(115200);

  Serial.println();
  Serial.print("Connecting");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  WiFi.setHostname(hostname);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  srv.on("/", []() {
    srv.send(200, "text/html", root_html);
  });

  srv.begin();
}

void loop()
{
  srv.handleClient();
}
