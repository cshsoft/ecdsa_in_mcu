/**********************************************************************
 *
 * Filename:    sissdk_ecdsa.c
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
#include "sissdk_ecdsa.h"
#include "sissdk_utils.h"

static uint8_t	m_hash[SHA256_HASH_SIZE];

/**
 * \brief  Generate a Hash for the data
 *
 * \param src      Data pointer to the hash is required
 * \param size     Data size
 *                 
 *
 * \return         true on success.
 * \return         false on failure.
 */
 
uint8_t * sissdk_ecdsa_sha256(uint8_t * src,uint32_t size)
{
	int ret = -1;
	if(src == NULL) {
		sis_log(LogError,"src data is NULL\r\n");
		return NULL;
	}
	if ((ret = mbedtls_sha256_ret(src, size, m_hash, 0)) != 0)
	{
			sis_log(LogError,"[failed]: mbedtls_sha256_ret returned %d\r\n", ret);
			return NULL;
	}
	util_dump_buf("[ok]: Hash = ", m_hash, sizeof(m_hash));
	return m_hash;
}

/**
 * \brief  Load the secret key
 *
 * \param keyPair  The public or private key generated by ecdsa.exe
 * \param ctx      The context of the ecdsa
 *                 
 *
 * \return         true on success.
 * \return         false on failure.
 */

bool sissdk_ecdsa_loadKeyPair(S_ECDSA_CARTICATE * keyPair,mbedtls_ecdsa_context * ctx)
{
		const uint8_t * group = keyPair->m_id;
    int ret;
    if (keyPair->__id) 
		{
        if ( 0 != (ret = mbedtls_ecp_tls_read_group(&ctx->grp, &group, sizeof(keyPair->m_id))))
				{
            sis_log(LogError,"[failed]: mbedtls_ecp_tls_read_group: %d\r\n", ret);
            return false;
        }
    }
    if (keyPair->__pub)
		{
        if (0 != (ret = mbedtls_ecp_point_read_binary(&ctx->grp, &ctx->Q, keyPair->m_pub, sizeof(keyPair->m_pub))))
				{
            sis_log(LogError,"[failed]: mbedtls_ecp_point_read_binary: %d\r\n", ret);
            return false;
        }
    }
    if (keyPair->__pri) 
		{
        if (0 != (ret = mbedtls_mpi_read_binary(&ctx->d, keyPair->m_pri, sizeof(keyPair->m_pri))))
				{
            sis_log(LogError,"[failed]: mbedtls_mpi_read_binary: %d\r\n", ret);
            return false;
        }
    }
    return true;
}

/**
 * \brief  Certificate to verify data
 *
 * \param pubKey  	The public key generated by ecdsa.exe
 * \param src      	The data used to validate the certificate
 * \param src_size  data size
 * \param sig			  Certificate of data
 *                 
 *
 * \return         true on success.
 * \return         false on failure.
 */

bool sissdk_ecdsa_verify(uint8_t * pubKey,uint8_t * src,uint32_t src_size,uint8_t * sig,size_t sig_len)
{
		uint8_t * pHash = NULL;
	
    if(NULL  == (pHash = sissdk_ecdsa_sha256(src,src_size))){
			return false;
		}
		
		mbedtls_ecdsa_context ctx_verify;
		
		mbedtls_ecdsa_init(&ctx_verify);
		if(!sissdk_ecdsa_loadKeyPair((S_ECDSA_CARTICATE *)pubKey,&ctx_verify)){
			mbedtls_ecdsa_free(&ctx_verify);
			return false;
		}
		
		sis_log(LogDebug,"[-v]Preparing verification context...\r\n");
		
		int ret;
    if ((ret = mbedtls_ecdsa_read_signature(&ctx_verify,
        pHash, SHA256_HASH_SIZE,
        sig, sig_len)) != 0)
    {
        sis_log(LogError,"[failed]:mbedtls_ecdsa_genkey returned %d\r\n", ret);
				mbedtls_ecdsa_free(&ctx_verify);
				return false;
    }
    sis_log(LogDebug,"[ok]: verification passed...\r\n");
    mbedtls_ecdsa_free(&ctx_verify);
		
		return true;
}

/**
 * \brief  Generate certificates for the data
 *
 * \param priKey  	The private key generated by ecdsa.exe
 * \param src      	The data used to validate the certificate
 * \param src_size  data size
 * \param sig			  Certificate of data (output)
 *                 
 *
 * \return         true on success.
 * \return         false on failure.
 */
bool sissdk_ecdsa_sign(uint8_t * priKey,uint8_t * src,uint32_t src_size,uint8_t * sig,size_t * sig_len)
{
		uint8_t * pHash = NULL;
    if(NULL  == (pHash = sissdk_ecdsa_sha256(src,src_size))){
			return false;
		}
		
		mbedtls_ecdsa_context ctx_sign;
    
		mbedtls_ecdsa_init(&ctx_sign);
		if(!sissdk_ecdsa_loadKeyPair((S_ECDSA_CARTICATE *)priKey,&ctx_sign)){
			mbedtls_ecdsa_free(&ctx_sign);
			return false;
		}
		
		sis_log(LogDebug,"[-s]Signing src hash...\r\n");
		int ret;
		
    if ((ret = mbedtls_ecdsa_write_signature(&ctx_sign, MBEDTLS_MD_SHA256,
        pHash, SHA256_HASH_SIZE,
        sig, sig_len,
        NULL, NULL)) != 0)
    {
				sis_log(LogError,"[failed]:mbedtls_ecdsa_genkey returned %d\r\n", ret);
				mbedtls_ecdsa_free(&ctx_sign);
        return false;
    }
    mbedtls_printf("[ok]: ok (signature length = %u)\r\n", *sig_len);
		util_dump_buf("[ok]: Signature = ", sig, *sig_len);
		
		mbedtls_ecdsa_free(&ctx_sign);
		return true;
}



