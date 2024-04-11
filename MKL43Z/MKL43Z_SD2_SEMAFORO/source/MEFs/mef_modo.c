#include <MEFs/mef_modo.h>
#include <MEFs/mef_habitual.h>
#include <MEFs/mef_peaton.h>
#include <MEFs/mef_secundario.h>
#include <stdint.h>
#include "SD2_board.h"
#include "cont_autos.h"

/*==================[macros and typedef]====================================*/
typedef enum {
    EST_MODO_HABITUAL = 0,
    EST_MODO_PEATON,
    EST_MODO_SECUNDARIO,
} estMefModo_enum;

/*==================[internal data definition]==============================*/
static estMefModo_enum estado_MEF_modo;

/*==================[external functions definition]=========================*/

extern void mef_modo_init(void){
    estado_MEF_modo = EST_MODO_HABITUAL;
    count_setCarCount(BOARD_SW_ID_3,0); // Setting car count at initial value: 0
    /* Inicializacion de sub-mefs */
    mef_habitual_init();
    mef_peaton_init();
    mef_secundario_init();
}

extern void mef_modo(void){
    switch(estado_MEF_modo){
        case EST_MODO_HABITUAL:
        	switch(mef_habitual()){
        		case TR_TO_PEATON:
        			estado_MEF_modo = EST_MODO_PEATON;
        			mef_peaton_reset();
        		break;
        		case TR_TO_SECUNDARIO:
        			estado_MEF_modo = EST_MODO_SECUNDARIO;
        			mef_secundario_reset();
        		break;
        		default:
        			estado_MEF_modo = EST_MODO_HABITUAL;
        		break;
        	}
        	break;
        case EST_MODO_PEATON:
            if(mef_peaton()){
            	estado_MEF_modo = EST_MODO_HABITUAL;
            	mef_habitual_init_sec_5();
            }
            break;
        case EST_MODO_SECUNDARIO:
            if(mef_secundario()){
            	estado_MEF_modo = EST_MODO_HABITUAL;
            	mef_habitual_reset();
            }
            break;
    }
}

extern void mef_modo_task1ms(void){
    if (estado_MEF_modo == EST_MODO_HABITUAL) mef_habitual_task1ms();
    if (estado_MEF_modo == EST_MODO_PEATON) mef_peaton_task1ms();
    if (estado_MEF_modo == EST_MODO_SECUNDARIO) mef_secundario_task1ms();
}

/*==================[end of file]============================================*/
