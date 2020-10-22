/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "iw_fw_microservices.h"
#include <stdarg.h>


static iw_list_t _mservices;

static BOOL _device_registered = FALSE;


// private prototype
static void *_safe_alloc(const size_t __sz);
static iw_list_node_t *_param_list_item_with_init_value(struct stParameter *__param, const AUTO __value);
struct stParameter *_make_task_parameter(int __param_type, const enum ePARAM_TYPE __type, const char *const __name, const char *const __alias, const size_t __sz);
static void _del_param_list(iw_list_t *const pparam_list);
static void _del_task_link(struct stTaskLink *task_link);
static void _find_task_links(struct stTaskLink *__tl, const struct stTaskInfo *__ti, iw_list_t *__list);
static BOOL _add_task_link(PSTMicroServiceInfo __pmsi, PSTTaskInfo __start_ti, PSTTaskInfo __dest_ti);
static struct stTaskLink *_find_last_task_link(struct stTaskLink *__tl, BOOL(*__test)(PSTTaskInfo, void *), void *__cond);
static BOOL _curr_task_state(PSTTaskInfo __ti, void *__cond);
static BOOL _onoff_test(PSTTaskInfo __ti, void *__cond);


#define NEW_LWM_OBJECT_FROM_PARAM(__param) new_lwm_object_from_type_value(__param->type, __param->value)


static void *_safe_alloc(const size_t __sz)
{
	void *v = NULL;

	if (__sz > 0) {
		v = malloc(__sz);
		if(v == NULL)
			return NULL;
		ZeroMemory(v, __sz);
	}

	return v;
}

void set_device_is_registered(BOOL __flag)
{
	_device_registered = __flag;
}

BOOL get_device_is_registered()
{
	return _device_registered;
}

struct stTaskInfo *add_new_task(
	struct stMicroServiceInfo *__parent,
	void(*__fp)(void *),
	ProcessingHandler __init_fp,
	ProcessingHandler __before_fp,
	ProcessingHandler __after_fp,
	const char *__name,
	const uint32_t __input_count,
	const uint32_t __output_count,
	const int32_t __lwmsg_input
)
{
	struct stTaskInfo *__self = NULL;

	if (__lwmsg_input >= 0 && (uint32_t)__lwmsg_input + 1 > __input_count)
		return __self;

	if (__parent) {
		__self = _safe_alloc(sizeof(struct stTaskInfo));
		__self->parent = __parent;
		ZeroMemory(__self->name, MS_MAX_NAME_LENGTH + 1);
		strncpy(__self->name, __name, MS_MAX_NAME_LENGTH);
		iw_init_list(&__self->user_params);
		iw_init_list(&__self->static_params);
		iw_init_list(&__self->flexible_params);
		__self->initial_state = I_S_READY;
		__self->offloading = FALSE;
		__self->priority = MS_TASK_PRIORITY;
		__self->state = NOT_READY;

		__self->task_t = (iw_task_t)NULL;
		__self->task_fn = __fp;
		__self->task_idx = IW_GET_LIST_NODE_NUM(&__parent->tasks);

		__self->init_fn = __init_fp;
		__self->before_fn = __before_fp;
		__self->after_fn = __after_fp;

		__self->input_queues = NULL;
		__self->output_ports = NULL;
		__self->output_ptr = NULL;
		__self->input_item_size = 0;
		__self->output_item_size = 0;

		__self->lwmsg_input_port = __lwmsg_input;

		__self->input_count = __input_count;
		if (__input_count > 0) {
			__self->input_queues = _safe_alloc(sizeof(iw_queue_t) * __input_count);
			if (__lwmsg_input >= 0)
				__self->input_queues[__lwmsg_input] = create_queue(mainQUEUE_LENGTH, sizeof(PLWMessage));
		}

		__self->output_count = __output_count;
		if (__output_count > 0) {
			__self->output_ports = _safe_alloc(sizeof(struct stConnectLine *) * __output_count);
		}

		iw_list_node_t *__tasks = _safe_alloc(sizeof(iw_list_node_t));
		if(__tasks != NULL){
			_IW_SET_LIST_NODE_DATA(__tasks, __self);
			iw_add_list_node(&__parent->tasks, __tasks);
		}
	}

	return __self;
}

void set_inout_item_size(struct stTaskInfo *__self, const size_t __input_item_size, const size_t __output_item_size)
{
	if (__self) {
		__self->input_item_size = __input_item_size;
		__self->output_item_size = __output_item_size;

		if (__self->input_queues) {
			for (int i=0; i < __self->input_count; i++) {
				if (i != __self->lwmsg_input_port) {
					if (__self->input_queues[i])
						delete_queue(__self->input_queues[i]);
					__self->input_queues[i] = create_queue(mainQUEUE_LENGTH, __input_item_size);
				}
			}
		}

		if (__self->output_ptr)
			free(__self->output_ptr);
		__self->output_ptr = NULL;
		if (__output_item_size > 0)
			__self->output_ptr = _safe_alloc(__output_item_size);
	}
}

static iw_list_node_t *_param_list_item_with_init_value(struct stParameter *__param, const AUTO __value)
{
	iw_list_node_t *__param_item = NULL;
	if (__param) {
		enum ePARAM_ATTR attr = __param->attr;
		__param->attr = READ | WRITE;
		memcpy(__param->value, __value, __param->value_size);
		__param->attr = attr;
		__param_item = _safe_alloc(sizeof(iw_list_node_t));
		if (__param_item) {
			_IW_SET_LIST_NODE_DATA(__param_item, __param);
		}
	}
	return __param_item;
}

static void _add_param_to_list(iw_list_t *list, struct stParameter *param, const AUTO value)
{
	if (list && param) {
		iw_list_node_t *__param_item = _param_list_item_with_init_value(param, value);
		if (__param_item) {
			iw_add_list_node(list, __param_item);
		}
		else {
			if (param->value)
				free(param->value);
			param->value = NULL;
			free(param);
			param = NULL;
		}
	}
}

