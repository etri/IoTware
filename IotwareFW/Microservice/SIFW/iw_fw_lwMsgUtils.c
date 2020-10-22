/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

/**
 * @file iw_fw_lwMsgUtils.c
 * @brief 경량형 메시지 유틸리티 구현 파일
 * @details 경량형 메시지 파싱, 직렬화 등 필수 유틸리티 구현 파일
 */
#include <ctype.h>
#include "iw_fw_lwMsgUtils.h"


extern size_t _get_size_of_type(const enum ePARAM_TYPE type);

static iw_queue_t lwmsg_receive_queue = NULL;
static iw_queue_t lwmsg_send_queue = NULL;
static iw_task_t lwmsg_receive_task = (iw_task_t)NULL;
static iw_task_t lwmsg_send_task = (iw_task_t)NULL;
static int lwmsg_tx_dri = 1;
static char lwmsg_tx_dri_str[10];
static char lwmsg_di_str[15] = "";
static BOOL network_enabled = FALSE;


/**
 * @brief 빈 경량형 메시지 구조체 생성
 * @details 빈 경량형 메시지를 동적할당 하고 그 구조체 포인터를 리턴
 * @return 생성한 경량형 메시지 구조체 포인터
 */
PLWMessage new_lwm_empty_message()
{
	PLWMessage rtn = (PLWMessage)calloc(1, sizeof(struct stLightWeightMessage));

	if (!rtn) return rtn;

	iw_init_list(&rtn->header);
	iw_init_list(&rtn->body);

	return rtn;
}

/**
 * @brief 새 Request 경량형 메시지 구조체 생성
 * @details 새 Request 경량형 메시지를 동적할당 하고 초기값을 지정한 뒤 그 구조체 포인터를 리턴
 * @param __if Interface ID
 * @param __dri dri
 * @param __di di
 * @return 생성한 경량형 메시지 구조체 포인터
 */
PLWMessage new_lwm_req_message(const uint32_t __if, const char *const __dri, const char *const __di)
{
	PLWMessage rtn = NEW_LWM_EMPTY_MESSAGE();

	if (!rtn) return rtn;

	PLWMObject _if = NEW_LWM_NUMBER_OBJECT();
	PLWMNumber num = _if->value;
	num->type = eP_UINT32;
	num->value = calloc(1, sizeof(__if));
	if (num->value)
		*(uint32_t *)num->value = __if;
	SET_LWM_HEADER_OBJECT(rtn, "if", _if);

	if (__dri) {
		PLWMObject _dri = NEW_LWM_STRING_OBJECT(__dri);
		SET_LWM_HEADER_OBJECT(rtn, "dri", _dri);
	}

	if (__di) {
		PLWMObject _di = NEW_LWM_STRING_OBJECT(__di);
		SET_LWM_HEADER_OBJECT(rtn, "di", _di);
	}

	return rtn;
}

/**
 * @brief 새 Response 경량형 메시지 구조체 생성
 * @details 새 Response 경량형 메시지를 동적할당 하고 초기값을 지정한 뒤 그 구조체 포인터를 리턴
 * @param __dri dri
 * @param __rsc Response Code
 * @return 생성한 경량형 메시지 구조체 포인터
 */
PLWMessage new_lwm_resp_message(const char *const __dri, const uint32_t __rsc)
{
	PLWMessage rtn = NEW_LWM_EMPTY_MESSAGE();

	if (!rtn) return rtn;

	if (__dri) {
		PLWMObject _dri = NEW_LWM_STRING_OBJECT(__dri);
		SET_LWM_HEADER_OBJECT(rtn, "dri", _dri);
	}

	if (__rsc) {
		PLWMObject _rsc = NEW_LWM_NUMBER_OBJECT();
		PLWMNumber num = _rsc->value;
		num->type = eP_UINT32;
		num->value = calloc(1, sizeof(__rsc));
		if (num->value)
			*(uint32_t *)num->value = __rsc;
		SET_LWM_HEADER_OBJECT(rtn, "rsc", _rsc);
	}

	return rtn;
}

/**
 * @brief 경량형 메시지에서 Interface ID 획득
 * @details 경량형 메시지에서 Interface ID 획득
 * @param __msg 경량형 메시지 구조체 포인터
 * @return Interface ID, 실패 시 0
 */
uint32_t get_lwm_interface_id(PLWMessage __msg)
{
	uint32_t __if = 0;
	PLWMObject _if = get_lwm_object_from_dict(&__msg->header, "if");

	// get_lwm_number(_if, &__if);
	if (_if && IS_LWM_NUMBER(_if)) {
		PLWMNumber num = (PLWMNumber)_if->value;
		if (num && num->type == eP_UINT32) {
			__if = *(uint32_t *)(num->value);
		}
	}

	return __if;
}

/**
 * @brief 경량형 메시지에서 dri 획득
 * @details 경량형 메시지에서 dri 획득
 * @param __msg 경량형 메시지 구조체 포인터
 * @return dri, 실패 시 NULL
 */
char *get_lwm_dri(PLWMessage __msg)
{
	char *rtn = NULL;
	PLWMObject _dri = get_lwm_object_from_dict(&__msg->header, "dri");

	if (_dri && IS_LWM_STRING(_dri)) {
		rtn = _dri->value;
	}

	return rtn;
}

