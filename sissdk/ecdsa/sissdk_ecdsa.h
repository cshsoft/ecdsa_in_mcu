/**
 * @file    Api.h
 * @brief   ICWorkShop Safety License Shield Api Functions
 *
 * Safety License Shield
 * Copyright (c) 2009-2017, ICWorkshop Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#ifndef __SISSDK_ECDSA_H
#define __SISSDK_ECDSA_H 		

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "mbedtls/config.h"
#include "mbedtls/platform.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/ecdsa.h"
#include "mbedtls/sha256.h"

#define SHA256_HASH_SIZE						32
#define CERTIFICATION_ID_SIZE       3
#define CERTIFICATION_PUB_SIZE      49
#define CERTIFICATION_PRI_SIZE      24
#define CERTIFICATION_SIG_MAX				128

#ifdef __cplusplus
extern "C" {
#endif


#pragma pack(push,1)
typedef struct S_ECDSA_CARTICATE
{
    bool        __id;
    uint8_t     m_id[CERTIFICATION_ID_SIZE];
    bool        __pri;
    uint8_t     m_pri[CERTIFICATION_PRI_SIZE];
    bool        __pub;
    uint8_t     m_pub[CERTIFICATION_PUB_SIZE];
}S_ECDSA_CARTICATE;
#pragma pack(pop)  


bool sissdk_ecdsa_verify(uint8_t * pubKey,uint8_t * src,uint32_t src_size,uint8_t * sig,size_t sig_len);			//Generate a certificate
bool sissdk_ecdsa_sign(uint8_t * priKey,uint8_t * src,uint32_t src_size,uint8_t * sig,size_t * sig_len);				//Verify a certificate

#ifdef __cplusplus
}
#endif

#endif