void add_task_param(int __param_type, struct stTaskInfo *__ti, const enum ePARAM_TYPE __type, const char *const __name, const char *const __alias, const AUTO __value, const size_t __value_size)
{
	iw_list_t *list = NULL;
	struct stParameter *param = _make_task_parameter(__param_type, __type, __name, __alias, __value_size);

	if (__param_type == USER_PARAM) {
		list = &__ti->user_params;
	}
	else if (__param_type == STATIC_PARAM) {
		list = &__ti->static_params;
	}
	else if (__param_type == FLEXIBLE_PARAM) {
		list = &__ti->flexible_params;
	}

	_add_param_to_list(list, param, __value);
}

void add_ms_input_param(struct stMicroServiceInfo *__pmsi, const enum ePARAM_TYPE __type, const char *const __name, const char *const __alias, const AUTO __value, const size_t __value_size)
{
	iw_list_t *list = &__pmsi->input_params;
	struct stParameter *param = _make_task_parameter(MS_INPUT_PARAM, __type, __name, __alias, __value_size);
	_add_param_to_list(list, param, __value);
}

struct stParameter *_make_task_parameter(int __param_type, const enum ePARAM_TYPE __type, const char *const __name, const char *const __alias, const size_t __sz)
{
	struct stParameter *param = _safe_alloc(sizeof(struct stParameter));

	if (param) {
		enum ePARAM_ATTR attr = NONE;
		if (__param_type == USER_PARAM) {
			attr = NONE;
		}
		else if (__param_type == STATIC_PARAM) {
			attr = READ;
		}
		else if (__param_type == FLEXIBLE_PARAM || __param_type == MS_INPUT_PARAM) {
			attr = READ | WRITE;
		}
		strncpy(param->name, __name, MS_MAX_NAME_LENGTH);
		strncpy(param->alias, __alias, MS_MAX_NAME_LENGTH);
		param->value = _safe_alloc(__sz);
		param->value_size = __sz;
		param->attr = attr;
		param->type = __type;
	}

	return param;
}


struct stTaskInfo *get_task_info_by_name(const struct stMicroServiceInfo *const __msi, const char *const __name)
{
	if (__msi == NULL)
		return NULL;

	if (__name == NULL || __name[0] == '\0')
		return NULL;

	LIST_FOREACH(&__msi->tasks, pxItem) {
		PSTTaskInfo ti = IW_GET_LIST_NODE_DATA(pxItem);
		if (strcmp(__name, ti->name) == 0)
			return ti;
	}
	return NULL;
}

struct stTaskInfo *get_task_info_by_idx(const struct stMicroServiceInfo *const __msi, const size_t __index)
{
	if (__msi == NULL)
		return NULL;

	if (IW_GET_LIST_NODE_NUM(&__msi->tasks) < __index + 1)
		return NULL;

	LIST_FOREACH(&__msi->tasks, pxItem) {
		PSTTaskInfo ti = IW_GET_LIST_NODE_DATA(pxItem);
		if (ti->task_idx == __index)
			return ti;
	}
	return NULL;
}

const void *get_parameter_value_ptr(const struct stTaskInfo *const __taskinfo, const char *const __param_name)
{
	const struct stParameter *param_name = get_parameter_from_param_list(&__taskinfo->user_params, __param_name, NULL);
	if (param_name) {
		return param_name->value;
	}

	param_name = get_parameter_from_param_list(&__taskinfo->static_params, __param_name, NULL);
	if (param_name) {
		return param_name->value;
	}

	param_name = get_parameter_from_param_list(&__taskinfo->flexible_params, __param_name, NULL);
	if (param_name) {
		return param_name->value;
	}
	return NULL;
}

const void *get_ms_input_parameter_value_ptr(const struct stMicroServiceInfo *const __pmsi, const char *const __param_name)
{
	const struct stParameter *param_name = get_parameter_from_param_list(&__pmsi->input_params, __param_name, NULL);
	if (param_name) {
		return param_name->value;
	}
	return NULL;
}

/**
 * @brief 파라미터 정보 구조체 획득
 * @details 특정 파라미터 리스트에서 파라미터 이름으로 검색 해 파라미터 정보 구조체를 획득
 * 검색에 사용하는 파라미터 이름은 Task에서 사용하는 파라미터 이름 혹은 경량형 메시지에서 사용하는 파라미터 이름
 * 둘 중 하나 혹은 둘 다를 입력함
 * @param __params 파라미터 리스트 포인터
 * @param __param_name Task에서 사용하는 파라미터 이름
 * @param __param_alias 경량형 메시지에서 사용하는 파라미터 이름
 * @return 파라미터 정보 구조체 포인터, 검색에 실패하면 NULL
 */
struct stParameter *get_parameter_from_param_list(const iw_list_t *const __params, const char *const __param_name, const char *const __param_alias)
{
	struct stParameter *param = NULL;

	if (__params == NULL || _IW_LIST_IS_EMPTY(__params))
		return NULL;

	if ((__param_name == NULL && __param_alias == NULL) || (__param_name[0] == '\0' && __param_alias[0] == '\0'))
		return NULL;

	LIST_FOREACH(__params, pxParamItem) {
		param = IW_GET_LIST_NODE_DATA(pxParamItem);
		if (__param_name && strcmp(__param_name, param->name) == 0)
			break;
		if (__param_alias && strcmp(__param_alias, param->alias) == 0)
			break;
		param = NULL;
	}

	return param;
}

struct stParameter *get_parameter_w_task(const struct stMicroServiceInfo *const __msi, const iw_task_t __task_t, const char *const __param_name)
{
	if (__msi == NULL)
		return NULL;

	const iw_list_t *const pxConstList = &__msi->tasks;
	const struct stTaskInfo *ti = NULL;
	struct stParameter *param = NULL;

	if (pxConstList == NULL || _IW_LIST_IS_EMPTY(pxConstList))
		return NULL;

	if (__param_name == NULL || __param_name[0] == '\0')
		return NULL;

