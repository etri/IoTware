/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "iw_fw_types_internal.h"
#include "iw_fw_lwMsgUtils.h"

struct stTaskInfo;
struct stMicroServiceInfo;


enum {
	USER_PARAM = 1,
	STATIC_PARAM,
	FLEXIBLE_PARAM,
	MS_INPUT_PARAM
};

typedef struct stProcessingReturn {
	bool is_abort;
	void *user_data;
} STProcessingReturn, *PSTProcessingReturn;

typedef struct stProcessingParam {
	struct stTaskInfo *task_info;
	void *input_data;
	int32_t queue_num;
	uint32_t sleep_ms;
	uint32_t total_sleep_ms;
	uint32_t total_sleep_count;
	void *user_data;
} STProcessingParam, *PSTProcessingParam;

typedef struct stProcessingReturn (*ProcessingHandler)(struct stProcessingParam *);

typedef struct stConnectLine {
	struct stTaskInfo *start_task;
	uint32_t output_port_no;
	struct stTaskInfo *dest_task;
	uint32_t input_port_no;
} STConnectLine, *PSTConnectLine;
typedef const struct stConnectLine * LPCConnectLine;

typedef struct stParameter {
	void *value;
	size_t value_size;
	enum ePARAM_TYPE type;
	char name[MS_MAX_NAME_LENGTH+1];
	char alias[MS_MAX_NAME_LENGTH+1];
	enum ePARAM_ATTR attr : 2;
	iw_tick_t last_updated;
} STParameter, *PSTParameter;
typedef const struct stParameter * LPCParameter;

typedef struct stTaskLink {
	struct stTaskInfo *task;
	struct stTaskLink *next_on;
	struct stTaskLink *next_off;
	iw_list_t *connect_link_head;
	char output_param_name[30];
} STTaskLink, *PSTTaskLink;
typedef const struct stTaskLink * LPCTaskLink;


typedef struct stTaskInfo {
	char name[MS_MAX_NAME_LENGTH+1];
	iw_task_t task_t;
	void(*task_fn)(void *);
	uint32_t task_idx;
	ProcessingHandler init_fn;
	ProcessingHandler before_fn;
	ProcessingHandler after_fn;
	//ProcessingHandler reset_fn;
	struct stMicroServiceInfo *parent;
	iw_queue_t *input_queues;
	struct stConnectLine **output_ports;
	void *output_ptr;
	size_t input_item_size;
	size_t output_item_size;
	iw_list_t user_params;
	iw_list_t static_params;
	iw_list_t flexible_params;
	uint32_t input_count;
	uint32_t output_count;
	int32_t lwmsg_input_port;
	enum eSTATE_TYPE initial_state;
	uint32_t priority;
	BOOL offloading;
	BOOL add_task_list;
	enum eSTATE_TYPE state;
} STTaskInfo, *PSTTaskInfo;
typedef const struct stTaskInfo * LPCTaskInfo;

typedef struct stMicroServiceInfo {
	char ms_id[MS_MAX_NAME_LENGTH+1];
	char name[MS_MAX_NAME_LENGTH+1];
	iw_list_t tasks;
	iw_mutex_t operation_lock;
	struct stTaskLink *task_link_head;
	struct stTaskLink *current_task_link;
	iw_list_t connect_lines;
	enum eSTATE_TYPE state;
	char latest_dri[MS_MAX_NAME_LENGTH+1];
	iw_list_t input_params;
	struct stParameter output_param;
	void(*create)(struct stMicroServiceInfo *);
} STMicroServiceInfo, *PSTMicroServiceInfo;
typedef const struct stMicroServiceInfo * LPCMicroServiceInfo;


#define FN_PROTOTYPE_MAKE_MS(__func_name) struct stMicroServiceInfo *make_##__func_name(const char *const name, const char *const ms_id)
#define FN_EXTERN_MAKE_MS(__func_name) extern struct stMicroServiceInfo *make_##__func_name(const char *const name, const char *const ms_id)

#define FN_CALL_MAKE_MS(__name,__ms_id) make_##__name(#__name, __ms_id)

#define FN_CALL_MAKE_MS_W_FNAME(__func_name,__name,__ms_id) make_##__func_name(__name, __ms_id)

#define FN_MAKE_MS(__func_name) \
	struct stMicroServiceInfo *make_##__func_name(const char *const name, const char *const ms_id) \
	{ \
		struct stMicroServiceInfo *rtn = malloc(sizeof(struct stMicroServiceInfo)); \
		if (rtn) { \
			ZeroMemory(rtn, sizeof(struct stMicroServiceInfo)); \
			strncpy(rtn->ms_id, ms_id, MS_MAX_NAME_LENGTH); \
			strncpy(rtn->name, name, MS_MAX_NAME_LENGTH); \
			iw_init_list(&rtn->tasks); \
			iw_init_list(&rtn->connect_lines); \
			iw_init_list(&rtn->input_params); \
			iw_create_mutex(&rtn->operation_lock); \
			rtn->create = create; \
		} \
		return rtn; \
	}

