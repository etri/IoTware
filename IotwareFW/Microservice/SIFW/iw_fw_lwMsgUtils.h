/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

/**
 * @file iw_fw_lwMsgUtils.h
 * @brief 경량형 메시지 유틸리티 헤더
 * @details 경량형 메시지의 필수 데이터, 혹은 유틸리티 함수/매크로 정의 헤더
 */
#pragma once

#include "iw_fw_types_internal.h"


enum interfaceIDs
{
	eLWM_IF_DEV_REG = 1,
	eLWM_IF_DEV_MS_INFO_REPORT,
	eLWM_IF_DEV_TASK_INFO_REQUEST,
	// RESERVED
	eLWM_IF_MS_CREATION = 10,
	eLWM_IF_MS_RUN,
	eLWM_IF_MS_INPUT_PARAM_SET,
	eLWM_IF_MS_OUTPUT_PARAM_READ,
	eLWM_IF_MS_OUTPUT_REPORT,
	eLWM_IF_MS_STOP,
	eLWM_IF_MS_DELETE,
	// RESERVED
	eLWM_IF_TASK_ON = 20,
	eLWM_IF_TASK_PARAM_SET,
	eLWM_IF_TASK_PARAM_READ,
	eLWM_IF_TASK_PARAM_READ_ALL,
	eLWM_IF_TASK_OFF,
	// RESERVED
};

enum responseCodes
{
	eLWM_RSC_SUCCESS = 200,			// Request received and processed
	eLWM_RSC_MS_CREATED = 201,		// New Microservice Created
	eLWM_RSC_ACCEPTED =202,			// Request received and waiting for process
	// RESERVED
	eLWM_RSC_BAD_REQ = 400,			// Error in Interface ID, etc..
	eLWM_RSC_UNAUTH = 401,			// 
	eLWM_RSC_FORBIDDEN = 403,		// Uncontrollable parameters, tasks or microservices
	eLWM_RSC_NOTFOUND = 404,		// Non-existent parameter, tasks or microservices
	eLWM_RSC_NOMCMSG = 405,			// No Microservice Control MSG type
	eLWM_RSC_TOO_MANY_REQ = 429,
	// RESERVED
	eLWM_RSC_SERVER_ERR = 500,
	// RESERVED
};



/**
 * @brief 파라미터 데이터 타입
 */
typedef enum ePARAM_TYPE {
	eP_INT8		= 1 << 0,			/**< int8_t  */
	eP_UINT8	= 1 << 1,			/**< uint8_t  */
	eP_INT16	= 1 << 2,			/**< int16_t  */
	eP_UINT16	= 1 << 3,			/**< uint16_t  */
	eP_INT32	= 1 << 4,			/**< int32_t  */
	eP_UINT32	= 1 << 5,			/**< uint32_t  */
	eP_FLOAT	= 1 << 6,			/**< float  */
	eP_DOUBLE,						/**< double  */
	eP_STRING,						/**< 문자열  */

	eP_DECIMAL = 0x3F,				/**< (eP_INT8 | eP_UINT8 | eP_INT16 | eP_UINT16 | eP_INT32 | eP_UINT32) */
	eP_UNKNOWN = 0xFFFF,			/**< 불명(초기값)  */
} PARAM_TYPE;

#define IS_DECIMAL(d) ((d) == (d & eP_DECIMAL))


// private
/**
 * @brief 경량형 메시지 데이터 타입
 */
typedef enum eLWMSG_DATA_TYPE {
	eLWM_Unknown = 0,				/**< 미정의(기본)  */
	eLWM_Number = 1 << 0,			/**< 숫자  */
	eLWM_String = 1 << 1,			/**< 문자열  */
	eLWM_Array = 1 << 2,			/**< Array  */
	eLWM_Dictionary = 1 << 3,		/**< Dictionary  */
} LWMSG_DATA_TYPE;

// private
/**
 * @brief 숫자 값을 저장하는 경량형 메시지 Object
 */
typedef struct stLightWeightMessageNumber {
	void *value;					/**< 데이터  */
	enum ePARAM_TYPE type;			/**< 숫자의 데이터 타입  */
} LWMNumber, *PLWMNumber;

// private
typedef iw_list_t LWMArray;			// list of struct stLightWeightMessageObject
typedef iw_list_t *PLWMArray;
typedef const iw_list_t *LPCLWMArray;

// public
/**
 * @brief Object를 저장하는 경량형 메시지 Object
 */
