 
## DESCRIPCION DE LAS FUNCIONES
	
- La función `board_init()` se encarga de inicializar los pines GPIO para los LEDs y pulsadores, 
	configurando su dirección (entrada o salida) y otras características como resistencias 
	pull-up/pull-down, velocidad de cambio y filtrado pasivo.

- La función `board_setLed()` permite controlar el estado de un LED específico en 
	la placa, pudiendo encenderlo, apagarlo o alternar su estado.

- La función `board_getSw()` devuelve el estado (presionado o no) de un pulsador específico en la placa.
 