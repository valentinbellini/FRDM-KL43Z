#ifndef MMA8451_REGISTERS_H_
#define MMA8451_REGISTERS_H_

/*==================[inclusions]=============================================*/
#include "stdint.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/


// Direcciones de los registros del acelerómetro
#define STATUS_ADDRESS       	0X00
#define OUT_X_MSB_ADDRESS		0X01 	// Brief: These registers contain the X-axis,
#define OUT_X_LSB_ADDRESS		0X02	// Y-axis,
#define OUT_Y_MSB_ADDRESS		0X03	// and Z-axis,
#define OUT_Y_LSB_ADDRESS		0X04	// and 14-bit output sample data
#define OUT_Z_MSB_ADDRESS		0X05	// expressed as 2's
#define OUT_Z_LSB_ADDRESS		0X06	// complement numbers.
#define INT_SOURCE_ADDRESS   	0X0C
#define FF_MT_CFG_ADDRESS		0X15
#define FF_MT_SRC_ADDRESS		0X16
#define FF_MT_THS_ADDRESS		0X17
#define FF_MT_COUNT_ADRESS		0X18
#define CTRL_REG1_ADDRESS   	0X2A
#define CTRL_REG4_ADDRESS   	0X2D
#define CTRL_REG5_ADDRESS   	0X2E

/*==================[registers]=================================================*/

// STATUS_t
typedef union
{
    struct
    {
        unsigned XDR:1;
        unsigned YDR:1;
        unsigned ZDR:1;
        unsigned ZYXDR:1;
        unsigned XOW:1;
        unsigned YOW:1;
        unsigned ZOW:1;
        unsigned ZYXOW:1;
    };
    uint8_t data;
}STATUS_t;

// INT_SOURCE_t --> Determinar la fuente de la IRQ. (Se limpian leyendo)
typedef union
{
    struct
    {
        unsigned SRC_DRDY:1;	// Data-Ready interrupt bit status -- Logic ‘1’ indicates that the X, Y, Z data-ready interrupt is active indicating the presence of new data and/or data overrun.
        unsigned :1;
        unsigned SRC_FF_MT:1;	// FreeFall/Motion interrupt status bit. Default 0 (inactive)
        unsigned SRC_PULSE:1;	// Pulse interrupt status bit. Default 0. '1' means interrupt was generated
        unsigned SRC_LNDPRT:1;
        unsigned SRC_TRANS:1;
        unsigned SRC_FIFO:1;
        unsigned SRC_ASLP:1;
    };
    uint8_t data;
}INT_SOURCE_t;	// 0x0C -- pag 25

// FF_MT_CGF_t This is the freefall/motion configuration register for setting up the conditions of the freefall or motion function
typedef union
{
    struct
    {
        unsigned :1;
        unsigned :1;
        unsigned :1;
        unsigned XEFE:1;	// Event flag enable on X event. Default value: 0. (DISABLED)
        unsigned YEFE:1;	// Event flag enable on Y event. Default value: 0. (DISABLED)
        unsigned ZEFE:1;	// Event flag enable on Z event. Default value: 0. (DISABLED)
        unsigned OAE:1;		// Motion detect/freefall detect flag selection. Default 0 (Freefall flag) (AND combination)
        unsigned ELE:1;		/*Event latch enable: Event flags are latched into FF_MT_SRC register.
        						Reading of the FF_MT_SRC register clears the event flag EA and all FF_MT_SRC bits.
								0 (Default): Event flag latch disabled; 1: Event flag latch enabled*/
    };
    uint8_t data;
} FF_MT_CFG_t;

//FF_MT_SRC_t
typedef union
{
    struct
    {
        unsigned XHP:1;
        unsigned XHE:1;
        unsigned YHP:1;
        unsigned YHE:1;
        unsigned ZHP:1;
        unsigned ZHE:1;
        unsigned :1;
        unsigned EA:1;
    };
    uint8_t data;
} FF_MT_SRC_t;

// FF_MT_THS_t --> freefall and motion threshold register
typedef union
{
    struct
    {
        unsigned THS:7;		// Debounce counter mode selection. Default value: 0 (INCREMENTS OR DECREMENTS DEBOUNCE)
        unsigned DBCNTM:1; 	// Freefall/motion threshold: Default value: 000_0000
    };
    uint8_t data;
    /*Note: The threshold resolution is 0.063 g/LSB and the threshold register has a range of 0 to 127 counts.*/
} FF_MT_THS_t;