typedef struct stLightWeightMessageObject {
	void *value;					/**< Number(PLWMNumber), String(char *), Array(PLWMArray), Dictionary(PLWMDictionary) 중 하나의 데이터  */
	enum eLWMSG_DATA_TYPE type;		/**< Object 타입 */
} LWMObject, *PLWMObject;

// private
/**
 * @brief KVO(Key-Value Object) 형태의 Object를 저장하는 경량형 메시지 Object
 */
typedef struct stLightWeightMessageKVObject {
	char *key;										/**< Key  */
	struct stLightWeightMessageObject *value;		/**< 경량형 메시지 Object  */
} LWMKVObject, *PLWMKVObject;

// private
typedef iw_list_t LWMDictionary;		// list of struct stLightWeightMessageKVObject
typedef iw_list_t *PLWMDictionary;
typedef const iw_list_t *LPCLWMDictionary;

// public
/**
 * @brief 경량형 메시지 구조체
 */
typedef struct stLightWeightMessage {
	LWMDictionary header;			/**< 경량형 메시지 헤더  */
	LWMDictionary body;				/**< 경량형 메시지 바디  */
} LWMessage, *PLWMessage;

#define NEW_LWM_EMPTY_MESSAGE() new_lwm_empty_message()
#define NEW_LWM_REQ_MESSAGE(__if,__dri,__di) new_lwm_req_message(__if, __dri, __di)
#define NEW_LWM_RES_MESSAGE(__dri,__rsc) new_lwm_resp_message(__dri, __rsc)

#define GET_LWM_INTERFACE_ID(__msg) get_lwm_interface_id(__msg)
#define GET_LWM_DRI(__msg) get_lwm_dri(__msg)
#define GET_LWM_TX_DRI() get_lwm_tx_dri()
	
#define SET_LWM_DI(__di) set_lwm_di(__di)
#define GET_LWM_DI() get_lwm_di()


#define GET_LWM_HEADER_OBJECT(__msg,__key) _GET_LWM_OBJECT_FROM_DICT((PLWMDictionary)(&__msg->header), __key)
#define SET_LWM_HEADER_OBJECT(__msg,__key,__value) _SET_LWM_OBJECT_FROM_DICT((PLWMDictionary)(&__msg->header), __key, __value)

#define GET_LWM_BODY_OBJECT(__msg,__key) _GET_LWM_OBJECT_FROM_DICT((PLWMDictionary)(&__msg->body), __key)
#define SET_LWM_BODY_OBJECT(__msg,__key,__value) _SET_LWM_OBJECT_FROM_DICT((PLWMDictionary)(&__msg->body), __key, __value)

#define NEW_LWM_NUMBER_OBJECT() new_lwm_empty_object(eLWM_Number)
#define NEW_LWM_STRING_OBJECT(__str) new_lwm_string_object(__str)
#define NEW_LWM_ARRAY_OBJECT() new_lwm_empty_object(eLWM_Array)
#define NEW_LWM_DICT_OBJECT() new_lwm_empty_object(eLWM_Dictionary)

#define ADD_LWM_ARRAY_ITEM(__arr_obj,__obj) add_lwm_object_to_array(__arr_obj->value, __obj)
#define LWM_FOREACH(__dict_or_arr,__obj_item) \
	for ( \
		iw_list_node_t *__obj_item = IW_GET_HEAD((iw_list_t *)__dict_or_arr); \
		__obj_item; \
		__obj_item = IW_GET_NEXT(__obj_item) \
	)
#define LWM_ARRAY_LENGTH(__arr_obj) IW_GET_LIST_NODE_NUM((iw_list_t *)__arr_obj->value)

#define _GET_LWM_OBJECT_FROM_DICT(__dict,__key) get_lwm_object_from_dict(__dict, __key)
#define _SET_LWM_OBJECT_FROM_DICT(__dict,__key,__value) set_lwm_object_to_dict(__dict, __key, __value)
#define GET_LWM_OBJECT(__dict_obj,__key) get_lwm_object_from_dict(__dict_obj->value, __key)
#define SET_LWM_OBJECT(__dict_obj,__key,__value) set_lwm_object_to_dict(__dict_obj->value, __key, __value)

#define GET_LWM_STRING_VALUE(__obj) (const char *const)((((__obj) && ((__obj->type) == eLWM_String)) ? __obj->value : NULL))
#define SET_LWM_STRING_VALUE(__obj,__value) set_lwm_string(__obj, __value)