	LIST_FOREACH(pxConstList, pxItem) {
		ti = IW_GET_LIST_NODE_DATA(pxItem);

		if (__task_t != (iw_task_t)NULL && ti->task_t != __task_t) continue;

		param = get_parameter_from_param_list(&ti->user_params, __param_name, NULL);
		if (param != NULL)
			break;
		param = get_parameter_from_param_list(&ti->static_params, __param_name, NULL);
		if (param != NULL)
			break;
		param = get_parameter_from_param_list(&ti->flexible_params, __param_name, NULL);
		if (param != NULL)
			break;
		
		ti = NULL;
	}

	return param;
}

iw_queue_t get_input_queue_from_taskinfo(const struct stTaskInfo *const __ti, const size_t __index)
{
	if (__ti->input_count < __index + 1)
		return NULL;
	return __ti->input_queues[__index];
}

void set_output_param(struct stMicroServiceInfo *__pmsi, const enum ePARAM_TYPE __type, const size_t __size, const char *const __name)
{
	strncpy(__pmsi->output_param.name, __name, MS_MAX_NAME_LENGTH);
	__pmsi->output_param.value = _safe_alloc(__size);
	__pmsi->output_param.value_size = __size;
	__pmsi->output_param.type = __type;
	__pmsi->output_param.last_updated = 0; // get_tick_count();
}

/**
 * @brief output parameter 값 설정
 * @details 특정 Task에서 특정 이름을 가진 output parameter에 값을 설정
 * @param __ti Task 정보 구조체 포인터
 * @param __param_name output parameter 이름
 * @param __value 설정할 값
 */
void set_output_param_value(struct stTaskInfo *__ti, const char *__param_name, const AUTO __value, const enum ePARAM_TYPE __type, const size_t __size)
{
	struct stParameter *param = &__ti->parent->output_param;

	if(param != NULL){
		if (param->value) {
			free(param->value);
		}
		param->value = _safe_alloc(__size);
		param->value_size = __size;
		param->type = __type;
		if(param->value!=NULL)
			memcpy(param->value, __value, param->value_size);
		param->last_updated = get_tick_count(); //update
	}
}

/**
 * @brief 경량형 메시지 Object 획득
 * @details MicroService의 Parameter 정보 구조체로부터 경량형 메시지의 Object를 만들어 획득
 * @param __param user/static/flexible Parameter 정보 구조체 포인터
 * @return 생성된 경량형 메시지 Object, 실패 시 NULL
 */
PLWMObject new_lwm_object_from_type_value(const enum ePARAM_TYPE __type, AUTO __value)
{
	PLWMObject value = _new_lwm_empty_object_for_ms(__type);
	if (IS_DECIMAL(__type)) {
		int8_t int8_value;
		int16_t int16_value;
		int32_t int32_value;
		uint8_t uint8_value;
		uint16_t uint16_value;
		uint32_t uint32_value;
		long long_value;
		switch (__type) {
			case eP_INT8:
				memcpy(&int8_value, __value, sizeof(int8_t));
				long_value = int8_value;
				break;
			case eP_INT16:
				memcpy(&int16_value, __value, sizeof(int16_t));
				long_value = int16_value;
				break;
			case eP_INT32:
				memcpy(&int32_value, __value, sizeof(int32_t));
				long_value = int32_value;
				break;
			case eP_UINT8:
				memcpy(&uint8_value, __value, sizeof(uint8_t));
				long_value = uint8_value;
				break;
			case eP_UINT16:
				memcpy(&uint16_value, __value, sizeof(uint16_t));
				long_value = uint16_value;
				break;
			case eP_UINT32:
			default:
				memcpy(&uint32_value, __value, sizeof(uint32_t));
				long_value = uint32_value;
				break;
		}
		SET_LWM_INT_VALUE(value, long_value);
	}
	else if (__type == eP_FLOAT) {
		float float_value;
		memcpy(&float_value, __value, sizeof(float));
		SET_LWM_FLOAT_VALUE(value, float_value);
	}
	else if (__type == eP_DOUBLE) {
		float float_value;
		double double_value;
		memcpy(&double_value, __value, sizeof(double));
		float_value = (float)double_value;
		SET_LWM_FLOAT_VALUE(value, float_value);
	}
	else if (__type == eP_STRING) {
		// char *string_value;
		// __fn_get(__param, (AUTO *)&string_value);
		// SET_LWM_STRING_VALUE(value, string_value);
		SET_LWM_STRING_VALUE(value, __value);
	}
	else {
		delete_lwm_object(value);
		value = NULL;
	}
	return value;
}

void init_mservices()
{
	iw_init_list(&_mservices);
}

BOOL add_micro_service(const struct stMicroServiceInfo *const __pmsi)
{
	if (__pmsi == NULL) return FALSE;

	iw_list_node_t *msi_item = malloc(sizeof(iw_list_node_t));
	if (msi_item) {
		ZeroMemory(msi_item, sizeof(iw_list_node_t));
		_IW_SET_LIST_NODE_DATA(msi_item, __pmsi);
		iw_add_list_node(&_mservices, msi_item);
	}

	return TRUE;
}

PLWMessage make_device_registration_lwmsg(const int __if, const char *const __dri)
{
	PLWMessage lwm_msg = NEW_LWM_REQ_MESSAGE(__if, __dri, NULL);
	PLWMObject mis = NEW_LWM_ARRAY_OBJECT();
	const struct stMicroServiceInfo *pmsi = NULL;

	LIST_FOREACH(&_mservices, pxItem) {
		pmsi = IW_GET_LIST_NODE_DATA(pxItem);
		ADD_LWM_ARRAY_ITEM(mis, NEW_LWM_STRING_OBJECT(pmsi->ms_id));
		pmsi = NULL;
	}

	SET_LWM_BODY_OBJECT(lwm_msg, "mis", mis);

	return lwm_msg;
}

static BOOL _init_task_state(PSTTaskInfo __ti, void *__cond)
{
	return __ti->initial_state == I_S_READY;
}

