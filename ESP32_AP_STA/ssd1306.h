#ifndef SSD1306_H
#define SSD1306_H

#include <Adafruit_SSD1306.h>  // Librería del display
#include <Adafruit_GFX.h>

// Definir dimensiones de la pantalla
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1  // Pin de reset (puedes usar -1 si no lo usas)

// Crear la instancia del display
extern Adafruit_SSD1306 display;

// Función para inicializar el display
void ssd1306_init();
void ssd1306_showIP(IPAddress ip);
void ssd1306_easyPrint(String str);


#endif