#define FN_PROTOTYPE_TASK(__func_name) void __func_name(void *)
#define FN_EXTERN_TASK(__func_name) 

#define ADD_NEW_MICRO_SERVICE(__name,__ms_id) add_micro_service(FN_CALL_MAKE_MS(__name, __ms_id));
#define ADD_NEW_MICRO_SERVICE_W_FNAME(__func_name,__name,__ms_id) add_micro_service(FN_CALL_MAKE_MS_W_FNAME(__func_name, __name, __ms_id));

#define IS_pMS_RUNNING(__ms_info) ( __ms_info->state == RUNNING )

#define IS_MS_RUNNING(__ms_info) ( __ms_info.state == RUNNING )

#define SET_TASK_RUNNING(__taskinfo) ( __taskinfo->state = RUNNING )
#define SET_TASK_WAITING(__taskinfo) ( __taskinfo->state = WAITING )

#define IS_MS_TASKS_RUNNING(__ms_info) is_tasks_running(__ms_info)

#define CREATE_TASK__STACKSIZE(__ms_info,__name,__func,__stacksize) \
	{ \
		struct stTaskInfo *ti = get_task_info_by_name(__ms_info, __name); \
		ti->task_t = create_task(__func, __name, __stacksize, ti, ti->priority, NULL); \
	}

#define CREATE_TASK(__ms_info,__name,__func) \
	{ \
		struct stTaskInfo *ti = get_task_info_by_name(__ms_info, __name); \
		ti->task_t = create_task(__func, __name, IW_MINIMAL_STACK_SIZE, ti, ti->priority, NULL); \
	}

#define RUN_TASK(__ti) run_task(__ti, FALSE)
#define RUN_TASK_FORCE(__ti) run_task(__ti, TRUE)

#define ADD_NEW_TASK_START(__parent,__func_name,__alias,__inputs,__outputs,__lwmsg_input) \
	{ \
		struct stTaskInfo *__self = add_new_task(__parent, __func_name, NULL, NULL, NULL, __alias, __inputs, __outputs, __lwmsg_input); \
		
#define ADD_NEW_TASK_END() \
	}

#define SET_INIT_FUNCTION(__func) __self->init_fn = __func
#define SET_PREPROCESSING_FUNCTION(__func) __self->before_fn = __func
#define SET_POSTPROCESSING_FUNCTION(__func) __self->after_fn = __func

#define SET_OFFLOADING(__enable) __self->offloading = __enable

#define SET_ADD_TASK_LIST(__enable) __self->add_task_list = __enable

#define SET_INITIAL_ONOFF(__enable) \
	{ \
		if (!__enable) \
			__self->initial_state = I_S_STOP; \
	}
#define GET_INITIAL_ONOFF(__ti) (__ti->initial_state != I_S_STOP)

