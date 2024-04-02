#include "mef_main.h"
#include <stdint.h>
#include "SD2_board.h"

/*==================[macros and typedef]====================================*/
typedef enum {
    EST_MAIN_HAB = 0,
    EST_MAIN_PEATON,
    EST_MAIN_SECUNDARIO,
} estMefMain_enum;

/*==================[internal data definition]==============================*/
static estMefMain_enum estado_MEF_main;

/*==================[external functions definition]=========================*/

extern void mef_main_init(void){
    estado_MEF_main = EST_MAIN_HAB;
    /* Inicializacion de sub-mefs */
    mef_peaton_init()
    mef_secundario_init()
    mef_habitual_init()
}

extern void mef_main(void){
    switch(estado_MEF_main){
        case EST_MAIN_HAB:
            mef_habitual();
            if (key_getPressEv(BOARD_SW_ID_1)){
                mef_peaton_reset();
                estado_MEF_main = EST_MAIN_PEATON;
            }
            if(getAutos() >= 3){
                mef_secundario_reset()
                estado_MEF_main = EST_MAIN_SECUNDARIO;
            }
            break;
        case EST_MAIN_PEATON:
            if(mef_peaton()){
                estado_MEF_main = EST_MAIN_HAB;
            }
            break;
        case EST_MAIN_SECUNDARIO:
            if(mef_secundario()){
                estado_MEF_main = EST_MAIN_HAB;
            }
            break;
    }
}

extern void mef_main_task1ms(void){
    if (estado_MEF_main == EST_MAIN_HAB) mef_habitual_task1ms();
    if (estado_MEF_main == EST_MAIN_PEATON) mef_peaton_task1ms();
    if (estado_MEF_main == EST_MAIN_SECUNDARIO) mef_secundario_task1ms();
}

/*==================[end of file]============================================*/
