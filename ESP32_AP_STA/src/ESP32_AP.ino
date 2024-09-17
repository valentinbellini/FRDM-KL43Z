#include <WiFi.h>
#include <WebServer.h>
#include "wifi_manager.h"
#include "html.h"
#include "ssd1306.h"
#include <Ticker.h>
#include "crypto.h"

// =================================== [ MACROS AND TYPEDEF ] =================================== //

#define SERIAL_BAUDRATE 115200
#define CHANGE_TOKEN_REQUEST_TIME 30

/* Estados MEF */
enum Estado { AP_MODE, 
              STA_MODE, 
              CONECTADO };
              
Estado estadoActual = AP_MODE;

/* Almacena el dato extra recibido del formulario */
String extraData = "";  

WebServer server(80);

/* Lista de criptomonedas para mostrar */
const String tokens[] = {"bitcoin", "ethereum", "solana"};
const int numTokens = sizeof(tokens) / sizeof(tokens[0]);
int currentTokenIndex = 0; // Índice de la criptomoneda actual

/* Ticker para manejar interrupciones de tiempo */
Ticker timer;
bool timerStarted = false;  // Bandera para verificar si el temporizador ya se inició


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
      // ssd1306_ShowSTAConnected(WiFi.localIP(), extraData); /* NO MUESTRA ESTE MENSAJE */
      digitalWrite(2, HIGH);
      delay(500);
      digitalWrite(2, LOW);
      delay(500);

      // Iniciar el temporizador si aún no se ha iniciado
      if (!timerStarted) {
        timer.attach(CHANGE_TOKEN_REQUEST_TIME, switchToken);  // Cambiar el token cada 10 segundos
        timerStarted = true;            // Marcar el temporizador como iniciado
      }
      
      break;

  }
}



// ================================= [ CAMBIAR TOKEN ] ================================= //

// Función llamada por el temporizador con interrupciones para cambiar el token
void switchToken() {
  currentTokenIndex = (currentTokenIndex + 1) % numTokens;
  displayTokenPrice(tokens[currentTokenIndex], getTokenPrice(tokens[currentTokenIndex]));;
}
