/*
Copyright (C) <2020> <ETRI>

This License and Service Agreement (LSA) applies to all works and their derivative works based on source form version and object form version of IoTware Project. Currently, the LSA for IoTware Project has two policies, 'Open Source License' and 'Commercial License'. Therefore, all works including the source code and executable code of IoTware Project and derivative works based thereon are subject to either 'Open Source License' or 'Commercial License' depending on the user's needs and purpose. Details related to the selection of the applicable license are specified in this LSA. If you use any or all of IoTware Project in any form, you are deemed to have consented to this LSA. If you breach any of the terms and conditions set forth in this LSA, you are solely responsible for any losses or damages incurred by Electronics and Communications Research Institute (ETRI), and ETRI assume no responsibility for you or any third party.
If you use the source form version or object form version of IoTware Project in whole or in part to develop a code or a derivative work, and you want to commercialize the result in some form, you will be covered under a commercial license. And if you are subject to a commercial license, the contract for the use of IoTware Project is subject to TECHNOLOGY LICENSE AGREEMENT of ETRI. You acknowledge that ETRI has all legal rights, title and interest, including intellectual property rights in the IoTware Project (regardless of whether such intellectual property rights are registered or where such rights exist) and agree with no objection thereto. Except as provided in a subsidiary agreement, nothing in this LSA grants you the right to use IoTware Project or the name, service mark, logo, domain name and other unique identification marks of ETRI.
If you use the source form version or object form version of IoTware Project in whole or in part to develop a code or a derivative work, and you do not commercialize the result in any form, you will be covered under an open source license. IoTware Project is in accordance with Free Software Foundation (FSF)'s open source policy, and is allowed to use it in the appropriate scope and manner, and you must comply with the applicable open source license policy applied to IoTware Project. IoTware Project is, in principle, subject to GNU Lesser General Public License version 2.1 (LGPLv2.1). If you have acquired all or a part of the IoTware Project in any way and it is subject to a license other than the open source license described above, please contact the following address for the technical support and other inquiries before use, and check the usage information.
*/

/**
 * @file iw_fw_types_internal.h
 * @brief 내부 용 MicroService 주요 정의 헤더 파일
 * @details 내부 용 MicroService 주요 정의 헤더 파일
 */
#pragma once

#ifdef _WIN32
#include <conio.h>
#define EVENT uint8_t
#define STRING char *
#define AUTO void *
#else
// #include "wdk-debug.h"
// #include "wdk_sleep.h"

#include "iw_fw_stdmacros.h"
#endif

#include "iw_fw_ms_settings.h"


/**
 * @brief 파라미터 읽기/쓰기 퍼미션 속성
 */
typedef enum ePARAM_ATTR {
	NONE = 0,						/**< 읽기/쓰기 불가  */
	READ = 1 << 0,					/**< 읽기 가능  */
	WRITE = 1 << 1					/**< 쓰기 가능  */
} PARAM_ATTR;

/**
 * @brief MicroService/Task 상태
 */
typedef enum eSTATE_TYPE {
	NOT_READY = 0,					/**< NOT READY 혹은 오프로딩됨  */
	CREATED = 1 << 0,				/**< CREATED  */
	WAITING = CREATED,				/**< CREATED = WAITING  */
	STOPPED = 1 << 1,				/**< STOPPED  */
	RUNNING = 1 << 2,				/**< RUNNING  */
	ACTIVE = WAITING | RUNNING,		/**< ACTIVE 상태 = WAITING | RUNNING  */

	I_S_ACTIVE = NOT_READY,			/**< INITIAL_STATE ACTIVE  */
	I_S_READY = CREATED,			/**< INITIAL_STATE READY  */
	I_S_STOP = STOPPED				/**< INITIAL_STATE STOP  */
} STATE_TYPE;

#define IS_ACTIVE_STATE(state) ((state & ACTIVE) != 0)


#define _IW_LIST_IS_EMPTY(__list) ( IW_GET_LIST_NODE_NUM(__list) == 0 )
#define _IW_SET_LIST_NODE_DATA( node, pdata ) ( ( node )->pData = ( void * ) ( pdata ) )

#define LIST_FOREACH(__list,__itemname) \
	for ( \
		iw_list_node_t *__itemname = IW_GET_HEAD(__list); \
		__itemname; \
		__itemname = IW_GET_NEXT(__itemname) \
	)

#define LIST_FOREACH_TO_DEL(__list,__itemname) \
	for ( \
		iw_list_node_t *__itemname = IW_GET_HEAD(__list); \
		IW_GET_LIST_NODE_NUM(__list) != 0; \
		__itemname = IW_GET_HEAD(__list) \
	)

#define INFINITE_LOOP_START() \
	for (;;)

#define FOR_LOOP_START(__index_name,__range_max) \
	for (int __index_name = 0; __index_name < __range_max; __index_name++)
