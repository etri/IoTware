/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
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
