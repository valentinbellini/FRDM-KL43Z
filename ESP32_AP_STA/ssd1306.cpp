#include "SSD1306.h"
#include <Arduino.h>

// Instancia del display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void ssd1306_init() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Dirección I2C
    Serial.println(F("Error al iniciar el display SSD1306"));
    for(;;); // Detener el programa si falla el display
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.display();
}

void ssd1306_showIP(IPAddress ip) {
  display.clearDisplay();            // Limpiar el display antes de mostrar la IP
  display.setCursor(0, 0);           // Establecer el cursor en la posición inicial
  display.setTextSize(1);            // Tamaño del texto
  display.setTextColor(WHITE);       // Color del texto
  
  // Convertir la dirección IP en cadena de texto
  String ipStr = "IP: " + ip.toString();

  // Mostrar la dirección IP en el display
  display.println(ipStr);
  display.display();                 // Actualizar el display con el contenido
}

void ssd1306_easyPrint(String str){
  display.clearDisplay();            // Limpiar el display antes de mostrar la IP
  display.setCursor(0, 0);           // Establecer el cursor en la posición inicial
  display.setTextSize(1);            // Tamaño del texto
  display.setTextColor(WHITE);       // Color del texto
  display.println(str);
  display.display();                 // Actualizar el display con el contenido
}



