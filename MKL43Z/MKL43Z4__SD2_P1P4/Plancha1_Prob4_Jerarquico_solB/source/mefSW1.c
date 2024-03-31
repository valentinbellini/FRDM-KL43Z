/*
###############################################################################
#
# Copyright 2023, Gustavo Muro
# All rights reserved
#
# This file is part of SISTEMAS DIGITALES 2 - FCEIA - UNR.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its
#    contributors may be used to endorse or promote products derived from this
#    software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#                                                                             */

/*==================[inclusions]=============================================*/
#include "mefSW1.h"
#include "SD2_board.h"
#include "key.h"

/*==================[macros and typedef]=====================================*/

typedef enum {
    EST_SW1_BLINK = 0,
    EST_SW1_ON,
}estMefSW1_enum;

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

static estMefSW1_enum estMefSW1;
static int32_t tim1msSW1;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

extern void mefSW1_init(void)
{
    mefSW1_reset();
}

extern void mefSW1_reset(void)
{
    estMefSW1 = EST_SW1_BLINK;
    tim1msSW1 = 500;
    board_setLed(BOARD_LED_ID_VERDE, BOARD_LED_MSG_OFF);
    board_setLed(BOARD_LED_ID_ROJO, BOARD_LED_MSG_OFF);
}

extern bool mefSW1(void)
{
    bool ret = 0;

    switch (estMefSW1)
    {
        case EST_SW1_BLINK:
            if (tim1msSW1 == 0)
            {
                tim1msSW1 = 500;
                board_setLed(BOARD_LED_ID_VERDE, BOARD_LED_MSG_TOGGLE);
                board_setLed(BOARD_LED_ID_ROJO, BOARD_LED_MSG_TOGGLE);
            }
            if (key_getPressEv(BOARD_SW_ID_1))
            {
                board_setLed(BOARD_LED_ID_VERDE, BOARD_LED_MSG_ON);
                board_setLed(BOARD_LED_ID_ROJO, BOARD_LED_MSG_ON);
                estMefSW1 = EST_SW1_ON;
            }
            break;

        case EST_SW1_ON:
            if (key_getPressEv(BOARD_SW_ID_1))
            {
                estMefSW1 = EST_SW1_BLINK;
                ret = 1;
            }
            break;
        }

        return ret;
}

extern void mefSW1_task1ms(void)
{
    if (tim1msSW1)
        tim1msSW1--;
}


/*==================[end of file]============================================*/
