#ifndef MEF_MAIN_H_
#define MEF_MAIN_H_

/*==================[inclusions]=============================================*/
#include <stdbool.h>
/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros and typedef]=====================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
extern void mef_main_init(void);
extern void mef_main(void);	// True si hay que salir de la mef
extern void mef_main_task1ms(void);
/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* MEF_MAIN_H_ */

