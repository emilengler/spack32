#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#define SSID "karlsruhe.freifunk.net"
#define URL "https://club.entropia.de/spaceapi"

#define JSON_SIZE 200
#define DELAY 1000 * 60 * 1 // 1 minutes
#define LED_BUILTIN 2

void setup() {
  Serial.begin(9600);

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID);
  Serial.print("Connecting to WiFi .");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
  }
  Serial.println(WiFi.localIP());

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  HTTPClient client;

  client.begin(URL);
  int httpCode = client.GET();
  if (httpCode != HTTP_CODE_OK) {
    Serial.println("ERROR: HTTPS failure");
    delay(DELAY);
    return;
  }
  Serial.println("Got HTTP response");

  StaticJsonDocument<JSON_SIZE> doc;
  DeserializationError error = deserializeJson(doc, client.getString());
  if (error) {
    Serial.println("ERROR: Deserialization failure");
    delay(DELAY);
    return;
  }
  Serial.println("Parsed JSON successfully");

  bool open = doc["state"]["open"];
  Serial.println(open);

  if (open) {
    Serial.println("Setting LED to on");
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    Serial.println("Setting LED to off");
    digitalWrite(LED_BUILTIN, LOW);
  }

end:
  client.end();
  delay(DELAY);
}
