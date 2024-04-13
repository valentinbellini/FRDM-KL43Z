/**
 * @file    mef_modo.h
 * @brief   Hierarchical MEF for main logic.
 * @autor	Valentin Bellini & Iván Saitta
 */

#ifndef MEF_MODO_H_
#define MEF_MODO_H_

/*==================[inclusions]=============================================*/
#include <stdbool.h>
/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros and typedef]=====================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
extern void mef_modo_init(void);
extern void mef_modo(void);
extern void mef_modo_task1ms(void);
/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* MEF_MODO_H_ */

