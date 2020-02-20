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
 
#ifndef __SISSDK_API_H
#define __SISSDK_API_H 		

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "sissdk_crc.h"
#include "sissdk_aes.h"
#include "sissdk_ecdsa.h"
#include "sissdk_benchmark.h"
#include "sissdk_utils.h"

#pragma	anon_unions


//prefix header

#define SIS_CMD_PREFIX_HEAD 	 "SLSTA:"
#define SIS_CMD_ENDFIX_HEAD 	 ":SLEND"

#define SIS_CMD_HEAD_SIZE			 (sizeof(SIS_CMD_PREFIX_HEAD)-1)	//strlen - 1, no \0
#define SIS_CMD_TAIL_SIZE			 (sizeof(SIS_CMD_ENDFIX_HEAD)-1)	//strlen - 1, no \0

#define SLS_PROJECT_SIZE		 		17			
#define SLS_SN_SIZE	 				 		17
#define SLS_HWVER_SIZE	 		 		6	
#define	SLS_FWVER_SIZE			 		6
#define	SLS_UID_SIZE 				 		12

#define SLS_KEY_SIZE				 		17		

#define SLS_PACKAGE_SIZE		 		256		//packet size
#define SLS_EXTDATA_MAX_SIZE		1024	//max extData size
#define SLS_LICMAX_SIZE					1024	//max lic DataSize


//cmd
typedef enum E_SlsCmd
{
	cmdNull					=	0x0000,		//Null Command ,For beachmark /heart beat etc
	
	// SLS Device cmd
	cmdDeviceFind,							
	cmdDeviceInfo,						
	cmdDevicePair,						
	cmdDevicePairResp,						
	cmdDeviceUnPair,		
	
	// SLS Device config
	cmdDeviceConfig,						
	cmdDeviceFwStart,						
	cmdDeviceFwPkgSend,						
	cmdDeviceFwPkgEnd,		
	cmdDeviceExtDatSet,	
	cmdDeviceExtDatReq,
	cmdDeviceExtDatResp,
	cmdDeviceLicLogReq,
	cmdDeviceLicLogResp,
	
	//SLS Device License
	cmdChipLicenseReq,		
	cmdChipLicenseResp,				
	cmdChipLicenseConfirm,	
	
	cmdOK						=	0xFFFE,	
	cmdError				=	0xFFFF		
}E_SlsCmd;

//Error code  
typedef enum	E_SlsErrcode
{	
	cmdErrorUnKnow		= 0x0000,
	cmdErrorCRC32,
	cmdErrorProject,
	cmdErrorBusy,
	cmdErrrorPaired,
	cmdErrorHander,
	cmdErrorConfigUpdate,
	cmdErrorFWUpdate,
	cmdErrorUIDInfo,
	cmdErrorLicDisabled,
	cmdErrorLicNoLeft,
	cmdErrorUnPaired,
	cmdErrorInterError,
	cmdErrorMemory,
	cmdErrorTimeOut,
	cmdErrorSize,
	cmdErrorCmd,
	cmdErrorParams,
	cmdErrorPermissions,
	cmdErrorDisable,
	cmdErrorDefaultPassword,
	
	cmdErrorMax
}E_SlsErrcode;

//Error infomation 
static const char * const D_SlsErrcode[cmdErrorMax] = {
	"Unknown error",
	"CRC error",
	"Project mismatch error",
	"Busy now!",
	"No paired error",
	"No handler error",
	"Update config error/writting",
	"Update firmware error/writting",
	"Uid illegal error ",
	"Authorization has been disabled",
	"The authorization has been exhausted",
	"The device was not paired",
	"Device internal exception error",
	"Device out of memory",
	"Time out",
	"Error size",
	"Error cmd",
	"Parameter error",
	"Permissions error",
	"Error funtion Disabled!"
	"Error default password!"
	
};


//cmdNull,etc
typedef struct S_SlsPayLoadHeader
{
	uint32_t 		 m_handler;
	uint8_t		 m_project[SLS_PROJECT_SIZE];
}
S_SlsPayLoadHeader,
S_SlsCmdNull,
S_SlsCmdOK
;