PLWMessage make_device_information_lwmsg(const int __if, const char *const __dri, const char *const __di)
{
	PLWMessage lwm_msg = NEW_LWM_REQ_MESSAGE(__if, __dri, __di);
	PLWMObject mif = NEW_LWM_ARRAY_OBJECT();
	struct stMicroServiceInfo *pmsi = NULL;
	const struct stTaskInfo *ti = NULL;
	const struct stParameter *pparam = NULL;
	const struct stParameter *poparam = NULL;

	LIST_FOREACH(&_mservices, pxItem) {
		pmsi = IW_GET_LIST_NODE_DATA(pxItem);

		BOOL isRunning = (pmsi->state == RUNNING);
		if (!isRunning)
			pmsi->create(pmsi);

		PLWMObject dmi = NEW_LWM_DICT_OBJECT();
		PLWMObject dop = NEW_LWM_ARRAY_OBJECT();

		// TODO: input params
		if (__if == eLWM_IF_MS_INPUT_PARAM_SET) {
			
		}

		struct stTaskLink *result = _find_last_task_link(pmsi->task_link_head, _init_task_state, NULL);
		if (result) {
			ADD_LWM_ARRAY_ITEM(dop, NEW_LWM_STRING_OBJECT(result->output_param_name));
		}

		SET_LWM_OBJECT(dmi, "mi", NEW_LWM_STRING_OBJECT(pmsi->ms_id));
		// TODO: set input params
		if (LWM_ARRAY_LENGTH(dop) > 0)
			SET_LWM_OBJECT(dmi, "ops", dop);
		ADD_LWM_ARRAY_ITEM(mif, dmi);

		if (!isRunning)
			del_micro_service(pmsi);
		pmsi = NULL;
	}

	SET_LWM_BODY_OBJECT(lwm_msg, "mif", mif);

	return lwm_msg;
}

/**
 * @brief 기기 정보 등록 경량형 메시지 생성
 * @details Interface ID 3 Task 정보 등록 경량형 메시지를 생성
 * @param __dri dri
 * @return 생성된 경량형 메시지
 * @todo 재설계 및 설계에 따른 전체 재작성 필요
 */
PLWMessage make_device_information_lwmsg2(const char *const __dri, const uint32_t __res)
{
	PLWMessage lwm_msg = NEW_LWM_RES_MESSAGE(__dri, __res);
	PLWMObject tif = NEW_LWM_ARRAY_OBJECT();
	struct stMicroServiceInfo *pmsi = NULL;
	const struct stTaskInfo *ti = NULL;
	const struct stParameter *pparam = NULL;
	const struct stParameter *poparam = NULL;

	LIST_FOREACH(&_mservices, pxItem) {
		pmsi = IW_GET_LIST_NODE_DATA(pxItem);

		BOOL isRunning = (pmsi->state == RUNNING);
		if (!isRunning)
			pmsi->create(pmsi);

		LIST_FOREACH(&pmsi->tasks, pxTaskItem) {
			ti = IW_GET_LIST_NODE_DATA(pxTaskItem);
			if(ti->add_task_list == TRUE){

				PLWMObject dti = NEW_LWM_DICT_OBJECT();
				PLWMObject dsp = NEW_LWM_DICT_OBJECT();
				PLWMObject dfp = NEW_LWM_DICT_OBJECT();

				LIST_FOREACH(&ti->static_params, pxParamItem) {
					pparam = IW_GET_LIST_NODE_DATA(pxParamItem);
					PLWMObject value = NEW_LWM_OBJECT_FROM_PARAM(pparam);
					SET_LWM_OBJECT(dsp, pparam->alias, value);
					pparam = NULL;
				}

				LIST_FOREACH(&ti->flexible_params, pxParamItem) {
					pparam = IW_GET_LIST_NODE_DATA(pxParamItem);
					PLWMObject value = NEW_LWM_OBJECT_FROM_PARAM(pparam);
					SET_LWM_OBJECT(dfp, pparam->alias, value);
					pparam = NULL;
				}

				PLWMObject _ti = NEW_LWM_NUMBER_OBJECT();
				SET_LWM_INT_VALUE(_ti, ti->task_idx);
				SET_LWM_OBJECT(dti, "ti", _ti);
				SET_LWM_OBJECT(dti, "to", ti->offloading ? NEW_LWM_STRING_OBJECT("TRUE") : NEW_LWM_STRING_OBJECT("FALSE"));
				if (LWM_ARRAY_LENGTH(dsp) > 0)
					SET_LWM_OBJECT(dti, "sp", dsp);
				if (LWM_ARRAY_LENGTH(dfp) > 0)
					SET_LWM_OBJECT(dti, "fp", dfp);
				ADD_LWM_ARRAY_ITEM(tif, dti);
			}
		}

		if (!isRunning)
			del_micro_service(pmsi);
		pmsi = NULL;
	}

	SET_LWM_BODY_OBJECT(lwm_msg, "tif", tif);

	return lwm_msg;
}

BOOL create_micro_service(struct stMicroServiceInfo *const __pmsi)
{
	if (__pmsi == NULL) return FALSE;

	if (__pmsi->state == NOT_READY) {
		iw_lock_mutex(__pmsi->operation_lock);
		__pmsi->create(__pmsi);

		__pmsi->state = CREATED;		
		iw_unlock_mutex(__pmsi->operation_lock);
	}
	else {
		return FALSE;
	}

	return TRUE;
}


// 상록
/*

BOOL run_micro_service(struct stMicroServiceInfo *const __pmsi)
{
	struct stTaskInfo *psi = NULL;
	BOOL task_creation_fail = FALSE;

	if (__pmsi == NULL) return FALSE;

	if (__pmsi->state == CREATED) {
		iw_lock_mutex(__pmsi->operation_lock);
		__pmsi->state = RUNNING;

		LIST_FOREACH(&__pmsi->tasks, pxItem) {
			psi = IW_GET_LIST_NODE_DATA(pxItem);
			if (GET_INITIAL_ONOFF(psi)) {
				if (!run_task(psi, FALSE)) {
					task_creation_fail = TRUE;
					break;
				}
			}
		}

		if (task_creation_fail) {
			iw_unlock_mutex(__pmsi->operation_lock);
			stop_micro_service(__pmsi);
			return FALSE;
		}
		else {
			set_output_connect_line(__pmsi);
		}
		iw_unlock_mutex(__pmsi->operation_lock);
	}
	else {
		return FALSE;
	}

	return TRUE;
}
*/