/**
 * @brief 디바이스 생성 dri 조회
 * @details 디바이스 생성 dri 조회
 * @param 
 * @return dri, 실패 시 
 */
const char *get_lwm_tx_dri()
{
  	sprintf(lwmsg_tx_dri_str, "%d", lwmsg_tx_dri++);
	//printf("lwmsg_tx_dri_str: %s, lwmsg_tx_dri:%d\n", lwmsg_tx_dri_str, lwmsg_tx_dri);
	if(lwmsg_tx_dri>99999900)
		lwmsg_tx_dri = 1;

	return (const char *)lwmsg_tx_dri_str;
}


/**
 * @brief 디바이스 di 조회
 * @details 디바이스 di 조회
 * @param 
 * @return di, 실패 시 
 */
BOOL set_lwm_di(const char *const __di)
{
	if(strlen(__di)>14){
		printf("[E][iw_fw_lwMsgCore] Too long MCMSG Device ID(di) %s(%d)\n", lwmsg_di_str, strlen(__di));
		return FALSE;
	}
	
	strcpy(lwmsg_di_str, __di);
	//printf("lwmsg_di_str: %s(%d)\n", lwmsg_di_str, strlen(__di));

	return TRUE;
}

/**
 * @brief 디바이스 di 조회
 * @details 디바이스 di 조회
 * @param 
 * @return di, 실패 시 
 */
const char *get_lwm_di()
{
	return (const char *)lwmsg_di_str;
}

/**
 * @brief Dictionary Object에서 Object 획득
 * @details 경량형 메시지의 Dictionary Object에서 Key에 따른 Object 획득
 * @param __dict 경량형 메시지 Dictionary Object 포인터
 * @param __key Key
 * @return 경량형 메시지 Object 포인터
 */
PLWMObject get_lwm_object_from_dict(LPCLWMDictionary __dict, const char *const __key)
{
	PLWMObject rtn = NULL;
	PLWMKVObject pkvo = NULL;

	if (__dict && __key) {
		LIST_FOREACH(__dict, pxLWMItem) {
			pkvo = IW_GET_LIST_NODE_DATA(pxLWMItem);
			if (strcmp(pkvo->key, __key) == 0) {
				rtn = pkvo->value;
				break;
			}
		}
	}

	return rtn;
}

/**
 * @brief Dictionary Object에 Object 설정
 * @details 경량형 메시지의 Dictionary Object의 Key에 Object 설정
 * @param __dict 경량형 메시지 Dictionary Object 포인터
 * @param __key Key
 * @param __value 저장할 경량형 메시지 Object 포인터
 */
void set_lwm_object_to_dict(PLWMDictionary __dict, const char *const __key, PLWMObject __value)
{
	if (__dict && __key && __value) {
		_delete_lwm_kvo_by_key(__dict, __key);

		iw_list_node_t *lwm_item = calloc(1, sizeof(iw_list_node_t));
		if (lwm_item) {
			PLWMKVObject kvo = (PLWMKVObject)calloc(1, sizeof(LWMKVObject));
			if (!kvo) {
				free(lwm_item);
				return;
			}
			kvo->key = (char *)calloc(strlen(__key) + 1, sizeof(char));
			if (kvo->key)
				strcpy(kvo->key, __key);
			kvo->value = (PLWMObject)__value;

			_IW_SET_LIST_NODE_DATA(lwm_item, kvo);
			iw_add_list_node(__dict, lwm_item);
		}
	}
}

/**
 * @brief Array Object에 Object 추가
 * @details 경량형 메시지 Array Object에 경량형 메시지 Object 추가
 * @param __array 경량형 메시지 Array Object 포인터
 * @param __value 저장할 경량형 메시지 Object 포인터
 */
void add_lwm_object_to_array(PLWMArray __array, PLWMObject __value)
{
	if (__array && __value) {
		iw_list_node_t *lwm_item = calloc(1, sizeof(iw_list_node_t));
		if (lwm_item) {
			_IW_SET_LIST_NODE_DATA(lwm_item, __value);
			iw_add_list_node(__array, lwm_item);
		}
	}
}

/**
 * @brief 새 빈 경량형 메시지 Array / Dictionary Object 생성
 * @details 새 빈 경량형 메시지 Array / Dictionary Object 동적할당으로 생성
 * @return 경량형 메시지 Array / Dictionary Object 포인터
 */
iw_list_t *new_lwm_empty_array_or_dict()
{
	iw_list_t * rtn = (iw_list_t *)calloc(1, sizeof(iw_list_t));
	if (rtn)
		iw_init_list(rtn);
	return rtn;
}

/**
 * @brief 타입에 맞는 새 빈 경량형 메시지 Object 생성
 * @details MicroService 데이터 타입에 따라 알맞은 경량형 메시지 Object를 동적할당 생성
 * Array, Dictionary 타입은 없으며, 문자열 혹은 숫자 형태만 존재
 * @param __type MicroService 데이터 타입
 * @return 경량형 메시지 Object 포인터
 */
PLWMObject _new_lwm_empty_object_for_ms(const enum ePARAM_TYPE __type)
{
	if (IS_DECIMAL(__type) || __type == eP_FLOAT || __type == eP_DOUBLE) {
		return new_lwm_empty_object(eLWM_Number);
	}
	else if (__type == eP_STRING) {
		return new_lwm_empty_object(eLWM_String);
	}

	return NULL;
}