#define GET_LWM_INT_VALUE(__obj) (int32_t)((((__obj) && ((__obj->type) == eLWM_Number)) ? *(int32_t *)(((PLWMNumber)__obj->value)->value) : 0))
#define GET_LWM_FLOAT_VALUE(__obj) (float)((((__obj) && ((__obj->type) == eLWM_Number)) ? *(float *)(((PLWMNumber)__obj->value)->value) : 0))
#define SET_LWM_INT_VALUE(__obj,__value) set_lwm_int(__obj, __value)
#define SET_LWM_FLOAT_VALUE(__obj,__value) set_lwm_float(__obj, __value)

#define IS_LWM_NUMBER(__obj) ((__obj->type) == eLWM_Number)
#define IS_LWM_STRING(__obj) ((__obj->type) == eLWM_String)
#define IS_LWM_ARRAY(__obj) ((__obj->type) == eLWM_Array)
#define IS_LWM_DICT(__obj) ((__obj->type) == eLWM_Dictionary)

#define DEL_LWM_MESSAGE(__msg) delete_lwm_message(__msg)
#define DEL_LWM_OBJECT(__obj) delete_lwm_object(__obj)
#define _DEL_LWM_KVOBJECT(__kvo) _delete_lwm_keyvalue_object(__kvo)

#define SERIALIZE_LWM_MESSAGE(__msg) serialize_lwm_message(__msg)
#define PARSE_LWM_MESSAGE_STR(__str) parse_lwm_message_str(__str)

#define RESPONSE_LWMSG(__msg) response_lwmsg(__msg)
#define REQUEST_LWMSG(__msg) request_lwmsg(__msg)

PLWMessage new_lwm_empty_message();
PLWMessage new_lwm_req_message(const uint32_t __if, const char *const __dri, const char *const __di);
PLWMessage new_lwm_resp_message(const char *const __dri, const uint32_t __rsc);
uint32_t get_lwm_interface_id(PLWMessage __msg);
char *get_lwm_dri(PLWMessage __msg);
/* XXX: Added from TO21COMMS { */
BOOL set_lwm_di(const char *const __di);
const char *get_lwm_di();
const char *get_lwm_tx_dri(void);
/* } */
PLWMObject get_lwm_object_from_dict(LPCLWMDictionary __dict, const char *const __key);
void set_lwm_object_to_dict(PLWMDictionary __dict, const char *const __key, PLWMObject __value);
iw_list_t *new_lwm_empty_array_or_dict();
void add_lwm_object_to_array(PLWMArray __array, PLWMObject __value);
PLWMObject _new_lwm_empty_object_for_ms(const enum ePARAM_TYPE __type);
PLWMObject new_lwm_empty_object(const LWMSG_DATA_TYPE __type);
PLWMObject new_lwm_string_object(const char *__str);
void get_lwm_number(PLWMObject __obj, void *__number_buffer);
void set_lwm_string(PLWMObject __obj, const char *__str);
void set_lwm_int(PLWMObject __obj, const int32_t __int);
void set_lwm_float(PLWMObject __obj, const float __float);
void delete_lwm_object(PLWMObject __obj);
void delete_lwm_message(PLWMessage __msg);
void _delete_lwm_keyvalue_object(PLWMKVObject __pkvo);
void _delete_lwm_kvo_by_key(PLWMDictionary __dict, const char *const __key);
void _delete_lwm_kvo(PLWMDictionary __dict);
void _delete_lwm_dictionary(PLWMDictionary __dict);
void _delete_lwm_array(PLWMArray __array);
char *serialize_lwm_object(PLWMObject __obj);
char *serialize_lwm_kvo(PLWMKVObject __kvo);
char *serialize_lwm_array(LPCLWMArray __array);
char *serialize_lwm_dictionary(LPCLWMDictionary __dict);
char *serialize_lwm_message(PLWMessage __msg);
PLWMessage parse_lwm_message_str(const char *__str);
iw_queue_t get_lwmsg_receive_queue();
void set_lwmsg_receive_queue(iw_queue_t __lwmsg_recv_q);
iw_queue_t get_lwmsg_send_queue();
void set_lwmsg_send_queue(iw_queue_t __lwmsg_send_q);
iw_task_t get_lwmsg_receive_task();
void set_lwmsg_receive_task(iw_task_t __lwmsg_recv_t);
iw_task_t get_lwmsg_send_task();
void set_lwmsg_send_task(iw_task_t __lwmsg_send_t);
iw_error_t response_lwmsg(PLWMessage __msg);
iw_error_t request_lwmsg(PLWMessage __msg);
BOOL get_lwmsg_network_enabled();
void set_lwmsg_network_enabled(BOOL __enabled);

