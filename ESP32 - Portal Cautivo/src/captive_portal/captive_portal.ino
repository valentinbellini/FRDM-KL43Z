#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include "wifi_config.h" // Contiene las credenciales: ssid_sta y password_sta

// Definir el SSID y contraseña del punto de acceso
const char* ssid_ap = "MiPortalAP";
const char* password_ap = "12345678";

// Crear una instancia del servidor web asíncrono en el puerto 80
AsyncWebServer server(80);

// Función para inicializar el sistema de archivos SPIFFS
void initSPIFFS() {
    if (!SPIFFS.begin(true)) {
        Serial.println("No se pudo montar SPIFFS");
        while (true); // Detener la ejecución si no se puede montar SPIFFS
    }
    Serial.println("SPIFFS montado con éxito.");
}

// Función para configurar la ESP32 como Access Point (AP)
void setupAccessPoint() {
    WiFi.softAP(ssid_ap, password_ap);
    Serial.println("Access Point configurado.");
}

// Función para conectar la ESP32 a una red Wi-Fi como cliente (STA)
void connectToWiFi() {
    WiFi.begin(ssid_sta, password_sta);

    int retryCount = 0;
    int maxRetries = 20;

    // Intentar conectarse a la red Wi-Fi un número limitado de veces
    while (WiFi.status() != WL_CONNECTED && retryCount < maxRetries) {
        delay(1000);
        Serial.println("Conectando a WiFi...");
        retryCount++;
    }

    // Verificar si la conexión fue exitosa
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Conectado a la red Wi-Fi!");
    } else {
        Serial.println("No se pudo conectar a la red Wi-Fi. Continuando en modo AP...");
    }
}

// Función para configurar las rutas y manejar las peticiones HTTP del servidor web
void setupWebServer() {
    // Servir la página principal del portal cautivo
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/index.html", "text/html");
    });

    // Servir el archivo CSS
    server.on("/styles.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/styles.css", "text/css");
    });

    // Servir el archivo JavaScript
    server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/script.js", "application/javascript");
    });

    // Manejar las rutas no encontradas
    server.onNotFound([](AsyncWebServerRequest *request) {
        request->send(404, "text/plain", "404: Not Found");
    });

    // Iniciar el servidor web
    server.begin();
    Serial.println("Servidor web iniciado.");
}

void setup() {
    // Inicializar la comunicación serial para la depuración
    Serial.begin(115200);

    // Inicializar SPIFFS
    initSPIFFS();

    // Configurar el Access Point (AP)
    setupAccessPoint();

    // Intentar conectarse a una red Wi-Fi
    connectToWiFi();

    // Configurar y empezar el servidor web
    setupWebServer();
}

void loop() {
    // No es necesario código en loop para la configuración básica
    // Todo se maneja de manera asíncrona
}