/**
 * @brief 타입에 맞는 새 빈 경량형 메시지 Object 생성
 * @details 경량형 메시지 데이터 타입에 따라 알맞은 경량형 메시지 Object를 동적할당 생성
 * @param __type 경량형 메시지 데이터 타입
 * @return 경량형 메시지 Object 포인터
 */
PLWMObject new_lwm_empty_object(const LWMSG_DATA_TYPE __type)
{
	void *value = NULL;
	PLWMObject rtn = (PLWMObject)calloc(1, sizeof(LWMObject));

	if (!rtn)
		return rtn;

	switch (__type) {
		case eLWM_Number:
			value = calloc(1, sizeof(LWMNumber));
			break;
		case eLWM_String:
		default:
			break;
		case eLWM_Array:
			value = (PLWMArray)new_lwm_empty_array_or_dict();
			break;
		case eLWM_Dictionary:
			value = (PLWMDictionary)new_lwm_empty_array_or_dict();
			break;
	}

	rtn->type = __type;
	rtn->value = value;

	return rtn;
}

/**
 * @brief 새 경량형 메시지 문자열 Object 생성
 * @details 초기값을 갖는 새 경량형 메시지 숫자 Object를 동적할당 해 생성
 * @param __str 초기 문자열 값, NULL 가능
 * @return 경량형 메시지 문자열 Object 포인터
 */
PLWMObject new_lwm_string_object(const char *__str)
{
	PLWMObject rtn = new_lwm_empty_object(eLWM_String);

	if (__str) {
		rtn->value = calloc(strlen(__str) + 1, sizeof(char));
		if (rtn->value)
			strcpy(rtn->value, __str);
	}

	return rtn;
}

/**
 * @brief 경량형 메시지 숫자 Object로부터 숫자 값 획득
 * @details 경량형 메시지 숫자 Object로부터 숫자 값 획득
 * @param __obj 경량형 메시지 Object 포인터
 * @param __number_buffer 숫자가 저장될 버퍼
 */
void get_lwm_number(PLWMObject __obj, void *__number_buffer)
{
	if (__obj && IS_LWM_NUMBER(__obj) && __obj->value) {
		PLWMNumber value = (PLWMNumber)__obj->value;
		size_t sz = _get_size_of_type(value->type);
		memcpy(__number_buffer, value->value, sz);
	}
}

/**
 * @brief 경량형 메시지 문자열 Object에 문자열 설정
 * @details 경량형 메시지 문자열 Object에 문자열 설정
 * @param __obj 경량형 메시지 Object 포인터
 * @param __str 문자열
 */
void set_lwm_string(PLWMObject __obj, const char *__str)
{
	if (__obj && IS_LWM_STRING(__obj) && __str) {
		if (__obj->value)
			free(__obj->value);
		__obj->value = calloc(strlen(__str) + 1, sizeof(char));
		if (__obj->value)
			strcpy(__obj->value, __str);
	}
}

PLWMNumber _get_lwm_number_from_obj(PLWMObject __obj)
{
	PLWMNumber num = NULL;
	if (__obj && IS_LWM_NUMBER(__obj)) {
		if (!__obj->value) {
			__obj->value = calloc(1, sizeof(LWMNumber));
		}
		if (__obj->value) {
			num = (PLWMNumber)__obj->value;
			if (num->value)
				free(num->value);
		}
	}
	return num;
}

/**
 * @brief 경량형 메시지 숫자 Object에 정수 값 설정
 * @details 경량형 메시지 숫자 Object에 정수 값 설정
 * @param __obj 경량형 메시지 Object 포인터
 * @param __int 정수 값
 */
void set_lwm_int(PLWMObject __obj, const int32_t __int)
{
	PLWMNumber num = _get_lwm_number_from_obj(__obj);
	if (num) {
		num->value = calloc(1, sizeof(__int));
		num->type = eP_INT32;
		if (num->value)
			*(int32_t *)num->value = __int;
	}
}

/**
 * @brief 경량형 메시지 숫자 Object에 실수 값 설정
 * @details 경량형 메시지 숫자 Object에 실수 값 설정
 * @param __obj 경량형 메시지 Object 포인터
 * @param __float 실수 값
 */
void set_lwm_float(PLWMObject __obj, const float __float)
{
	PLWMNumber num = _get_lwm_number_from_obj(__obj);
	if (num) {
		num->value = calloc(1, sizeof(__float));
		num->type = eP_FLOAT;
		if (num->value)
			*(float *)num->value = __float;
	}
}

/**
 * @brief 경량형 메시지 Object 삭제
 * @details 경량형 메시지 Object 안전하게 삭제
 * @param __obj 삭제할 경량형 메시지 Object 포인터
 */
void delete_lwm_object(PLWMObject __obj)
{
	if (__obj) {
		if (__obj->value) {
			if (IS_LWM_NUMBER(__obj)) {
				PLWMNumber value = (PLWMNumber)__obj->value;
				if (value->value)
					free(value->value);
				free(__obj->value);
			}
			else if (IS_LWM_STRING(__obj)) {
				free(__obj->value);
			}
			else if (IS_LWM_ARRAY(__obj)) {
				_delete_lwm_array((PLWMArray)__obj->value);
			}
			else if (IS_LWM_DICT(__obj)) {
				_delete_lwm_dictionary((PLWMDictionary)__obj->value);
			}
			__obj->value = NULL;
			__obj->type = eLWM_Unknown;
		}

		free(__obj);
	}
}

