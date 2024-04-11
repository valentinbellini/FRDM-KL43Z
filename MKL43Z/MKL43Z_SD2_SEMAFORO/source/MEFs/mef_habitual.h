#ifndef MEF_HABITUAL_H_
#define MEF_HABITUAL_H_

/*==================[inclusions]=============================================*/
#include <stdbool.h>
/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros and typedef]=====================================*/

typedef enum {
    TR_NONE = 0,
	TR_TO_PEATON,
	TR_TO_SECUNDARIO,
} tr_enum;

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

extern void mef_habitual_init(void);
extern void mef_habitual_reset(void);
extern void mef_habitual_init_sec_5(void);
extern tr_enum mef_habitual(void);
extern void mef_habitual_task1ms(void);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* MEF_HABITUAL_H_ */


