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
  display.clearDisplay();
}

void ssd1306_first_msg(){
  ssd1306_setupDisplay();
  display.println("Welcome to ESP32");
  display.println("Conectar a ESP32_AP");
  display.println("Pass: 12345678");
  display.println("http://192.168.4.1");
}

void ssd1306_easyPrint(String str){
  ssd1306_setupDisplay();
  display.println(str);
  display.display();                 // Actualizar el display con el contenido
}

void ssd1306_conexionExitosa(){
  ssd1306_setupDisplay();
  display.println("Conexion exitosa.");
  display.println("Cargando datos...");
}


// Función para mostrar el precio del token en el display OLED
void displayTokenPrice(String token, float price) {
  token.toUpperCase();  // Convertir el token a mayúsculas para impresion
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