/**
 * @brief 경량형 메시지 삭제
 * @details 경량형 메시지 안전하게 삭제
 * @param __msg 삭제할 경량형 메시지 구조체 포인터
 */
void delete_lwm_message(PLWMessage __msg)
{
	if (__msg) {
		_delete_lwm_kvo(&__msg->header);
		_delete_lwm_kvo(&__msg->body);
		free(__msg);
	}
}

/**
 * @brief 경량형 메시지 KVO 형태 Object 삭제
 * @details 경량형 메시지 KVO 형태 Object 안전하게 삭제
 * @param __obj 삭제 할 경량형 메시지 KVO 형태 Object 포인터
 */
void _delete_lwm_keyvalue_object(PLWMKVObject __pkvo)
{
	if (__pkvo) {
		delete_lwm_object(__pkvo->value);
		free(__pkvo->key);
		__pkvo->value = NULL;
		__pkvo->key = NULL;
		free(__pkvo);
	}
}

/**
 * @brief 경량형 메시지 Dictionary Object에서 Key/Value 삭제
 * @details 경량형 메시지 Dictionary Object에서 지정한 Key와 그 Value 안전하게 삭제
 * Value는 경량형 메시지 KVO 형태 Object
 * @param __dict 경량형 메시지 Dictionary Object 포인터
 * @param __key 삭제할 키
 */
void _delete_lwm_kvo_by_key(PLWMDictionary __dict, const char *const __key)
{
	if (__dict && __key) {
		LIST_FOREACH(__dict, pxLWMItem) {
			PLWMKVObject pkvo = IW_GET_LIST_NODE_DATA(pxLWMItem);
			if (pkvo && strcmp(pkvo->key, __key) == 0) {
				_delete_lwm_keyvalue_object(pkvo);
				pkvo = NULL;
				iw_remove_list_node(__dict, pxLWMItem);
				free(pxLWMItem);
				break;
			}
		}
	}
}

/**
 * @brief 경량형 메시지 Dictionary Object 내용 삭제
 * @details 경량형 메시지 Dictionary Object에서 내용만 안전하게 모두 삭제
 * @param __dict 경량형 메시지 Dictionary Object 포인터
 */
void _delete_lwm_kvo(PLWMDictionary __dict)
{
	if (__dict) {
		LIST_FOREACH_TO_DEL(__dict, pxLWMItem) {
			PLWMKVObject pkvo = IW_GET_LIST_NODE_DATA(pxLWMItem);
			_delete_lwm_keyvalue_object(pkvo);
			pkvo = NULL;
			iw_remove_list_node(__dict, pxLWMItem);
			free(pxLWMItem);
		}
	}
}

/**
 * @brief 경량형 메시지 Dictionary Object 삭제
 * @details 경량형 메시지 Dictionary Object 안전하게 삭제
 * @param __dict 삭제 할 경량형 메시지 Dictionary Object 포인터
 */
void _delete_lwm_dictionary(PLWMDictionary __dict)
{
	_delete_lwm_kvo(__dict);
	free(__dict);
}

/**
 * @brief 경량형 메시지 Array Object 삭제
 * @details 경량형 메시지 Array Object 안전하게 삭제
 * @param __dict 삭제 할 경량형 메시지 Array Object 포인터
 */
void _delete_lwm_array(PLWMArray __array)
{
	if (__array) {
		LIST_FOREACH_TO_DEL(__array, pxLWMItem) {
			PLWMObject obj = IW_GET_LIST_NODE_DATA(pxLWMItem);
			DEL_LWM_OBJECT(obj);
			obj = NULL;
			iw_remove_list_node(__array, pxLWMItem);
			free(pxLWMItem);
		}
		free(__array);
	}
}

/**
 * @brief 경량형 메시지 Object 직렬화
 * @details 경량형 메시지 Object를 동적할당한 문자열로 직렬화
 * @param __obj 경량형 메시지 Object 포인터
 * @return 직렬화 한 문자열, 실패 시 NULL
 */