// D.H.Kim
BOOL run_micro_service(struct stMicroServiceInfo *const __pmsi)
{
	struct stTaskInfo *psi = NULL;
	BOOL task_creation_fail = FALSE;

	if (__pmsi == NULL) {
		printf("[run_micro_service] stMicroServiceInfo is NULL\n");
		return FALSE;
	}

	if (__pmsi->state == CREATED) {
		iw_lock_mutex(__pmsi->operation_lock);
		__pmsi->state = RUNNING;

		LIST_FOREACH(&__pmsi->tasks, pxItem) {
			psi = IW_GET_LIST_NODE_DATA(pxItem);
			if (GET_INITIAL_ONOFF(psi)) {
				if (!run_task(psi, FALSE)) {
					task_creation_fail = TRUE;
					printf("[run_micro_service] task_creation_fail: %s\n", psi->name);
					break;
				}else{					
					printf("[run_micro_service] task_creation_success: %s\n", psi->name);
				}
			}
		}

		if (task_creation_fail) {
			iw_unlock_mutex(__pmsi->operation_lock);
			stop_task(__pmsi);
			return FALSE;
		}
		else {
			set_output_connect_line(__pmsi);
		}
		iw_unlock_mutex(__pmsi->operation_lock);
	}
	else if (__pmsi->state == STOPPED) {
		iw_lock_mutex(__pmsi->operation_lock);
		__pmsi->state = RUNNING;
		//printf("run_micro_service: MS Run\n");
		
		LIST_FOREACH(&__pmsi->tasks, pxItem) {
			psi = IW_GET_LIST_NODE_DATA(pxItem);

			if(psi->state == STOPPED){
				psi->state = RUNNING;
			}
		}
		iw_unlock_mutex(__pmsi->operation_lock);
	}
	else {
		printf("[run_micro_service] __pmsi->state error %d\n", psi->state);
		return FALSE;
	}

	return TRUE;
}

BOOL stop_micro_service(struct stMicroServiceInfo *const __pmsi)
{
	BOOL rtn = TRUE;
	struct stTaskInfo *psi = NULL;
	struct stParameter *poparam = NULL;
	//BOOL operation = NULL;

	if (__pmsi == NULL) return FALSE;

	if (__pmsi->state == RUNNING)
		stop_task(__pmsi);

	iw_lock_mutex(__pmsi->operation_lock);
	__pmsi->state = NOT_READY;

	_del_task_link(__pmsi->task_link_head);
	__pmsi->task_link_head = NULL;

	LIST_FOREACH_TO_DEL(&__pmsi->tasks, pxItem) {
		psi = IW_GET_LIST_NODE_DATA(pxItem);

		if (psi) {
			if (psi->task_t)
				delete_task(psi->task_t);

			_del_param_list(&psi->user_params);
			_del_param_list(&psi->static_params);
			_del_param_list(&psi->flexible_params);

			for (int i=0;i<psi->input_count;i++) {
				if (psi->input_queues[i])
					delete_queue(psi->input_queues[i]);
				psi->input_queues[i] = NULL;
			}
			if (psi->input_queues)
				free(psi->input_queues);
			psi->input_queues = NULL;

			if (psi->output_ports)
				free(psi->output_ports);
			psi->output_ports = NULL;

			free(psi);
			psi = NULL;
		}

		iw_remove_list_node(&__pmsi->tasks, pxItem);
		free(pxItem);
	}

	if (__pmsi->output_param.value) {
		free(__pmsi->output_param.value);
		__pmsi->output_param.value = NULL;
	}

	_del_param_list(&__pmsi->input_params);

	LIST_FOREACH_TO_DEL(&__pmsi->connect_lines, pxLineItem) {
		PSTConnectLine line = IW_GET_LIST_NODE_DATA(pxLineItem);
		if (line) {
			free(line);
			line = NULL;
		}
		iw_remove_list_node(&__pmsi->connect_lines, pxLineItem);
		free(pxLineItem);
	}
	__pmsi->current_task_link = NULL;

	// create MS
	__pmsi->create(__pmsi);
	__pmsi->state = CREATED;		

	iw_unlock_mutex(__pmsi->operation_lock);
	
	/*operation = del_micro_service(__pmsi);
	if(operation)
		printf("[stop_micro_service] del_micro_service SUCCESS\n");
	else{
		printf("[stop_micro_service] del_micro_service FAIL\n");
		return FALSE;
	}

	operation = create_micro_service(__pmsi);
	if(operation)
		printf("[stop_micro_service] create_micro_service SUCCESS\n");
	else{
		printf("[stop_micro_service] create_micro_service FAIL\n");
		return FALSE;
	}*/

	return rtn;
}

/*

BOOL stop_micro_service(struct stMicroServiceInfo *const __pmsi)
{
	BOOL rtn = NULL;

	if(del_micro_service(__pmsi)){
		rtn = create_micro_service(__pmsi);
	}else{
		return rtn;
	}

	return rtn;
}
BOOL stop_micro_service(struct stMicroServiceInfo *const __pmsi)
{
	struct stTaskInfo *psi = NULL;

	if (__pmsi == NULL) return FALSE;

	if (__pmsi->state == RUNNING) {
		iw_lock_mutex(__pmsi->operation_lock);
		__pmsi->state = STOPPED;
		//printf("stop_micro_service: MS Stop\n");
		
		LIST_FOREACH(&__pmsi->tasks, pxItem) {
			psi = IW_GET_LIST_NODE_DATA(pxItem);

			if(psi->state == RUNNING || psi->state == WAITING){
				psi->state = STOPPED;
				psi->reset = TRUE;
				//printf("%s state STOPPED & reset TRUE\n", psi->name);
			}
			//else				
				//printf("%s state Not RUNNING or WAITING\n", psi->name);
		}
		
		iw_unlock_mutex(__pmsi->operation_lock);
	}
	else {
		return FALSE;
	}

	return TRUE;
}*/
// End of D.H.Kim