//error command
typedef struct	S_SlsCmdError
{
	S_SlsPayLoadHeader		m_header;
	uint16_t							m_errCode;
}S_SlsCmdError;


//Device function ON/OFF
typedef enum 		E_SlsEnable
{
	disable,
	enable
}
E_SlsLicEn,
E_SlsUidLimitEn,
E_SlsFwUpdateEn,
E_SlsExtDatWriteEn,
E_SlsExtDatReadEn,
E_SlsLogEn
;

//Device info
typedef struct	S_SlsCmdDeviceInfo
{
	S_SlsPayLoadHeader		m_header;
	
	uint8_t							m_sn[SLS_SN_SIZE];
	uint8_t							m_hwVer[SLS_HWVER_SIZE];
	uint8_t							m_fwVer[SLS_FWVER_SIZE];
	
	E_SlsLicEn						m_licEnable;
	E_SlsUidLimitEn				m_uidLimitEnable;
	E_SlsFwUpdateEn				m_fwUpdateEn;
	E_SlsExtDatWriteEn		m_fwExtDatWriteEn;
	E_SlsExtDatReadEn			m_fwExtDatReadEn;
	E_SlsLogEn						m_logEnable;
	
	uint32_t							m_licTotal;
	uint32_t							m_licLeft;
	uint8_t								m_uidLimitLwr[SLS_UID_SIZE];
	uint8_t								m_uidLimitUpr[SLS_UID_SIZE];
	
}S_SlsCmdDeviceInfo;

//Device info Config
typedef struct	S_SlsCmdDeviceConfig
{
	S_SlsPayLoadHeader		m_header;
	
	uint8_t		 					m_newProject[SLS_PROJECT_SIZE];
	
	uint8_t							m_newSn[SLS_SN_SIZE];
	uint8_t							m_newKey[SLS_KEY_SIZE];
	uint8_t							m_newIv[SLS_KEY_SIZE];
	
	E_SlsLicEn						m_licEnable;
	E_SlsUidLimitEn				m_uidEnable;
	E_SlsFwUpdateEn				m_fwUpdateEn;
	E_SlsExtDatWriteEn		m_fwExtDatWriteEn;
	E_SlsExtDatReadEn			m_fwExtDatReadEn;
	E_SlsLogEn						m_logEnable;
	
	uint32_t							m_licTotal;
	uint8_t								m_uidLimitLwr[SLS_UID_SIZE];
	uint8_t								m_uidLimitUpr[SLS_UID_SIZE];
	
	uint16_t							m_ConfigTimeLeft;				//default config one time
}S_SlsCmdDeviceConfig;

//Device Pair/unPair
typedef S_SlsPayLoadHeader S_SlsCmdDevicePairResp;
typedef S_SlsPayLoadHeader S_SlsCmdDeviceFind;

//Device Pair req
typedef enum 		E_SlsReqFlag
{
		normal,
		force
}E_SlsReqFlag;

//Device unPair req
typedef struct	S_SlsCmdDevicePair
{
	S_SlsPayLoadHeader		m_header;
	E_SlsReqFlag					m_flag;
}
S_SlsCmdDevicePair,
S_SlsCmdDeviceUnPair;


//Device update Fw req
typedef struct	S_SlsCmdDeviceFwStart
{
	S_SlsPayLoadHeader		m_header;
	uint32_t							m_size;
	uint32_t							m_crc32;
}
S_SlsCmdDeviceFwStart;

//Device update
typedef struct	S_SlsCmdDeviceFwPkgSend
{
	S_SlsPayLoadHeader		m_header;
	uint32_t							m_size;
	uint8_t								m_data[SLS_PACKAGE_SIZE];
}
S_SlsCmdDeviceFwPkgSend;

//Device update end
typedef S_SlsPayLoadHeader S_SlsCmdDeviceFwPkgEnd;

//Device ExtDataSet
typedef struct	S_SlsCmdDeviceExtDatSet
{
	S_SlsPayLoadHeader		m_header;
	uint32_t							m_flashAddr;
	uint32_t							m_size;
	uint8_t								m_data[SLS_EXTDATA_MAX_SIZE];
}
S_SlsCmdDeviceExtDatSet;

