#ifndef MEF_SECUNDARIO_H_
#define MEF_SECUNDARIO_H_

/*==================[inclusions]=============================================*/
#include <stdbool.h>
/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros and typedef]=====================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
extern void mef_secundario_init(void);
extern void mef_secundario_reset(void);
extern bool mef_secundario();	// True si hay que salir de la mef
extern void mef_secundario_task1ms(void);
/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* MEF_SECUNDARIO_H_ */


