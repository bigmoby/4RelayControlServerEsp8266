#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "[SSID_HERE]";
const char* password = "[SECRET_HERE]";

ESP8266WebServer server(80);

const int pin_relay_1 = 5;
const int pin_relay_2 = 4;
const int pin_relay_3 = 14;
const int pin_relay_4 = 12;

void handleRoot() {
  server.send(200, "text/plain", "REST API: /command?state=on&relay=1  ---> relay ON! \nREST API: command?state=off&relay=1 ---> relay OFF!");
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void) {
  pinMode(pin_relay_1, OUTPUT);
  pinMode(pin_relay_2, OUTPUT);
  pinMode(pin_relay_3, OUTPUT);
  pinMode(pin_relay_4, OUTPUT);
  digitalWrite(pin_relay_1, 1);
  digitalWrite(pin_relay_2, 1);
  digitalWrite(pin_relay_3, 1);
  digitalWrite(pin_relay_4, 1);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/command", []() {
    String state = server.arg("state");
    String relay_num = server.arg("relay");
    char relayChar[2];
    serialReceived.toCharArray(relayChar, 2);
    if (state == "on") {
      switch (relayChar)
      {
        case '1': digitalWrite(pin_relay_1, 0); server.send(200, "text/plain", "Relay 1 acceso!"); break;
        case '2': digitalWrite(pin_relay_2, 0); server.send(200, "text/plain", "Relay 2 acceso!");break;
        case '3': digitalWrite(pin_relay_3, 0); server.send(200, "text/plain", "Relay 3 acceso!");break;
        case '4': digitalWrite(pin_relay_4, 0); server.send(200, "text/plain", "Relay 4 acceso!");break;
        default: server.send(200, "text/plain", "No relay found!"); break;
      }
    }
    else if (state == "off") {
      switch (relayChar)
      {
        case '1': digitalWrite(pin_relay_1, 1); server.send(200, "text/plain", "Relay 1 spento!"); break;
        case '2': digitalWrite(pin_relay_2, 1); server.send(200, "text/plain", "Relay 2 spento!");break;
        case '3': digitalWrite(pin_relay_3, 1); server.send(200, "text/plain", "Relay 3 spento!");break;
        case '4': digitalWrite(pin_relay_4, 1); server.send(200, "text/plain", "Relay 4 spento!");break;
        default: server.send(200, "text/plain", "No relay found!"); break;
      }
    }
  });

  server.on("/help", []() {
    server.send(200, "text/plain", "REST API: /command?state=on&relay=1  ---> relay ON! \nREST API: command?state=off&relay=1 ---> relay OFF!");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}
