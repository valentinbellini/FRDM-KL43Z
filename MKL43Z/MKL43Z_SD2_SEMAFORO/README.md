# Implementación de Máquina de Estado Finito para Control de Cruce de Caminos

Este proyecto contiene la implementación de una Máquina de Estado Finito (MEF) para controlar el cruce de caminos, modelando el comportamiento descrito en el enunciado del problema.

## Descripción del Problema

El cruce consta de una ruta principal, un camino secundario y un paso peatonal sobre la ruta. Se deben controlar las señales de tráfico para vehículos y peatones, así como la detección de vehículos en el camino secundario.

## Estructura del Proyecto

El proyecto está estructurado en archivos separados para cada MEF, siguiendo el criterio de reutilización de código:

- `main.c`: Archivo principal que contiene la lógica principal del programa y las funciones de inicialización.
- `mef_modo.h` y `mef_modo.c`: Implementación del MEF para controlar las transiciones entre las sub-mefs.
- `mef_peaton.h` y `mef_peaton.c`: Implementación del MEF para controlar el cruce peatonal.
- `mef_habitual.h` y `mef_habitual.c`: Implementación del MEF en la secuncia habitual de operación.
- `mef_secundario.h` y `mef_secundario.c`: Implementación del MEF para controlar el cruce secundario a partir de los autos detectados por el sensor.
- `cont_autos.h` y `cont_autos.c`: Funciones para contar la cantidad de autos almacenados en el camino secundario.
- `key.h` y `key.c`: Funciones para controlar el pulsado de los interruptores (SW) de la placa.
- `SD2_board.h` y `SD2_board.c`: Funciones de inicialización de periféricos, configuración de clocks, entre parámetros de la placa de desarrollo.

## StateCharts

![DiagramaMEF](MEFtp1.svg)

## Detalles de Implementación

- Se utilizan los leds de la placa FRDM-KL43Z para representar las señales de tráfico.
- Se emplea el pulsador SW1 para simular el cruce peatonal.
- Se simula la detección de vehículos en el camino secundario utilizando el pulsador SW3.

## Instrucciones de Compilación y Ejecución

1. Clonar este repositorio en la placa FRDM-KL43Z.
2. Compilar el proyecto utilizando el entorno de desarrollo de su preferencia.
3. Ejecutar el programa en la placa FRDM-KL46Z.

## Contribuciones

Las contribuciones son bienvenidas. Si encuentra algún error o tiene sugerencias de mejora, por favor, abra un problema o envíe una solicitud de extracción.

