#include <WiFi.h>
#include <WebServer.h>
#include "wifi_manager.h"
#include "html.h"
#include "ssd1306.h"


// =================================== [ MACROS AND TYPEDEF ] =================================== //
#define SERIAL_BAUDRATE 115200

enum Estado { AP_MODE, 
              STA_MODE, 
              CONECTADO };
              
Estado estadoActual = AP_MODE;
String extraData = "";  // Almacena el dato extra recibido del formulario

WebServer server(80);

// =================================== [ FUNCTIONS ] =================================== //

void handleRoot() {
  server.send(200, "text/html", HTML_FORM);
}

void handleConnect() {
  String ssid = server.arg("ssid");
  String pass = server.arg("password");
  extraData = server.arg("extraData");

  server.send(200, "text/html", "<html><body><h1>Conectando a la red...</h1></body></html>");

  WiFi.softAPdisconnect(true);
  connectToWiFi(ssid, pass);
  estadoActual = STA_MODE;
}

// =================================== [ SETUP ] =================================== //

void setup() {
  Serial.begin(SERIAL_BAUDRATE);
  Serial.println("[MAIN SETUP] Iniciando...");

  ssd1306_init();
  setupAP(server);  // Configura el AP y el servidor web

  pinMode(2,OUTPUT);
}


// =================================== [ LOOP ] =================================== //

void loop() {
  switch (estadoActual) {
    case AP_MODE:
      server.handleClient();
      break;

    case STA_MODE:
      if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Conexión exitosa a la red WiFi!");
        ssd1306_easyPrint("Conexion exitosa.");
        estadoActual = CONECTADO;
      } else {
        Serial.println("Conectando...");
        ssd1306_easyPrint("Conectando...");
        delay(500);
      }
      break;

    case CONECTADO:
      // Lugar para la ejecución principal
      ssd1306_ShowSTAConnected(WiFi.localIP(), extraData); /* NO MUESTRA ESTE MENSAJE */
      digitalWrite(2, HIGH);
      delay(500);
      digitalWrite(2, LOW);
      delay(500);
      break;

  }
}
