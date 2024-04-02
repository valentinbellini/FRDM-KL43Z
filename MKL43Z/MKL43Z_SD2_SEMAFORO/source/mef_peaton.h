#ifndef MEF_PEATON_H_
#define MEF_PEATON_H_

/*==================[inclusions]=============================================*/
#include <stdbool.h>
/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros and typedef]=====================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
extern void mef_peaton_init(void);
extern void mef_peaton_reset(void);
extern bool mef_peaton();	// True si hay que salir de la mef
extern void mef_peaton_task1ms(void);
/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* MEF_PEATON_H_ */