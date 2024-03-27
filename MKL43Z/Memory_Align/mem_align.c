
#include "board.h"


typedef struct  __attribute__((packed))
//typedef struct
{
    uint8_t dato8;
    uint16_t dato16;
    uint8_t dato8_2;
    uint32_t dato32;
}myStruct_t;


static myStruct_t myStruct;
static uint8_t *p8;
static uint16_t *p16;
static uint32_t *p32;


int main(void) {

    BOARD_InitDebugConsole();

    myStruct.dato8 = 0x12;
    myStruct.dato16 = 0x3456;
    myStruct.dato8_2 = 0x12;
    myStruct.dato32 = 0x789ABCDE;

    p8 = &myStruct.dato8;
    p16 = &myStruct.dato16;
    p32 = &myStruct.dato32;


    while (1)
    {
        *p8 = 0xAB;
        *p16 = 0xABCD;
        *p32 = 0xABCDEF12;
    }
}