// FF_MT_COUNT_t --> This register sets the number of debounce sample counts for the event trigger.
typedef union
{
    struct
    {
        unsigned D:8;
    };
    uint8_t data;
    /*This register sets the minimum number of debounce sample counts of continuously
     * matching the detection condition user  selected for the freefall, motion event.*/
} FF_MT_COUNT_t;

// CTRL_REG1_t
typedef union
{
    struct
    {
        unsigned ACTIVE:1;		// Full-scale selection. Default value: 0 // (0: Standby mode; 1: Active mode)
        unsigned F_READ:1;		// Fast read mode: Data format limited to single byte default value: 0. // (0: Normal mode 1: Fast-read mode)
        unsigned LNOISE:1;		// Reduced noise reduced maximum range mode. Default value: 0 // (0: Normal mode; 1: Reduced noise mode)
        unsigned DR:3;			// DR[2:0] Data-rate selection. Default value: 000. (VER TABLA 63)
        unsigned ASLP_RATE:2;	// Configures the auto-wake sample frequency when the device is in sleep mode. Default value: 00. (VER TABLA 62)
    };
    uint8_t data;
}CTRL_REG1_t; // 0x2A -- pag 45 manual de referencia
// Note: Except for standby mode selection, the device must be in standby mode to change any of the fields within CTRL_REG1

// CTRL_REG4_t --> "Interrupt Enable Select"
typedef union
{
    struct
    {
        unsigned INT_EN_DRDY:1;		// Data-Ready Interrupt enable. Default value: 0 (disabled)
        unsigned :1;				// --
        unsigned INT_EN_FF_MT:1;	// Freefall/motion interrupt enable. Default value: 0 (disabled)
        unsigned INT_EN_PULSE:1;	// Pulse detection interrupt enable. Default value: 0 (disabled)
        unsigned INT_EN_LNDPRT:1;	// Orientation (landscape/portrait) int enable. Default 0 (disabled)
        unsigned INT_EN_TRANS:1;	// Transient interrupt enable. Default value: 0 (disabled)
        unsigned INT_EN_FIFO:1;		// FIFO Interrupt enable. Default value: 0 (disabled)
        unsigned INT_EN_ASLP:1;		// Auto-sleep/wake interrupt enable. Default value: 0 (disabled)
    };
    uint8_t data;
}CTRL_REG4_t;	// 0x2D -- pag 48 manual de referencia

// CTRL_REG5_t
typedef union
{
    struct
    {
        unsigned INT_CFG_DRDY:1;	// INT1/INT2 configuration. Default value: 0. // 0: Interrupt is routed to INT2 pin; 1: Interrupt is routed to INT1 pin
        unsigned :1;
        unsigned INT_CFG_FF_MT:1;	// INT1/INT2 configuration. Default value: 0. // 0: Interrupt is routed to INT2 pin; 1: Interrupt is routed to INT1 pin
        unsigned INT_CFG_PULSE:1;	// INT1/INT2 configuration. Default value: 0. // 0: Interrupt is routed to INT2 pin; 1: Interrupt is routed to INT1 pin
        unsigned INT_CFG_LNDPRT:1;	// INT1/INT2 configuration. Default value: 0. // 0: Interrupt is routed to INT2 pin; 1: Interrupt is routed to INT1 pin
        unsigned INT_CFG_TRANS:1;	// INT1/INT2 configuration. Default value: 0. // 0: Interrupt is routed to INT2 pin; 1: Interrupt is routed to INT1 pin
        unsigned INT_CFG_FIFO:1;	// INT1/INT2 configuration. Default value: 0. // 0: Interrupt is routed to INT2 pin; 1: Interrupt is routed to INT1 pin
        unsigned INT_CFG_ASLP:1;	// INT1/INT2 configuration. Default value: 0. // 0: Interrupt is routed to INT2 pin; 1: Interrupt is routed to INT1 pin
    };
    uint8_t data;
}CTRL_REG5_t; // 0x2E

/*==================[end of file]============================================*/
#endif /* MMA8451_H_ */
