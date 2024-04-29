// Constantes de la Red de Petri
#define MAX_LUGARES     6
#define MAX_TRANSICIONES    4
#define MAX_ENTRADAS    2

// Vector de entradas adquiridas
int8_t entradas[MAX_ENTRADAS];

// Adquisición de entradas (usando macros)
void adquisicion_entradas(){
    entradas[0] = SW1;
    entradas[1] = SW2;
}

// Matriz de incidencia de la Red de Petri:
int8_t matriz_incidencia[MAX_LUGARES][MAX_TRANSICIONES] ={
    {-1 , 0 , 0 , 1 },
    { 1 ,-1 , 0 , 0 },
    { 0 , 1 ,-1 , 0 },
    { 0 , 0 ,-1 , 1 },
    { 0 , 1 , 0 , -1},
    { 0 , 0 , 1 , -1}
};

// Marcado de la Red
uint8_t marcado[MAX_LUGARES] = {1,0,0,1,0,0};

// Condiciones de disparo de las transiciones
uint8_t ft0(){return entradas[0];}
uint8_t ft1(){return !entradas[0] && entradas[1];}
uint8_t ft2(){return !entradas[1] && entradas[0];}
uint8_t ft3(){return !entradas[0];}

// Vector de funciones de habilitacion de transiciones (punteros a funciones)
uint8_t (*trans_func[MAX_TRANSICIONES])()={ft0,ft1,ft2,ft3};

// Estado transiciones (vector de sensibilizacion)
uint8_t sensibilizacion[MAX_TRANSICIONES];

// Establecimiento de las salidas (usando macros)
void salidas(){
    led_rojo(marcado[0] || (marcado[2]&&entradas[1]) || marcado[4]);
    led_verde(marcado[4]);
    led_amarillo(marcado[1] || marcado[5]);
}