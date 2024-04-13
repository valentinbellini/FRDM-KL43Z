/**
 * @file    mef_modo.c
 * @brief   Hierarchical MEF for main logic.
 * @autor	Valentin Bellini & Iván Saitta
 */

/*==================[inclusions]=============================================*/
#include <MEFs/mef_modo.h>
#include <MEFs/mef_habitual.h>
#include <MEFs/mef_peaton.h>
#include <MEFs/mef_secundario.h>
#include <stdint.h>
#include <stdbool.h>

#include "SD2_board.h"
#include "key.h"
#include "cont_autos.h"

/*==================[macros and typedef]====================================*/
typedef enum { // MEF states
    EST_MODO_HABITUAL = 0,
    EST_MODO_PEATON,
    EST_MODO_SECUNDARIO,
} estMefModo_enum;

/*==================[internal data definition]==============================*/
static estMefModo_enum estado_MEF_modo;
/*==================[external functions definition]=========================*/

void mef_modo_init(void){
    estado_MEF_modo = EST_MODO_HABITUAL;	// Initial State
    count_resetCarCount(BOARD_SW_ID_3); 	// Setting car count at initial value: 0
    /* sub-mefs initialization */
    mef_habitual_init();
    mef_peaton_init();
    mef_secundario_init();
}

void mef_modo(void){
    switch(estado_MEF_modo){
        case EST_MODO_HABITUAL:
        	switch(mef_habitual()){		// MEF evaluation return tr_enum type
        		case TR_TO_PEATON:		// Transition from mef_habitual EST_SEC_1 to mef_peaton with reset.
        			estado_MEF_modo = EST_MODO_PEATON;
        			mef_peaton_reset();
        		break;
        		case TR_TO_SECUNDARIO: 	// Transition from mef_habitual EST_SEC_1 to mef_secundario with reset.
        			estado_MEF_modo = EST_MODO_SECUNDARIO;
        			mef_secundario_reset();
        		break;
        		default:				// No transition to a different MEF. (return is "TR_NONE")
        			estado_MEF_modo = EST_MODO_HABITUAL;
        		break;
        	}
        	break;
        case EST_MODO_PEATON:
            if(mef_peaton()){			// MEF evaluation return bool. True if Timeout on mef_peaton.
            	estado_MEF_modo = EST_MODO_HABITUAL;
            	mef_habitual_init_sec_5();	// Init mef_habitual in state "EST_SEC_5"
            }
            break;
        case EST_MODO_SECUNDARIO:
            if(mef_secundario()){		// MEF evaluation return bool.
            	estado_MEF_modo = EST_MODO_HABITUAL;
            	mef_habitual_reset();	// Transition with Reset.
            }
            break;
    }
}

void mef_modo_task1ms(void){
	// The condition is evaluated to decrease the appropriate counters.
    if (estado_MEF_modo == EST_MODO_HABITUAL) mef_habitual_task1ms();
    if (estado_MEF_modo == EST_MODO_PEATON) mef_peaton_task1ms();
    if (estado_MEF_modo == EST_MODO_SECUNDARIO) mef_secundario_task1ms();
}

/*==================[end of file]============================================*/
