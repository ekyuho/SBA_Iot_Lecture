#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS D4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

#include <ESP8266WiFi.h>
const char* ssid     = "cookie";
const char* password = "0317137263";
const char* host = "184.106.153.149";    //"thingspeak.com"

void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  sensors.begin();
}

void loop() {
  delay(12000);

  sensors.requestTemperatures();
  float value = sensors.getTempCByIndex(0);

  Serial.print("connecting to ");
  Serial.println(host);
  
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  String url = "/update?key=04H5CMQQBR1RGFVY&field1=" + String(value);
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url +" HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");
}
