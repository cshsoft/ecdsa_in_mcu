/**********************************************************************
 *
 * Filename:    sissdk_utils.c
 *
 * Description: Fast use sisapi_sdk
 *
 * Notes:       The parameters for each supported sisapi standard are
 *				defined in the header file aes.h.  The implementations
 *				here should stand up to further additions to that list.
 *
 *
 * Copyright (c) 2019 by ICWORKSHOP.  This software is placed into
 * the public domain and may be used for any purpose.  However, this
 * notice must not be changed or removed and no warranty is either
 * expressed or implied by its publication or distribution.
 **********************************************************************/

/**
 * @file    sis_benchmark.c
 * @brief   Implementation of sis_benchmark.h
 */
#include "sissdk_utils.h"
#include <stdio.h>
#include "stm32g0xx_hal.h"
#include "stm32g0xx_ll_system.h"
#include "string.h"
#include "stdlib.h"
#include "mbedtls/entropy.h"

#ifdef SISSDK_LOG_ENABLE

extern UART_HandleTypeDef huart2;


int fputc(int ch, FILE *f)
{
		uint8_t ch8 = (uint8_t)ch;
		HAL_UART_Transmit(&huart2,&ch8,sizeof(ch8),5);
		return (ch);
}

int fgetc(FILE *f)
{		
		uint8_t ch8;
		if(HAL_OK == HAL_UART_Receive(&huart2,&ch8,sizeof(ch8),UINT16_MAX))
			return ch8;
		return 0;
}

#endif

/**
 * \brief  Print the data to the console
 *
 * \param title  	  The prefix label
 * \param buf      	The data used to validate the certificate
 * \param len			  data size
 *                 
 *
 * \return          void
 */

void util_dump_buf( const char *title, unsigned char *buf, size_t len )
{
    size_t i;
    sis_printf("[%d]%s", len,title);
    for( i = 0; i < len; i++ )
        sis_printf("%c%c", "0123456789ABCDEF" [buf[i] / 16],"0123456789ABCDEF" [buf[i] % 16] );
    sis_printf("\r\n");
}


#if defined(MBEDTLS_ENTROPY_HARDWARE_ALT)

static int util_get_random(unsigned char *buf, size_t len)
{
    int i, j;
    unsigned long tmp;

    for (i = 0; i < ((len + 3) & ~3) / 4; i++) {
        tmp = rand();

        for (j = 0; j < 4; j++) {
            if ((i * 4 + j) < len) {
                buf[i * 4 + j] = (unsigned char)(tmp >> (j * 8));
            } else {
                break;
            }
        }
    }

    return 0;
}

int mbedtls_hardware_poll(void *data, unsigned char *output, size_t len, size_t *olen)
{
    util_get_random(output, len);
    *olen = len;

    return 0;
}

#endif


