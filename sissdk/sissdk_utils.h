/**
 * @file    __SISSDK_UTILS.h
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
 
#ifndef __SISSDK_UTILS_H
#define __SISSDK_UTILS_H 		

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define SISSDK_LOG_ENABLE			//disbale /Enable 

#ifdef SISSDK_LOG_ENABLE

	typedef enum e_SisLogType
	{
		LogInfo,
		LogDebug,
		LogWarn,
		LogExcep,			//Exception
		LogError,
		
		logTypeMax
	}e_SisLogType;

	static const char A_SisLogType[logTypeMax] = 
	{
		'I','D','W','E','F'
	};
		
	#define sis_log(log,format,...)	printf("[%c]:"format,\
	A_SisLogType[log],\
	##__VA_ARGS__)
	#define sis_printf printf	
#else
	#define sis_log(log,format,...)
	#define sis_printf(format,...)
#endif
	
void util_dump_buf( const char *title, unsigned char *buf, size_t len );

#ifdef __cplusplus
}
#endif

#endif





























