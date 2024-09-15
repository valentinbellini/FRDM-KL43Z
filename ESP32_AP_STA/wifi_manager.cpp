#include "wifi_manager.h"
#include "ssd1306.h"

const char* ssid_ap = "ESP32_AP";
const char* password_ap = "12345678";

void setupAP(WebServer &server) {
  Serial.println("Configurando AP...");
  WiFi.softAP(ssid_ap, password_ap);
  
  IPAddress IP = WiFi.softAPIP();
  Serial.print("IP del AP: ");
  Serial.println(IP);
  ssd1306_ShowSetUpAP(IP); /* NO MUESTRA ESTE MENSAJE */
  
  server.on("/", handleRoot);
  server.on("/connect", handleConnect);
  server.begin();
}

void connectToWiFi(String ssid, String password) {
  WiFi.begin(ssid.c_str(), password.c_str());
}
