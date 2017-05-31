/*
 * 2017/3/15  Kyuho Kim
 * Template for using Thing Speak Service
 */

const char* host = "api.thingspeak.com";
String url = "/update?api_key=MVBATSXOGBR62YZX";   // Your Own Key here
const int httpPort = 80;
int interval = 20000;

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

const char* ssid = "cookie";   // Your own ssid here
const char* password = "0317137263";  // Your own password here

int pm25, pm10;

String working() { 
  //int r = pm25; // do some magic here!
  return(String("field1=")+String(pm25) +"&field2="+ String(pm10));
}

void delivering(String payload) { 
  WiFiClient client;
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpPort)) {
    Serial.print("connection failed: ");
    Serial.println(payload);
    return;
  }
  String getheader = "GET "+ String(url) +"&"+ String(payload) +" HTTP/1.1";
  client.println(getheader);
  client.println("User-Agent: ESP8266 Kyuho Kim");  
  client.println("Host: " + String(host));  
  client.println("Connection: close");  
  client.println();

  Serial.println(getheader);
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    Serial.println(line);
  }
  Serial.println("Done cycle.");
}

void connect_ap() {
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\n Got WiFi, IP address: ");
  Serial.println(WiFi.localIP());  
}

void setup() {
  Serial.begin(9600);
  connect_ap();

  Serial.println("ESPArduinoThingSpeak.cpp - 2017/3/15");
}

unsigned long mark = 0;
void loop() {
  static int  stat = 0;
  static byte buf[10];
  static int i;

  while (Serial.available()) {
    char c = Serial.read();
    //Serial.print("Stat="+ String(stat) +" c=");
    //Serial.println(c, HEX);

    switch(stat) {
      case 0: if (c == 0xAA) stat = 1; break;
      case 1: if (c == 0xC0) stat = 2; i = 0; break;
      case 2: if (i == 6) stat = 3;
              else buf[i++] = c;
              break;
      case 3: if (c == 0xAB) {
                stat = 0;
                
                pm25 = buf[0] + buf[1] * 255;
                pm10 = buf[2] + buf[3] * 255;
                Serial.println("PM2.5="+ String(pm25) +", PM10="+ String(pm10));
              } else {
                Serial.print("Wrong END");
                Serial.println(c, HEX);
              }
              break;
    }

  }

  if (millis() > mark ) {
     mark = millis() + interval;
     String payload = working();
     delivering(payload);
  }
}
