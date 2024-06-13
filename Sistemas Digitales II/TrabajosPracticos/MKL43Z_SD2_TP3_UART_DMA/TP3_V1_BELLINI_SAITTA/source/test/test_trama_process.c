#include "App/trama_process.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h> // Para usar strlen

#define _BUFFER_SIZE			32

void test_tramaProcess() {
    char buffer[_BUFFER_SIZE] = "0A0"; // Trama de ejemplo
    printf("Prueba de tramaProcess con trama: %s\n", buffer);
    tramaProcess(buffer, strlen(buffer));
    // Agrega más pruebas según sea necesario
}

