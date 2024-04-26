
/*==================[inclusions]=============================================*/
#include "mefModo.h"
#include "mefSec.h"
#include "mefSW1.h"

#include "SD2_board.h"
#include "key.h"

/*==================[macros and typedef]=====================================*/
// Estados de la MEF TOP que representa los distintos modos de funcionamiento del sistema
typedef enum {
    EST_MODO_SEC = 0,
    EST_MODO_SW1,
} estMefModo_enum;

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

static estMefModo_enum estMefModo;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

extern void mefModo_init(void)
{
    estMefModo = EST_MODO_SEC;

    mefSec_init();
    mefSW1_init();
}

extern void mefModo(void)
{
    switch (estMefModo)
    {
        case EST_MODO_SEC:
            mefSec();
            if (key_getPressEv(BOARD_SW_ID_1))
            {
                mefSW1_reset();
                estMefModo = EST_MODO_SW1;
            }
            break;

        case EST_MODO_SW1:
            if (mefSW1())
            {
                estMefModo = EST_MODO_SEC;
            }
            break;
    }
}

extern void mefModo_task1ms(void)
// Esta función es la clave para conservar el tiempo cuando se pasa de una MEF a otra.
{
    if (estMefModo == EST_MODO_SEC)
        mefSec_task1ms();

    if (estMefModo == EST_MODO_SW1)
        mefSW1_task1ms();
}


/*==================[end of file]============================================*/