static void _del_param_list(iw_list_t *const pparam_list)
{
	struct stParameter *pparam = NULL;

	LIST_FOREACH_TO_DEL(pparam_list, pxParamItem) {
		pparam = IW_GET_LIST_NODE_DATA(pxParamItem);
		if (pparam) {
			if (pparam->value)
				free(pparam->value);
			pparam->value = NULL;
			free(pparam);
			pparam = NULL;
		}
		iw_remove_list_node(pparam_list, pxParamItem);
		free(pxParamItem);
	}
}

static void _del_task_link(struct stTaskLink *task_link)
{
	if (task_link == NULL)
		return;
	if (task_link->connect_link_head) {
		LIST_FOREACH_TO_DEL(task_link->connect_link_head, pxItem) {
			iw_remove_list_node(task_link->connect_link_head, pxItem);
			free(pxItem);
		}
		free(task_link->connect_link_head);
	}
	if (task_link->next_on)
		_del_task_link(task_link->next_on);
	if (task_link->next_off)
		_del_task_link(task_link->next_off);
	free(task_link);
}

// Modified by D.H.Kim
// Original name was stop_micro_service()
// New stop_micro_service() is used for pause to tasks
BOOL stop_task(struct stMicroServiceInfo *const __pmsi)
{
	const struct stTaskInfo *psi = NULL;

	if (__pmsi == NULL) return FALSE;

	if (__pmsi->state == RUNNING || __pmsi->state == STOPPED) {
		iw_lock_mutex(__pmsi->operation_lock);
		__pmsi->state = STOPPED;

		LIST_FOREACH(&__pmsi->tasks, pxItem) {
			psi = IW_GET_LIST_NODE_DATA(pxItem);
			if (psi->task_t){
				suspend_task(psi->task_t);
				//printf("suspend_task %s\n", psi->name);
			}
			if (psi->task_t){
				resume_task(psi->task_t);
				//printf("resume_task %s\n", psi->name);
			}
		}

		__pmsi->latest_dri[0] = '\0';
		iw_unlock_mutex(__pmsi->operation_lock);
	}
	else {
		return FALSE;
	}

	return TRUE;
}


BOOL del_micro_service(struct stMicroServiceInfo *const __pmsi)
{
	struct stTaskInfo *psi = NULL;
	struct stParameter *poparam = NULL;

	if (__pmsi == NULL) return FALSE;

	if (__pmsi->state == RUNNING)
		stop_task(__pmsi);

	iw_lock_mutex(__pmsi->operation_lock);
	__pmsi->state = NOT_READY;

	_del_task_link(__pmsi->task_link_head);
	__pmsi->task_link_head = NULL;

	LIST_FOREACH_TO_DEL(&__pmsi->tasks, pxItem) {
		psi = IW_GET_LIST_NODE_DATA(pxItem);

		if (psi) {
			if (psi->task_t)
				delete_task(psi->task_t);

			_del_param_list(&psi->user_params);
			_del_param_list(&psi->static_params);
			_del_param_list(&psi->flexible_params);

			for (int i=0;i<psi->input_count;i++) {
				if (psi->input_queues[i])
					delete_queue(psi->input_queues[i]);
				psi->input_queues[i] = NULL;
			}
			if (psi->input_queues)
				free(psi->input_queues);
			psi->input_queues = NULL;

			if (psi->output_ports)
				free(psi->output_ports);
			psi->output_ports = NULL;

			free(psi);
			psi = NULL;
		}

		iw_remove_list_node(&__pmsi->tasks, pxItem);
		free(pxItem);
	}

	if (__pmsi->output_param.value) {
		free(__pmsi->output_param.value);
		__pmsi->output_param.value = NULL;
	}

	_del_param_list(&__pmsi->input_params);

	LIST_FOREACH_TO_DEL(&__pmsi->connect_lines, pxLineItem) {
		PSTConnectLine line = IW_GET_LIST_NODE_DATA(pxLineItem);
		if (line) {
			free(line);
			line = NULL;
		}
		iw_remove_list_node(&__pmsi->connect_lines, pxLineItem);
		free(pxLineItem);
	}
	__pmsi->current_task_link = NULL;

	iw_unlock_mutex(__pmsi->operation_lock);

	return TRUE;
}

struct stMicroServiceInfo *get_micro_service_info_by_id(const char *const __ms_id)
{
	struct stMicroServiceInfo *pmsi = NULL;

	if (__ms_id == NULL || __ms_id[0] == '\0')
		return pmsi;

	LIST_FOREACH(&_mservices, pxItem) {
		pmsi = IW_GET_LIST_NODE_DATA(pxItem);
		if (strcmp(__ms_id, pmsi->ms_id) == 0)
			break;
		pmsi = NULL;
	}

	return pmsi;
}

#include "iw_fw_base_tasks.h"

void print_task_state(struct stTaskInfo *__ti)
{
	if(__ti->state == CREATED)
		printf("%s Task state: CREATED\n", __ti->name);
	else if(__ti->state == STOPPED)
		printf("%s Task state: STOPPED\n", __ti->name);
	else if(__ti->state == RUNNING)
		printf(" %s Task state: RUNNING\n", __ti->name);
	else if(__ti->state == WAITING)
		printf("%s Task state: WAITING\n", __ti->name);
	else 
		printf("%s Task state: unknown\n", __ti->name);

}