char *serialize_lwm_object(PLWMObject __obj)
{
	char *str = NULL;

	if (__obj && __obj->value) {
		if (__obj->type == eLWM_Number) {
			PLWMNumber num = __obj->value;
			if (num->type == eP_INT32) {
				// "-2147483648"
				str = calloc(12, sizeof(char));
				if (str)
					snprintf(str, 11, "%ld", *(int32_t *)num->value);
			}
			else if (num->type == eP_UINT32) {
				// "4294967295"
				str = calloc(11, sizeof(char));
				if (str)
					snprintf(str, 10, "%ld", *(uint32_t *)num->value);
			}
			else if (num->type == eP_INT16) {
				// "-32768"
				str = calloc(7, sizeof(char));
				if (str)
					snprintf(str, 6, "%d", *(int16_t *)num->value);
			}
			else if (num->type == eP_UINT16) {
				// "65535"
				str = calloc(6, sizeof(char));
				if (str)
					snprintf(str, 5, "%d", *(uint16_t *)num->value);
			}
			else if (num->type == eP_INT8) {
				// "-128"
				str = calloc(5, sizeof(char));
				if (str)
					snprintf(str, 4, "%d", *(int8_t *)num->value);
			}
			else if (num->type == eP_UINT8) {
				// "255"
				str = calloc(4, sizeof(char));
				if (str)
					snprintf(str, 3, "%d", *(uint8_t *)num->value);
			}
			else if (num->type == eP_FLOAT) {
				str = calloc(21, sizeof(char));
				if (str)
					//snprintf(str, 20, "%.2f", *(float *)num->value);
					snprintf(str, 20, "%d.%d", (int)(*(float *)num->value), (int)(*(float *)num->value * 100.f) % 100);
			}
			else if (num->type == eP_DOUBLE) {
				str = calloc(21, sizeof(char));
				if (str)
					//snprintf(str, 20, "%lf", *(double *)num->value);
					snprintf(str, 20, "%d.%d", (int)(*(double *)num->value), (int)(*(double *)num->value * 100.f) % 100);
			}
		}
		else if (__obj->type == eLWM_String) {
			size_t len = strlen(__obj->value);
			str = calloc(len + 1, sizeof(char));
			if (str)
				memcpy(str, __obj->value, len);
		}
		else if (__obj->type == eLWM_Array) {
			str = serialize_lwm_array(__obj->value);
		}
		else if (__obj->type == eLWM_Dictionary) {
			str = serialize_lwm_dictionary(__obj->value);
		}
	}

	return str;
}

/**
 * @brief 경량형 메시지 KVO 형태 Object 직렬화
 * @details 경량형 메시지 KVO 형태 Object를 동적할당한 문자열로 직렬화
 * @param __kvo 경량형 메시지 KVO 형태 Object 포인터
 * @return 직렬화 한 문자열, 실패 시 NULL
 */
char *serialize_lwm_kvo(PLWMKVObject __kvo)
{
	char *str = NULL;

	if (__kvo && __kvo->key && __kvo->value) {
		char *str_value = serialize_lwm_object(__kvo->value);
		if (str_value) {
			str = calloc(strlen(__kvo->key) + strlen(str_value) + 2, sizeof(char));
			if (str) {
				strcpy(str, __kvo->key);
				strcat(str, "=");
				strcat(str, str_value);
			}
			free(str_value);
		}
	}

	return str;
}

/**
 * @brief 경량형 메시지 Array Object 직렬화
 * @details 경량형 메시지 Array Object를 동적할당한 문자열로 직렬화
 * @param __array 경량형 메시지 Array Object 포인터
 * @return 직렬화 한 문자열
 */
char *serialize_lwm_array(LPCLWMArray __array)
{
	char *str = calloc(3, sizeof(char));
	if (str!=NULL)
		strcpy(str, "[");

	if (__array) {
		LIST_FOREACH(__array, pxLWMItem) {
			PLWMObject pobj = IW_GET_LIST_NODE_DATA(pxLWMItem);
			char *temp = serialize_lwm_object(pobj);
			if (temp!=NULL && str!=NULL) {
				str = realloc(str, strlen(str) + strlen(temp) + 3);
				if (str!=NULL) {
					if (str[1] != '\0')
						strcat(str, ",");
					strcat(str, temp);
				}
				free(temp);
			}
		}
	}

	if (str)
		strcat(str, "]");
	return str;
}

/**
 * @brief 경량형 메시지 Dictionary Object 직렬화
 * @details 경량형 메시지 Dictionary Object를 동적할당한 문자열로 직렬화
 * @param __dict 경량형 메시지 Dictionary Object 포인터
 * @return 직렬화 한 문자열
 */
char *serialize_lwm_dictionary(LPCLWMDictionary __dict)
{
	char *str = calloc(3, sizeof(char));
	if (str!=NULL)
		strcpy(str, "{");

	if (__dict) {
		LIST_FOREACH(__dict, pxLWMItem) {
			PLWMKVObject pkvo = IW_GET_LIST_NODE_DATA(pxLWMItem);
			char *temp = serialize_lwm_kvo(pkvo);
			if (temp!=NULL && str!=NULL) {
				str = realloc(str, strlen(str) + strlen(temp) + 3);
				if (str!=NULL) {
					if (str[1] != '\0')
						strcat(str, ";");
					strcat(str, temp);
				}
				free(temp);
			}
		}
	}

	if (str)
		strcat(str, "}");
	return str;
}

/**
 * @brief 경량형 메시지 직렬화
 * @details 경량형 메시지를 동적할당한 문자열로 직렬화
 * @param __msg 경량형 메시지 포인터
 * @return 직렬화 한 문자열, 실패 시 NULL
 */
char *serialize_lwm_message(PLWMessage __msg)
{
	char *str = NULL;
	char *header = NULL, *body = NULL;

	if (__msg) {
		size_t len = 0;
		header = serialize_lwm_dictionary(&__msg->header);
		if (IW_GET_LIST_NODE_NUM(&__msg->body) > 0)
			body = serialize_lwm_dictionary(&__msg->body);
		if (header) len += strlen(header);
		if (body) len += strlen(body);

		str = calloc(len + 1, sizeof(char));
		if (str && header) {
			strcpy(str, header);
			free(header);
		}
		if (str && body) {
			strcat(str, body);
			free(body);
		}
	}

	return str;
}


