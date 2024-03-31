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
#include "mefSec.h"
#include "SD2_board.h"

/*==================[macros and typedef]=====================================*/
typedef enum {
    EST_SEC_1 = 0,
    EST_SEC_2,
    EST_SEC_3,
} estMefSec_enum;

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

static estMefSec_enum estMefSec;
static int32_t tim1msSec;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

extern void mefSec_init(void)
{
    estMefSec = EST_SEC_1;
    tim1msSec = 2000;
}

extern void mefSec(void)
{
    switch (estMefSec)
    {
        case EST_SEC_1:
            board_setLed(BOARD_LED_ID_VERDE, BOARD_LED_MSG_OFF);
            board_setLed(BOARD_LED_ID_ROJO, BOARD_LED_MSG_ON);
            if (tim1msSec == 0)
            {
                tim1msSec = 2000;
                estMefSec = EST_SEC_2;
            }
            break;

        case EST_SEC_2:
            board_setLed(BOARD_LED_ID_VERDE, BOARD_LED_MSG_ON);
            board_setLed(BOARD_LED_ID_ROJO, BOARD_LED_MSG_OFF);
            if (tim1msSec == 0)
            {
                tim1msSec = 2000;
                estMefSec = EST_SEC_3;
            }
            break;

        case EST_SEC_3:
            board_setLed(BOARD_LED_ID_VERDE, BOARD_LED_MSG_ON);
            board_setLed(BOARD_LED_ID_ROJO, BOARD_LED_MSG_ON);
            if (tim1msSec == 0)
            {
                tim1msSec = 2000;
                estMefSec = EST_SEC_1;
            }
            break;
    }
}

extern void mefSec_task1ms(void)
{
    if (tim1msSec)
        tim1msSec--;
}


/*==================[end of file]============================================*/
