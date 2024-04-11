#ifndef CONT_AUTOS_H
#define CONT_AUTOS_H

/*==================[inclusions]=============================================*/
#include "SD2_board.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/
typedef enum{
	SUMAR,
	RESTAR
} key_count_mode_enum;
 /*==================[external data declaration]==============================*/

/*==================[external functions definition]==========================*/

extern void count_updateCarCount(board_swId_enum id, key_count_mode_enum mode);
extern uint8_t count_getCarCount(board_swId_enum id);
extern void count_setCarCount(board_swId_enum id, uint8_t setValue);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* CONT_AUTOS_H */