// mode
// 0 : 오픈 가능
// 1 << 0 : dict
// 1 << 1 : array
// 1 << 2 : key:value 중 key
// 1 << 3 : key:value 중 value
enum eLWM_PARSING_MODE {
	ePM_Openable = 1 << 0,
	ePM_Dict = 1 << 1,
	ePM_Array = 1 << 2,
	ePM_KV_Key = 1 << 3,
	ePM_KV_Val = 1 << 4,
	ePM_Object = ePM_Dict | ePM_Array,
	ePM_Header = 1 << 6,
	ePM_Body = 1 << 7,
};

#define IS_LWM_OPENABLE(__mode) ((__mode & ePM_Openable) == ePM_Openable)
#define IS_LWM_HEADER_OPENABLE(__mode) (__mode == (ePM_Header | ePM_Openable))
#define IS_LWM_BODY_OPENABLE(__mode) (__mode == (ePM_Body | ePM_Openable))
#define SET_LWM_HEADER_OPENABLE(__mode) __mode = (ePM_Header | ePM_Openable)
#define SET_LWM_BODY_OPENABLE(__mode) __mode = (ePM_Body | ePM_Openable)
#define IS_LWM_OBJECT(__mode) ((__mode & ePM_Object) != 0)

static BOOL _lwm_is_token(const char ch, const char *tokens);
static PLWMObject _lwm_new_array_parsing(const char **p);
static PLWMObject _lwm_dict_parsing(const char **p, PLWMDictionary dict, BOOL no_return);
static PLWMObject _lwm_new_dict_parsing(const char **p);
static PLWMKVObject _lwm_kvo_parsing(const char **p);


/**
 * @brief 문자의 토큰 가능 여부 획득
 * @details 주어진 문자가 경량형 메시지 문자열에서의 토큰 가능한지 여부를 획득
 * 알파벳, 숫자, 공백에 포함되는지 혹은 출력 가능한 문자인지 여부로 판단
 * 또는 2번째 인자인 tokens에 포함해도 토큰 가능으로 판단
 * @param __ch 토큰 가능 여부 확인할 문자
 * @param tokens 추가 토큰 체크 시 사용할 문자들
 * @return 토큰 가능하면 1, 아니면 0
 */
static BOOL _lwm_is_token(const char ch, const char *tokens)
{
	const char *p = tokens;
	// 시작지점의 NULL문자는 무조건 처리문자로 간주
	if (*p == '\0') return TRUE;

	for (; *p != '\0'; ++p) {
		if (ch == *p || !(isalnum(ch) || isspace(ch) || ispunct(ch)))
			return TRUE;
	}

	return FALSE;
}

/**
 * @brief 경량형 메시지 Array Object로 파싱
 * @details 주어진 문자열로부터 경량형 메시지 Array Object를 파싱해 획득
 * @param p 문자열 포인터
 * @return 경량형 메시지 Array Object 포인터
 */
static PLWMObject _lwm_new_array_parsing(const char **p)
{
	PLWMObject arr_obj = NEW_LWM_ARRAY_OBJECT();
	PLWMObject obj = NULL;
	enum eLWM_PARSING_MODE mode = ePM_Openable;
	BOOL bFlag = TRUE;
	const char *start = NULL;
	char *valstr = NULL;
	BOOL value_has_num = FALSE;
	BOOL value_has_word = FALSE;
	BOOL value_has_one_period = FALSE;
	BOOL value_has_many_period = FALSE;

	if (**p == '[')
		++(*p);
	if (**p == ']')
		return arr_obj;

	for (; p && *p && **p != '\0'; ++(*p)) {
		if (isspace(**p))
			continue;
		if (IS_LWM_OPENABLE(mode) && (**p == ',' || **p == ']')) {
			bFlag = FALSE;
			break;
		}

		if (IS_LWM_OPENABLE(mode) && **p == '{') {
			mode = ePM_Dict;
			obj = _lwm_new_dict_parsing(p);
			ADD_LWM_ARRAY_ITEM(arr_obj, obj);
			obj = NULL;
		}
		else if (IS_LWM_OPENABLE(mode) && **p == '[') {
			mode = ePM_Array;
			obj = _lwm_new_array_parsing(p);
			ADD_LWM_ARRAY_ITEM(arr_obj, obj);
			obj = NULL;
		}
		else if (!IS_LWM_OPENABLE(mode) && IS_LWM_OBJECT(mode)) {
			if (**p == ',') {
				mode = ePM_Openable | ePM_Object;
				continue;
			}
			else if (**p == ']') {
				break;
			}
		}
		else if (IS_LWM_OPENABLE(mode) && start == NULL)  {
			mode = ePM_KV_Val;
			start = *p;

			if (_lwm_is_token(**p, "};=")) {
				bFlag = FALSE;
				break;
			}
		}
		else if (mode == ePM_KV_Val && (**p == ',' || **p == ']')) {
			size_t len = *p - start + 1;
			valstr = (char *)malloc(*p - start + 1);
			ZeroMemory(valstr, len);
			strncpy(valstr, start, len - 1);

			obj = NEW_LWM_STRING_OBJECT(valstr);
			ADD_LWM_ARRAY_ITEM(arr_obj, obj);
			obj = NULL;

			if (valstr) free(valstr);
			valstr = NULL;
			start = NULL;
			mode = ePM_Openable;

			if (**p == ']')
				break;
		}
		else if (mode == ePM_KV_Val) {
			// do nothing
		}
		else {
			bFlag = FALSE;
			break;
		}

		if (mode == ePM_KV_Val) {
			if (**p != '.' && !isdigit(**p)) value_has_word = TRUE;
			if (isdigit(**p)) value_has_num = TRUE;
			if (!value_has_one_period && !value_has_many_period && **p == '.')
				value_has_one_period = TRUE;
			else if (value_has_one_period && !value_has_many_period && **p == '.')
				value_has_many_period = FALSE;
		}
	}

	if (!bFlag) {
		if (obj) DEL_LWM_OBJECT(obj);
		obj = NULL;
		if (arr_obj) DEL_LWM_OBJECT(arr_obj);
		arr_obj = NULL;
	}

	return arr_obj;
}

