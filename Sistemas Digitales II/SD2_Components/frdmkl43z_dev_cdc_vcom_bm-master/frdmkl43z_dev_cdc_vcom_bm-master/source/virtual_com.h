/*
 ###############################################################################
 # The Clear BSD License
 # Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 # Copyright 2016 - 2017 NXP
 # All rights reserved.
 #
 # This file is part of SISTEMAS DIGITALES 2 - FCEIA - UNR.
 #
 # Redistribution and use in source and binary forms, with or without modification,
 # are permitted (subject to the limitations in the disclaimer below) provided
 # that the following conditions are met:
 #
 # o Redistributions of source code must retain the above copyright notice, this list
 #   of conditions and the following disclaimer.
 #
 # o Redistributions in binary form must reproduce the above copyright notice, this
 #   list of conditions and the following disclaimer in the documentation and/or
 #   other materials provided with the distribution.
 #
 # o Neither the name of the copyright holder nor the names of its
 #   contributors may be used to endorse or promote products derived from this
 #   software without specific prior written permission.
 #
 # NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 # THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 # ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 # WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 # DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 # ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 # (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 # LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 # ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 # (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 # SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 #                                                                             */

#ifndef _USB_CDC_VCOM_H_
#define _USB_CDC_VCOM_H_ 1

/*==================[inclusions]=============================================*/

/*==================[macros and typedef]=====================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

/*!
 * @brief Application initialization function.
 *
 * This function initializes the application.
 *
 * @return None.
 */
void virtual_com_init(void);

/** \brief envía datos por puerto USB emulando puerto serie COM
 **
 ** \param[inout] pBuf buffer a donde estan los datos a enviar
 ** \param[in] size tamaño del buffer
 ** \return cantidad de bytes enviados
 **/
int32_t virtual_com_send(uint8_t *pBuf, int32_t size);

/** \brief recibe datos por puerto USB emulando puerto serie COM
 **
 ** \param[inout] pBuf buffer a donde guardar los datos
 ** \param[in] size tamaño del buffer
 ** \return cantidad de bytes recibidos. Si retorna -1 la cantidad
 ** de bytes recibidas es mayor al buffer de recepción utilizado.
 **/
int32_t virtual_com_recv(uint8_t *pBuf, int32_t size);

/*==================[end of file]============================================*/
#endif /* _USB_CDC_VCOM_H_ */
