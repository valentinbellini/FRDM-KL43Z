#include "SSD1306.h"
#include <Arduino.h>

// Instancia del display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void ssd1306_setupDisplay() {
  display.clearDisplay();  // Limpiar el display
  display.setCursor(0, 0); // Establecer el cursor
  display.setTextSize(1);  // Tamaño del texto
  display.setTextColor(WHITE); // Color del texto
}

void ssd1306_init() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Dirección I2C
    Serial.println(F("Error al iniciar el display SSD1306"));
    for(;;); // Detener el programa si falla el display
  }
  ssd1306_setupDisplay();
  display.println("Welcome to ESP32");
  display.println("Conectar a ESP32_AP");
  display.display();
}

void ssd1306_showIP(IPAddress ip) {
  ssd1306_setupDisplay();
  
  // Convertir la dirección IP en cadena de texto
  String ipStr = ip.toString();

  // Mostrar la dirección IP en el display
  display.println("IP Address:");
  display.println(ipStr);
  display.display();                 // Actualizar el display con el contenido
}

void ssd1306_easyPrint(String str){
  ssd1306_setupDisplay();

  display.println(str);
  display.display();                 // Actualizar el display con el contenido
}

void ssd1306_printIPandString(IPAddress ip, String str){
  ssd1306_setupDisplay();
  String ipStr = ip.toString();
  display.println(str);
  display.println(ipStr);
  display.display();   
}


void ssd1306_ShowSetUpAP(IPAddress ip){
  ssd1306_setupDisplay();
  String ipStr = ip.toString();
  display.println("Bienvenido!");
  display.println("Para conectar ESP32 A WiFi");
  display.println("1. Conectarse a ESP32_AP");
  display.println("2. Abrir el explorador en:");
  display.println("http://"+ipStr);
  display.println("3. Completar los datos");
}

void ssd1306_ShowSTAConnected(IPAddress ip, String str){
  ssd1306_setupDisplay();
  String ipStr = ip.toString();
  display.println("Conexión Exitosa");
  display.println("[ESP32] IP Address asignada:");
  display.println(ipStr);
  display.println("BTC Address:");
  display.println(str);
}


// Función para mostrar el precio del token en el display OLED
void displayTokenPrice(String token, float price) {
  if (price != -1.0) {
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.println(token);
    
    display.setTextSize(1);
    display.setCursor(0, 25);
    display.println("Precio (USD):");
    
    display.setTextSize(2);
    display.setCursor(0, 40);
    display.println(price, 2); // Mostrar el precio con dos decimales
    
    display.display();
  } else {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Error obteniendo");
    display.println("el precio de:");
    display.setTextSize(2);
    display.setCursor(0, 40);
    display.println(token);
    display.display();
  }
}
