#ifndef CONT_AUTOS_H
#define CONT_AUTOS_H

/*==================[inclusions]=============================================*/
#include "SD2_board.h"
#include <stdint.h>

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/
typedef enum{
	SUMAR,
	RESTAR
} car_count_mode_enum;
 /*==================[external data declaration]==============================*/

/*==================[external functions definition]==========================*/

extern void count_updateCarCount(board_swId_enum id, car_count_mode_enum mode);
extern uint8_t count_getCarCount(board_swId_enum id);
extern void count_resetCarCount(board_swId_enum id);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* CONT_AUTOS_H */