/**
 * @brief 경량형 메시지 Dictionary Object로 파싱
 * @details 주어진 문자열로부터 경량형 메시지 Dictionary Object를 파싱해 획득
 * @param p 문자열 포인터
 * @param dict 파싱한 데이터가 저장될 미리 만들어진 경량형 메시지 Dictionary Object
 * @param no_return 리턴 값 무시 여부
 * @return 경량형 메시지 Dictionary Object 포인터, 실패 혹은 no_return == 1 시 NULL
 */
static PLWMObject _lwm_dict_parsing(const char **p, PLWMDictionary dict, BOOL no_return)
{
	PLWMObject obj = NULL;
	BOOL bFlag = TRUE;
	PLWMKVObject kvo = NULL;

	if (**p == '{')
		++(*p);

	for (; p && *p && **p != '\0'; ++(*p)) {
		if (isspace(**p))
			continue;
		if (**p == '}')
			break;
		if (**p == ';')
			continue;

		kvo = _lwm_kvo_parsing(p);
		if (kvo) {
			_delete_lwm_kvo_by_key(dict, kvo->key);

			iw_list_node_t *lwm_item = malloc(sizeof(iw_list_node_t));
			if (lwm_item) {
				ZeroMemory(lwm_item, sizeof(iw_list_node_t));
				_IW_SET_LIST_NODE_DATA(lwm_item, kvo);
				iw_add_list_node(dict, lwm_item);
			}

			if (**p == '}')
				break;
		}
		else {
			bFlag = FALSE;
			break;
		}
	}

	if (bFlag && dict && !no_return) {
		obj = NEW_LWM_DICT_OBJECT();
		obj->type = eLWM_Dictionary;
		obj->value = dict;
	}
	if (!bFlag && dict) {
		_delete_lwm_kvo(dict);
	}

	return obj;
}

/**
 * @brief 경량형 메시지 Dictionary Object로 파싱
 * @details 주어진 문자열로부터 경량형 메시지 Dictionary Object를 파싱해 획득
 * @param p 문자열 포인터
 * @return 경량형 메시지 Dictionary Object 포인터
 */
static PLWMObject _lwm_new_dict_parsing(const char **p)
{
	PLWMDictionary dict = (PLWMDictionary)new_lwm_empty_array_or_dict();
	PLWMObject obj = _lwm_dict_parsing(p, dict, FALSE);
	if (!obj) {
		free(dict);
	}
	return obj;
}

/**
 * @brief 경량형 메시지 KVO 형태 Object로 파싱
 * @details 주어진 문자열로부터 경량형 메시지 KVO 형태 Object를 파싱해 획득
 * @param p 문자열 포인터
 * @return 경량형 메시지 KVO 형태 Object 포인터
 */
static PLWMKVObject _lwm_kvo_parsing(const char **p)
{
	PLWMKVObject kvo = NULL;
	BOOL bFlag = TRUE;
	enum eLWM_PARSING_MODE mode = ePM_KV_Key;
	char *key = NULL, *valstr = NULL;
	PLWMObject value = NULL;
	const char *start = NULL;
	BOOL value_has_num = FALSE;
	BOOL value_has_word = FALSE;
	BOOL value_has_one_period = FALSE;
	BOOL value_has_many_period = FALSE;

	for (; p && *p && **p != '\0'; ++(*p)) {
		if (isspace(**p))
			continue;

		if (mode == ePM_KV_Key) {
			// {[]};,
			if (_lwm_is_token(**p, "{[]};,")) {
				bFlag = FALSE;
				break;
			}
			// =
			if (**p == '=') {
				if (!start || start[0] == '\0') {
					bFlag = FALSE;
					break;
				}
				else {
					size_t len = *p - start + 1;
					key = (char *)malloc(*p - start + 1);
					ZeroMemory(key, len);
					strncpy(key, start, len - 1);

					mode = ePM_KV_Val;
					start = NULL;
					continue;
				}
			}

			if (start == NULL) start = *p;
		}
		else if (mode == ePM_KV_Val) {
			if (start == NULL)  {
				start = *p;

				if (**p == '{') {
					value = _lwm_new_dict_parsing(p);
					if (!value) bFlag = FALSE;
					break;
				}
				else if (**p == '[') {
					value = _lwm_new_array_parsing(p);
					if (!value) bFlag = FALSE;
					break;
				}
				else if (_lwm_is_token(**p, "]};=,")) {
					bFlag = FALSE;
					break;
				}
			}
			else if (_lwm_is_token(**p, "{[=,")) {
				bFlag = FALSE;
				break;
			}
			else if (_lwm_is_token(**p, "]};")) {
				size_t len = *p - start + 1;
				valstr = (char *)malloc(*p - start + 1);
				ZeroMemory(valstr, len);
				strncpy(valstr, start, len - 1);
				break;
			}

			if (**p != '.' && !isdigit(**p)) value_has_word = TRUE;
			if (isdigit(**p)) value_has_num = TRUE;
			if (!value_has_one_period && !value_has_many_period && **p == '.')
				value_has_one_period = TRUE;
			else if (value_has_one_period && !value_has_many_period && **p == '.')
				value_has_many_period = FALSE;
		}
		else {
			bFlag = FALSE;
			break;
		}
	}

	if (bFlag) {
		kvo = (PLWMKVObject)malloc(sizeof(LWMKVObject));
		ZeroMemory(kvo, sizeof(LWMKVObject));
		kvo->key = key;
		if (!value) {
			kvo->value = NEW_LWM_STRING_OBJECT(valstr);
		}
		else {
			kvo->value = (PLWMObject)value;
		}
	}
	else {
		if (key) free(key);
		key = NULL;
	}

	if (valstr) free(valstr);
	valstr = NULL;

	return kvo;
}

