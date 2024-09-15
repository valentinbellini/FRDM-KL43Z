#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include <WebServer.h>
#include "ssd1306.h"

// Prototipos de las funciones
void setupAP(WebServer &server);
void connectToWiFi(String ssid, String password);

// Declarar los prototipos de las funciones manejadoras de rutas para ser visibles en wifi_manager.cpp
void handleRoot();
void handleConnect();

#endif
