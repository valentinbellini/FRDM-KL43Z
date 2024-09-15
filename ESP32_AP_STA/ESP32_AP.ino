#include <WiFi.h>
#include <WebServer.h>
#include "ssd1306.h"

/* ======================================== [ MACROS & TYPEDEF ] ======================================== */

#define SERIAL_BAUDRATE     115200  // Serial_begin monitor

/* ======================================== [ VARIABLES ] ======================================== */

// Definición de variables globales para la red WiFi
const char* ssid_ap = "ESP32_AP";
const char* password_ap = "12345678";
String ssid;
String pass;
String extraData;

// Declaración de un servidor en el puerto 80
WebServer server(80);

// Estado de la conexión WiFi
enum Estado {AP_MODE, STA_MODE, CONECTADO};
Estado estadoActual = AP_MODE;

/* ======================================== [ SET UP ] ======================================== */


// Función para manejar la página principal (root) y mostrar el formulario HTML
void handleRoot() {
  String html = "<html><body>";
  html += "<h1>Ingrese los datos de la red WiFi</h1>";
  html += "<form action=\"/connect\" method=\"POST\">";
  html += "SSID: <input type=\"text\" name=\"ssid\"><br>";
  html += "Password: <input type=\"password\" name=\"password\"><br>";
  html += "Extra Data: <input type=\"text\" name=\"extraData\"><br>";
  html += "<input type=\"submit\" value=\"Conectar\">";
  html += "</form></body></html>";

  server.send(200, "text/html", html);
}

// Función para manejar los datos recibidos del formulario
void handleConnect() {
  ssid = server.arg("ssid");
  pass = server.arg("password");
  extraData = server.arg("extraData");

  server.send(200, "text/html", "<html><body><h1>Conectando a la red...</h1></body></html>");
  
  WiFi.softAPdisconnect(true);  // Apaga el modo AP
  WiFi.begin(ssid.c_str(), pass.c_str()); // Intenta conectar al WiFi con los datos recibidos

  estadoActual = STA_MODE; // Cambia el estado para comenzar a intentar la conexión
}

// Función que configura el modo AP y levanta el servidor web
void setupAP() {
  Serial.println("Configurando AP...");
  WiFi.softAP(ssid_ap, password_ap);
  
  IPAddress IP = WiFi.softAPIP();
  Serial.print("IP del AP: ");
  Serial.println(IP);
  ssd1306_showIP(IP);
  
  server.on("/", handleRoot); // Asigna el manejador de la página raíz
  server.on("/connect", handleConnect); // Asigna el manejador para procesar el formulario
  server.begin();  // Inicia el servidor web
}

// Configuración inicial
void setup() {
  // Inicia el puerto serial y limpia los datos previos
  Serial.begin(SERIAL_BAUDRATE);
  Serial.println();
  Serial.println("[MAIN SETUP] Iniciando...");

  ssd1306_init();
  setupAP();  // Inicia en modo AP
  Serial.println("[MAIN SETUP] AP iniciado");
}

// Bucle principal
void loop() {
  switch (estadoActual) {
    case AP_MODE:
      server.handleClient();  // Maneja las peticiones al servidor mientras está en modo AP
      break;
    case STA_MODE:
      if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Conexión exitosa a la red WiFi!");
        Serial.print("Dirección IP: ");
        Serial.println(WiFi.localIP());
        ssd1306_easyPrint("Conexión exitosa a la red WiFi");
        
        // Aquí es donde se da lugar a la ejecución principal del programa.
        estadoActual = CONECTADO;
      } else {
        Serial.println("Conectando...");
        delay(500);
      }
      break;
    case CONECTADO:
      ssd1306_easyPrint("Conexion exitosa.");
      // Este es el lugar donde iría tu código principal
      // Puedes usar extraData o cualquier lógica que desees añadir.
      break;
  }
}
