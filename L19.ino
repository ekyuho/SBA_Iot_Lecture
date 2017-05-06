#include <ESP8266WiFi.h>

const char* ssid = "ssid";
const char* password = "password";

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(D4, OUTPUT);
  digitalWrite(D4, 0);
  
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.config(IPAddress(192,168,0,199), IPAddress(192,168,0,1), IPAddress(255,255,255,0));
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  server.begin();
  Serial.println("Server started");

  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  int val;
  if (req.indexOf("/gpio/0") != -1) {
    val = 0;
    digitalWrite(D4, val);
  } else if (req.indexOf("/gpio/1") != -1) {
    val = 1;
    digitalWrite(D4, val);
  } else if (req.indexOf("/favicon.ico") != -1)
    ;
  else {
    Serial.println("invalid request");
    client.stop();
    return;
  }
  client.flush();

  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nGPIO is now ";
  s += (val)?"high":"low";
  s += "</html>\n";

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");
}
