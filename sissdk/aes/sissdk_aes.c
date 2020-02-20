/**********************************************************************
 *
 * Filename:    aes_wrap.c
 *
 * Description: Fast use of AES128 CBC
 *
 * Notes:       The parameters for each supported AES standard are
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
 * @file    aes_wrap.c
 * @brief   Implementation of aes_wrap.h
 */
 
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "sissdk_aes.h"

static mbedtls_aes_context aes_context;


/**
 * \brief  This function performs an AES-CBC encryption or decryption operation
 *         on full blocks.
 *
 *         It performs the operation defined in the \p mode
 *         parameter (encrypt/decrypt), on the input data buffer defined in
 *         the \p input parameter.
 *
 *         It can be called as many times as needed, until all the input
 *         data is processed. mbedtls_aes_init(), and either
 *         mbedtls_aes_setkey_enc() or mbedtls_aes_setkey_dec() must be called
 *         before the first call to this API with the same context.
 *
 * \note   This function operates on full blocks, that is, the input size
 *         must be a multiple of the AES block size of \c 16 Bytes.
 *
 * \note   Upon exit, the content of the IV is updated so that you can
 *         call the same function again on the next
 *         block(s) of data and get the same result as if it was
 *         encrypted in one call. This allows a "streaming" usage.
 *         If you need to retain the contents of the IV, you should
 *         either save it manually or use the cipher module instead.
 *
 *
 * \param key      Initialization key (updated after use).
 * \param iv       Initialization vector (updated after use).
 *                 It must be a readable and writeable buffer of \c 16 Bytes.
 * \param input    The buffer holding the input data.
 *                 It must be readable and of size \p length Bytes.
 * \param output   The buffer holding the output data.
 *                 It must be writeable and of size \p length Bytes.
 * \param length   The length of the input data in Bytes. This must be a
 *                 multiple of the block size (\c 16 Bytes).
 *
 * \return         \c 0 on success.
 * \return         #MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH
 *                 on failure.
 */

bool decrypt_buffer_cbc(uint8_t * key, uint8_t * iv,uint8_t * input, uint32_t length,uint8_t * output)
{	
	mbedtls_aes_init(&aes_context);
	mbedtls_aes_setkey_dec(&aes_context,key,AES_KEY_SIZE);
	
	return mbedtls_aes_crypt_cbc( &aes_context,
                    MBEDTLS_AES_DECRYPT,
                    length,
                    iv,
                    input,
                    output ) == 0;
}



/**
 * \brief  This function performs an AES-CBC encryption or decryption operation
 *         on full blocks.
 *
 *         It performs the operation defined in the \p mode
 *         parameter (encrypt/decrypt), on the input data buffer defined in
 *         the \p input parameter.
 *
 *         It can be called as many times as needed, until all the input
 *         data is processed. mbedtls_aes_init(), and either
 *         mbedtls_aes_setkey_enc() or mbedtls_aes_setkey_dec() must be called
 *         before the first call to this API with the same context.
 *
 * \note   This function operates on full blocks, that is, the input size
 *         must be a multiple of the AES block size of \c 16 Bytes.
 *
 * \note   Upon exit, the content of the IV is updated so that you can
 *         call the same function again on the next
 *         block(s) of data and get the same result as if it was
 *         encrypted in one call. This allows a "streaming" usage.
 *         If you need to retain the contents of the IV, you should
 *         either save it manually or use the cipher module instead.
 *
 *
 * \param key      Initialization key (updated after use).
 * \param iv       Initialization vector (updated after use).
 *                 It must be a readable and writeable buffer of \c 16 Bytes.
 * \param input    The buffer holding the input data.
 *                 It must be readable and of size \p length Bytes.
 * \param output   The buffer holding the output data.
 *                 It must be writeable and of size \p length Bytes.
 * \param length   The length of the input data in Bytes. This must be a
 *                 multiple of the block size (\c 16 Bytes).
 *
 * \return         \c 0 on success.
 * \return         #MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH
 *                 on failure.
 */

bool encrypt_buffer_cbc(uint8_t * key, uint8_t * iv,uint8_t * input, uint32_t length,uint8_t * output)
{
		mbedtls_aes_init(&aes_context);
		mbedtls_aes_setkey_enc(&aes_context,key,AES_KEY_SIZE);
	
		return mbedtls_aes_crypt_cbc( &aes_context,
                    MBEDTLS_AES_ENCRYPT,
                    length,
                    iv,
                    input,
                    output ) == 0;
}