//Device ExtDataGet
typedef S_SlsPayLoadHeader S_SlsCmdDeviceExtDatReq;
typedef struct	S_SlsCmdDeviceExtDatResp
{
	S_SlsPayLoadHeader		m_header;
	uint32_t							m_size;
	uint8_t								m_data[SLS_EXTDATA_MAX_SIZE];
}
S_SlsCmdDeviceExtDatResp;

//Device LIC Log Get
typedef S_SlsPayLoadHeader S_SlsCmdDeviceLicLogReq;
typedef struct	S_SlsCmdDeviceLicLogResp
{
	S_SlsPayLoadHeader		m_header;
	
	uint32_t							m_licensedCnt;
	uint32_t							m_licensedOk;
	uint32_t							m_licenseErr;
	uint32_t							m_licenseUnknow;
}
S_SlsCmdDeviceLicLogResp;


//Device LIC req
typedef struct	S_SlsCmdChipLicenseReq
{
	S_SlsPayLoadHeader		m_header;
	
	uint16_t							m_uidSize;
	uint8_t								m_uidDat[SLS_UID_SIZE];
}
S_SlsCmdChipLicenseReq;

//Device LIC req
typedef struct	S_SlsCmdChipLicenseResp
{
	S_SlsPayLoadHeader		m_header;
	
	uint16_t							m_licSize;
	uint8_t								m_licDat[SLS_LICMAX_SIZE];
}
S_SlsCmdChipLicenseResp;

//Device LIC confirm
typedef enum 		s_SlsStatus
{
	Ok,
	Error
}s_SlsStatus;

typedef struct	S_SlsCmdChipLicenseConfirm
{
	S_SlsPayLoadHeader		m_header;
	
	s_SlsStatus						m_Status;
}S_SlsCmdChipLicenseConfirm;

//command Packet struct
typedef struct 	S_SlsPackage
{
	uint8_t				m_Header[SIS_CMD_HEAD_SIZE];
	uint16_t			m_RandFactor;
	
	//cmd
	union{
		E_SlsCmd			m_Cmd;
		uint16_t			m_CmdU16;
	};
	
	uint32_t			m_TotalSize;
	uint32_t			m_ActiveSize;
	union{
				S_SlsCmdNull											m_cmdNULL;
				
				// SLS Device cmd
				S_SlsCmdDeviceFind								m_cmdDeviceFind;						
				S_SlsCmdDeviceInfo								m_cmdDeviceInfo;
				S_SlsCmdDevicePair								m_cmdDevicePair;
				S_SlsCmdDevicePairResp						m_cmdDevicePairResp;					
				S_SlsCmdDeviceUnPair							m_cmdDeviceUnPair;			
				
				// SLS Device config
				S_SlsCmdDeviceConfig							m_cmdDeviceConfig;
				S_SlsCmdDeviceFwStart							m_cmdDeviceFwStart;						
				S_SlsCmdDeviceFwPkgSend						m_cmdDeviceFwPkgSend;						
				S_SlsCmdDeviceFwPkgEnd						m_cmdDeviceFwPkgEnd;		
				S_SlsCmdDeviceExtDatSet						m_cmdDeviceExtDatSet;
				S_SlsCmdDeviceExtDatReq						m_CmdDeviceExtDatReq;
				S_SlsCmdDeviceExtDatResp					m_cmdDeviceExtDatResp;
				S_SlsCmdDeviceLicLogReq						m_cmdDeviceLicLogReq;		
				S_SlsCmdDeviceLicLogResp					m_cmdDeviceLicLogResp;	
				
				//SLS Device License
				S_SlsCmdChipLicenseReq						m_cmdChipLicenseReq;
				S_SlsCmdChipLicenseResp						m_cmdChipLicenseResp;				
				S_SlsCmdChipLicenseConfirm				m_cmdChipLicenseConfirm;
				
				S_SlsCmdOK												m_cmdOK;
				S_SlsCmdError											m_cmdError;
	}m_Payload;
	
	uint32_t			m_Crc32;
	uint8_t				m_Tailer[SIS_CMD_TAIL_SIZE];
}SLS_cmdPackage;

#ifdef __cplusplus
}
#endif

#endif





