BOOL is_tasks_running(const struct stMicroServiceInfo *const __ms_info)
{
	struct stTaskInfo *psi = NULL;
	BOOL running = FALSE;

	if (__ms_info == NULL) return FALSE;

	if (__ms_info->state != RUNNING){
		return FALSE;
	}

	LIST_FOREACH(&__ms_info->tasks, pxItem) {
		psi = IW_GET_LIST_NODE_DATA(pxItem);
		if (psi->task_fn != base_task_type3 && psi->task_fn != base_task_type4 && psi->state == RUNNING) {
			// IS_ACTIVE_STATE()?
			running = TRUE;
			break;
		}
	}

	return running;
}

BOOL run_task(struct stTaskInfo *__ti, BOOL __force)
{
	if (!__force && IS_ACTIVE_STATE(__ti->state)){
		_DPRINTK("%s task IS_ACTIVE_STATE(%d) error.\n", __ti->name, __ti->state);
		return FALSE;
		}
	if (__ti->task_t)
		delete_task(__ti->task_t);
	__ti->task_t = (iw_task_t)NULL;
	__ti->state = STOPPED;
	//__ti->reset = FALSE;

	PSTProcessingParam pparam = _safe_alloc(sizeof(STProcessingParam));
	if(pparam != NULL){
		pparam->task_info = __ti;
		if (__ti->init_fn) {
			STProcessingReturn initReturn = __ti->init_fn(pparam);
			if (initReturn.is_abort){
				printf("[D][run_task] task init error.\n");
				return FALSE;
			}
			pparam->user_data = initReturn.user_data;
		}
		// init에서 반드시 1 이상이 설정되어야 함
		if (__ti->input_item_size > 0)
			pparam->input_data = _safe_alloc(__ti->input_item_size);
		else if (__ti->lwmsg_input_port >= 0)
			pparam->input_data = _safe_alloc(sizeof(PLWMessage));
		else{
			printf("[D][run_task] task input_data error.\n");
			return FALSE;
		}

		__ti->task_t = create_task(__ti->task_fn, __ti->name, IW_MINIMAL_STACK_SIZE, pparam, __ti->priority, NULL);
		__ti->state = CREATED;
	}

	return TRUE;
}

BOOL task_onoff(struct stTaskInfo *__ti, BOOL __onoff)
{
	BOOL rtn = FALSE;

	if (__ti == NULL){
		printf("[D][task_onoff] task info error.\n");
		return rtn;
	}

	if (!__ti->offloading){
		printf("[D][task_onoff] %s is non on/off task.\n", __ti->name);
		return rtn;
	}	

	// Task ON
	if (__onoff) {
		BOOL is_active = IS_ACTIVE_STATE(__ti->state);
		if (is_active) {
			_DPRINTK("%s task is already active.\n", __ti->name);
			return rtn;
		}
		//rtn = run_task(__ti, FALSE);
		//if(rtn){
			_DPRINTK("%s task is activated.\n", __ti->name);
			__ti->initial_state = I_S_ACTIVE;
			rtn = TRUE;
		//}
	}
	// Task OFF
	else {
		_DPRINTK("offloading %s\n", __ti->name);
		// Delete Task
		if (__ti->task_t)
			delete_task(__ti->task_t);
		__ti->task_t = (iw_task_t)NULL;

		// Task Connection Rearrangement
		for (int i=0;i<__ti->input_count;i++) {
			if (__ti->input_queues[i])
				delete_queue(__ti->input_queues[i]);
			if (i == __ti->lwmsg_input_port) {
				__ti->input_queues[i] = create_queue(mainQUEUE_LENGTH, sizeof(PLWMessage));
			}
			else if (__ti->input_item_size > 0) {
				__ti->input_queues[i] = create_queue(mainQUEUE_LENGTH, __ti->input_item_size);
			}
		}
		for (int i=0;i<__ti->output_count;i++) {
			__ti->output_ports[i] = NULL;
		}

		__ti->state = STOPPED;
		__ti->initial_state = I_S_STOP;
		printf("[D][task_onoff] ");
		print_task_state(__ti);
		rtn = TRUE;
	}

	set_output_connect_line(__ti->parent);

	return rtn;
}

PSTConnectLine add_connect_line(PSTMicroServiceInfo __pmsi, const char *__start_task_name, uint32_t __output_port_no, const char *__dest_task_name, uint32_t __input_port_no)
{
	STConnectLine cline;
	cline.start_task = get_task_info_by_name(__pmsi, __start_task_name);
	cline.dest_task = get_task_info_by_name(__pmsi, __dest_task_name);
	if (cline.start_task == NULL || cline.start_task->output_count < __output_port_no + 1)
		return NULL;
	if (cline.dest_task == NULL || cline.dest_task->input_count < __input_port_no + 1)
		return NULL;
	// _DPRINTK("CLINE: %s == %s, %s == %s?\n", __start_task_name, cline.start_task->name, __dest_task_name, cline.dest_task->name);

	cline.output_port_no = __output_port_no;
	cline.input_port_no = __input_port_no;

	PSTConnectLine pcline = _safe_alloc(sizeof(STConnectLine));
	*pcline = cline;
	iw_list_node_t *cl_item = _safe_alloc(sizeof(iw_list_node_t));
	_IW_SET_LIST_NODE_DATA(cl_item, pcline);
	iw_add_list_node(&__pmsi->connect_lines, cl_item);

	return pcline;
}

static void _find_task_links(struct stTaskLink *__tl, const struct stTaskInfo *__ti, iw_list_t *__list)
{
	if (__tl == NULL || __ti == NULL || __list == NULL)
		return;

	_find_task_links(__tl->next_on, __ti, __list);
	_find_task_links(__tl->next_off, __ti, __list);
	if (__tl->task == __ti) {
		iw_list_node_t *item = _safe_alloc(sizeof(iw_list_node_t));
		_IW_SET_LIST_NODE_DATA(item, __tl);
		iw_add_list_node(__list, item);
	}
}