#define ADD_PARAM(__param_type,__type,__name,__alias,__value) \
	{ \
		__type _temp = __value; \
		add_task_param(__param_type, __self, eP_ ## __type, __name, __alias, (const AUTO)&_temp, sizeof(__type)); \
	}
#define ADD_OUTPUT_PARAM(__type,__name) add_output_param(self, eP_ ## __type, sizeof(__type), __name)

#define MAKE_TASK_LINKS() make_task_links(self)
#define ADD_CONNECT_LINE(__start,__sport,__dest,__dport) add_connect_line(self, __start, __sport, __dest, __dport)
#define MAKE_CONNECT_LINK(__count,...) make_connect_link_list(__count, ## __VA_ARGS__)
#define SET_CONNECT_LINK(__clinks,__oname,__count,...) set_connect_link(self, __clinks, __oname, __count, ## __VA_ARGS__)
#define SET_OUTPUT_CONNECT_LINE() set_output_connect_line(self)

#define SET_OUTPUT_PARAM_VALUE(__taskinfo,__value,__type) set_output_param_value(__taskinfo, __taskinfo->parent->current_task_link->output_param_name, (const AUTO)&__value, eP_ ## __type, sizeof(__type))

#define GET_INPUT_PORT_QUEUE(__ti,__port) get_input_queue_from_taskinfo(__ti, __port)
#define DECLARE_INPUT_PORT_QUEUE(__name,__taskinfo,__index) \
	iw_queue_t __name = get_input_queue_from_taskinfo(__taskinfo, __index);

#define HAS_LWMSG_INPUT_QUEUE(__taskinfo) (__taskinfo->lwmsg_input_port >= 0)
#define GET_LWMSG_INPUT_QUEUE(__taskinfo) (__taskinfo->input_queues[__taskinfo->lwmsg_input_port])

#define SET_OUTPUT_EXISTS(__rtn, __flag) rtn.user_data = (void *)__flag;
#define SET_OUTPUT_VARIABLE_W_SIZE(__taskinfo,__value,__value_size) memcpy(__taskinfo->output_ptr, &__value, __value_size)
#define SET_OUTPUT_VARIABLE(__taskinfo,__value) SET_OUTPUT_VARIABLE_W_SIZE(__taskinfo,__value,sizeof(__value))
#define SET_OUTPUT_VALUE(__taskinfo,__type,__value) \
	{ \
		__type temp = __value; \
		memcpy(__taskinfo->output_ptr, &temp, sizeof(__type)); \
	}
#define SET_OUTPUT_POSITIVE(__taskinfo) SET_OUTPUT_VALUE(__taskinfo,EVENT,1)
#define SET_OUTPUT_NEGATIVE(__taskinfo) SET_OUTPUT_VALUE(__taskinfo,EVENT,0)



#define NEW_LWM_VALUE_OBJECT(__type,__value) new_lwm_object_from_type_value(__type, (AUTO)__value)


void set_device_is_registered(BOOL __flag);
BOOL get_device_is_registered();

struct stTaskInfo *add_new_task(struct stMicroServiceInfo *__parent, void(*__fp)(void *), ProcessingHandler __init_fp, ProcessingHandler __before_fp, ProcessingHandler __after_fp, const char *__alias, const uint32_t __input_count, const uint32_t __output_count, const int32_t __lwmsg_input);
void set_inout_item_size(struct stTaskInfo *__self, const size_t __input_item_size, const size_t __output_item_size);
void add_task_param(int __param_type, struct stTaskInfo *__ti, const enum ePARAM_TYPE __type, const char *const __name, const char *const __alias, const AUTO __value, const size_t __value_size);
void add_ms_input_param(struct stMicroServiceInfo *__pmsi, const enum ePARAM_TYPE __type, const char *const __name, const char *const __alias, const AUTO __value, const size_t __value_size);
void add_output_param(struct stMicroServiceInfo *__pmsi, const enum ePARAM_TYPE __type, const size_t __size, const char *const __name);

struct stTaskInfo *get_task_info_by_name(const struct stMicroServiceInfo *const __msi, const char *const __name);
struct stTaskInfo *get_task_info_by_idx(const struct stMicroServiceInfo *const __msi, const size_t __index);
const void *get_parameter_value_ptr(const struct stTaskInfo *const __taskinfo, const char *const __param_name);
const void *get_ms_input_parameter_value_ptr(const struct stMicroServiceInfo *const __pmsi, const char *const __param_name);
struct stParameter *get_parameter_from_param_list(const iw_list_t *const __params, const char *const __param_name, const char *const __param_alias);
struct stParameter *get_parameter_w_task(const struct stMicroServiceInfo *const __msi, const iw_task_t __task_t, const char *const __param_name);
iw_queue_t get_input_queue_from_taskinfo(const struct stTaskInfo *const __ti, const size_t __index);
void set_output_param_value(struct stTaskInfo *__ti, const char *__param_name, const AUTO __value, const enum ePARAM_TYPE __type, const size_t __size);
PLWMObject new_lwm_object_from_type_value(const enum ePARAM_TYPE __type, AUTO __value);

void init_mservices();
BOOL add_micro_service(const struct stMicroServiceInfo *const __pmsi);
PLWMessage make_device_registration_lwmsg(const int __if, const char *const __dri);
PLWMessage make_device_information_lwmsg(const int __if, const char *const __dri, const char *const __di);
PLWMessage make_device_information_lwmsg2(const char *const __dri, const uint32_t __res);
BOOL create_micro_service(struct stMicroServiceInfo *const __pmsi);
BOOL run_micro_service(struct stMicroServiceInfo *const __pmsi);
BOOL stop_micro_service(struct stMicroServiceInfo *const __pmsi);
BOOL stop_task(struct stMicroServiceInfo *const __pmsi);
BOOL del_micro_service(struct stMicroServiceInfo *const __pmsi);
struct stMicroServiceInfo *get_micro_service_info_by_id(const char *const __ms_id);
BOOL is_tasks_running(const struct stMicroServiceInfo *const __ms_info);

BOOL run_task(struct stTaskInfo *__ti, BOOL __force);
BOOL task_onoff(struct stTaskInfo *__ti, BOOL __onoff);

PSTConnectLine add_connect_line(PSTMicroServiceInfo __pmsi, const char *__start_task_name, uint32_t __output_port_no, const char *__dest_task_name, uint32_t __input_port_no);
void make_task_links(PSTMicroServiceInfo __pmsi);
iw_list_t *make_connect_link_list(const uint32_t __count, ...);
void set_connect_link(PSTMicroServiceInfo __pmsi, iw_list_t *__clinks, const char *__oname, const uint32_t __count, ...);
void set_output_connect_line(struct stMicroServiceInfo *__pmsi);