/**
 * @brief 경량형 메시지 파싱
 * @details 주어진 문자열로부터 경량형 메시지를 파싱해 획득
 * @param p 문자열 포인터
 * @return 경량형 메시지 포인터
 */
PLWMessage parse_lwm_message_str(const char *__str)
{
	if (!__str)
		return NULL;

	size_t len = strlen(__str);
	if (len == 0)
		return NULL;

	PLWMessage pmsg = NEW_LWM_EMPTY_MESSAGE();
	const char *p = __str;
	enum eLWM_PARSING_MODE SET_LWM_HEADER_OPENABLE(mode);
	BOOL bFlag = FALSE;

	int j=0;
	for (; *p != '\0'; ++p) {
		if (isspace(*p))
			continue;
		// []};=,
		if (_lwm_is_token(*p, "[]};=,")) {
			break;
		}

		if (*p == '{') {
			if (IS_LWM_HEADER_OPENABLE(mode)) {
				_lwm_dict_parsing(&p, &pmsg->header, TRUE);
				SET_LWM_BODY_OPENABLE(mode);
				bFlag = TRUE;
			}
			else if (IS_LWM_BODY_OPENABLE(mode)) {
				_lwm_dict_parsing(&p, &pmsg->body, TRUE);
				mode = ePM_Openable;
				bFlag = TRUE;
			}
			else {
				bFlag = FALSE;
				break;
			}
		}
		else {
			bFlag = FALSE;
			break;
		}
	}

	if (!bFlag) {
		DEL_LWM_MESSAGE(pmsg);
		pmsg = NULL;
	}

	return pmsg;
}

iw_queue_t get_lwmsg_receive_queue()
{
	return lwmsg_receive_queue;
}

void set_lwmsg_receive_queue(iw_queue_t __lwmsg_recv_q)
{
	if (__lwmsg_recv_q)
		lwmsg_receive_queue = __lwmsg_recv_q;
}

iw_queue_t get_lwmsg_send_queue()
{
	return lwmsg_send_queue;
}

void set_lwmsg_send_queue(iw_queue_t __lwmsg_send_q)
{
	if (!__lwmsg_send_q) return;

	if (lwmsg_send_queue) {
		PLWMessage lwmsg;
		while (recv_queue(lwmsg_send_queue, &lwmsg, FALSE) == IW_SUCCESS) {
			DEL_LWM_MESSAGE(lwmsg);
			sleep(10);
		}
		delete_queue(lwmsg_send_queue);
	}
	lwmsg_send_queue = __lwmsg_send_q;
}

iw_task_t get_lwmsg_receive_task()
{
	return lwmsg_receive_task;
}

void set_lwmsg_receive_task(iw_task_t __lwmsg_recv_t)
{
	lwmsg_receive_task = __lwmsg_recv_t;
}

iw_task_t get_lwmsg_send_task()
{
	return lwmsg_send_task;
}

void set_lwmsg_send_task(iw_task_t __lwmsg_send_t)
{
	lwmsg_send_task = __lwmsg_send_t;
}

/**
 * @brief Response 경량형 메시지 송신
 * @details Response 경량형 메시지를, 네트워크 인터페이스를 통해 송신
 * @param __msg 경량형 메시지 포인터
 * @return send_queue() 성공 시 0, 실패 시 -1
 */
iw_error_t response_lwmsg(PLWMessage __msg)
{
	if (lwmsg_send_queue && __msg)
		return send_queue(lwmsg_send_queue, &__msg, 0, lwmsg_send_task);
	return IW_FAIL;
}

/**
 * @brief Request 경량형 메시지 송신
 * @details Request 경량형 메시지를, 네트워크 인터페이스를 통해 송신
 * @param __msg 경량형 메시지 포인터
 * @return send_queue() 성공 시 0, 실패 시 -1
 */
iw_error_t request_lwmsg(PLWMessage __msg)
{
	if (lwmsg_send_queue && __msg)
		return send_queue(lwmsg_send_queue, &__msg, 0, lwmsg_send_task);
	return IW_FAIL;
}

BOOL get_lwmsg_network_enabled()
{
	return network_enabled;
}

void set_lwmsg_network_enabled(BOOL __enabled)
{
	network_enabled = __enabled;
}