static BOOL _add_task_link(PSTMicroServiceInfo __pmsi, PSTTaskInfo __start_ti, PSTTaskInfo __dest_ti)
{
	iw_list_t start_tls;
	iw_init_list(&start_tls);
	if (__pmsi->task_link_head == NULL) {
		__pmsi->task_link_head = _safe_alloc(sizeof(STTaskLink));
		__pmsi->task_link_head->task = __start_ti;
	}

	_find_task_links(__pmsi->task_link_head, __start_ti, &start_tls);

	LIST_FOREACH(&start_tls, pxItem) {
		PSTTaskLink start_tl = IW_GET_LIST_NODE_DATA(pxItem);
		PSTTaskLink dest_on_tl = _safe_alloc(sizeof(STTaskLink));
		dest_on_tl->task = __dest_ti;
		start_tl->next_on = dest_on_tl;
		if (__start_ti->offloading) {
			PSTTaskLink dest_off_tl = _safe_alloc(sizeof(STTaskLink));
			dest_off_tl->task = __dest_ti;
			start_tl->next_off = dest_off_tl;
		}
	}

	LIST_FOREACH_TO_DEL(&start_tls, pxItem) {
		iw_remove_list_node(&start_tls, pxItem);
		free(pxItem);
	}

	return TRUE;
}

void make_task_links(PSTMicroServiceInfo __pmsi)
{
	PSTTaskInfo left = NULL;
	PSTTaskInfo right = NULL;

	LIST_FOREACH(&__pmsi->tasks, pxItem) {
		PSTTaskInfo ti = IW_GET_LIST_NODE_DATA(pxItem);
		if (left == NULL) {
			left = ti;
			continue;
		}
		else if (right == NULL) {
			right = ti;
		}
		else {
			left = right;
			right = ti;
		}

		if (left != NULL && right != NULL)
			_add_task_link(__pmsi, left, right);
	}

	// 마지막에 들어간 TaskLink가 오프로딩 가능하다면 추가 분기가 필요함
	// 즉, Connect Link 넣을 공간이 추가적으로 필요함
	if (right && right->offloading) {
		_add_task_link(__pmsi, right, NULL);
	}
}

iw_list_t *make_connect_link_list(const uint32_t __count, ...)
{
	iw_list_t *rtn = _safe_alloc(sizeof(iw_list_t));
	iw_init_list(rtn);

	va_list ap;
	va_start(ap, __count);
	for (int i=0; i < __count; i++) {
		PSTConnectLine cline = va_arg(ap, PSTConnectLine);
		iw_list_node_t *item = _safe_alloc(sizeof(iw_list_node_t));
		if(item != NULL){
			_IW_SET_LIST_NODE_DATA(item, cline);
			iw_add_list_node(rtn, item);
		}
	}
	va_end(ap);

	return rtn;
}

static struct stTaskLink *_find_last_task_link(struct stTaskLink *__tl, BOOL(*__test)(PSTTaskInfo, void *), void *__cond)
{
	if (__tl == NULL || __test == NULL)
		return NULL;

	while (__tl != NULL) {
		if (__tl->next_on == NULL && __tl->next_off == NULL)
			break;

		if (__test(__tl->task, __cond))
			__tl = __tl->next_on;
		else
			__tl = __tl->next_off;
	}

	return __tl;
}

static BOOL _curr_task_state(PSTTaskInfo __ti, void *__cond)
{
	return IS_ACTIVE_STATE(__ti->state);
}

static BOOL _onoff_test(PSTTaskInfo __ti, void *__cond)
{
	iw_list_t *off_tasks = (iw_list_t *)__cond;
	LIST_FOREACH(off_tasks, pxItem) {
		PSTTaskInfo ti = IW_GET_LIST_NODE_DATA(pxItem);
		if (ti == __ti) {
			// _DPRINTK("[off test] %s == %s\n", __ti->name, ti->name);
			return FALSE;
		}
	}
	return TRUE;
}

void set_connect_link(PSTMicroServiceInfo __pmsi, iw_list_t *__clinks, const char *__oname, const uint32_t __count, ...)
{
	iw_list_t off_tasks;
	iw_init_list(&off_tasks);

	if(__pmsi != NULL){
		if (__count > 0) {
			va_list ap;
			va_start(ap, __count);
			for (int i=0; i < __count; i++) {
				char *cline = va_arg(ap, char *);
				PSTTaskInfo ti = get_task_info_by_name(__pmsi, cline);
				if (ti) {
					iw_list_node_t *item = _safe_alloc(sizeof(iw_list_node_t));
					if(item!=NULL){
						_IW_SET_LIST_NODE_DATA(item, ti);
						iw_add_list_node(&off_tasks, item);
					}
				}
			}
			va_end(ap);
		}

		PSTTaskLink result = _find_last_task_link(__pmsi->task_link_head, _onoff_test, &off_tasks);
		if (result) {
			result->connect_link_head = __clinks;
			strcpy(result->output_param_name, __oname);
			// _DPRINTK("set connect_link_head to %s(%d) with %d conditions\n", result->task->name, (uint32_t)result, IW_GET_LIST_NODE_NUM(&off_tasks));
		}

		LIST_FOREACH_TO_DEL(&off_tasks, pxItem) {
			iw_remove_list_node(&off_tasks, pxItem);
			free(pxItem);
		}
	}
}

void set_output_connect_line(struct stMicroServiceInfo *__pmsi)
{
	iw_list_t *connect_link_head = NULL;
	struct stTaskLink *result = _find_last_task_link(__pmsi->task_link_head, _curr_task_state, NULL);
	if (result) {
		connect_link_head = result->connect_link_head;
		__pmsi->current_task_link = result;
		set_output_param(__pmsi, eP_UNKNOWN, 0, __pmsi->current_task_link->output_param_name);
	}
	// 올바른 connect link를 찾은 경우
	if (connect_link_head != NULL) {
		LIST_FOREACH(connect_link_head, pxItem) {
			struct stConnectLine *cline = IW_GET_LIST_NODE_DATA(pxItem);
			if (cline->output_port_no < cline->start_task->output_count)
				cline->start_task->output_ports[cline->output_port_no] = cline;
		}
	}
}
