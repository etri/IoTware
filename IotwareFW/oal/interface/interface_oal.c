/*
Copyright (C) <2020> <ETRI>

This License and Service Agreement (LSA) applies to all works and their derivative works based on source form version and object form version of IoTware Project. Currently, the LSA for IoTware Project has two policies, 'Open Source License' and 'Commercial License'. Therefore, all works including the source code and executable code of IoTware Project and derivative works based thereon are subject to either 'Open Source License' or 'Commercial License' depending on the user's needs and purpose. Details related to the selection of the applicable license are specified in this LSA. If you use any or all of IoTware Project in any form, you are deemed to have consented to this LSA. If you breach any of the terms and conditions set forth in this LSA, you are solely responsible for any losses or damages incurred by Electronics and Communications Research Institute (ETRI), and ETRI assume no responsibility for you or any third party.
If you use the source form version or object form version of IoTware Project in whole or in part to develop a code or a derivative work, and you want to commercialize the result in some form, you will be covered under a commercial license. And if you are subject to a commercial license, the contract for the use of IoTware Project is subject to TECHNOLOGY LICENSE AGREEMENT of ETRI. You acknowledge that ETRI has all legal rights, title and interest, including intellectual property rights in the IoTware Project (regardless of whether such intellectual property rights are registered or where such rights exist) and agree with no objection thereto. Except as provided in a subsidiary agreement, nothing in this LSA grants you the right to use IoTware Project or the name, service mark, logo, domain name and other unique identification marks of ETRI.
If you use the source form version or object form version of IoTware Project in whole or in part to develop a code or a derivative work, and you do not commercialize the result in any form, you will be covered under an open source license. IoTware Project is in accordance with Free Software Foundation (FSF)'s open source policy, and is allowed to use it in the appropriate scope and manner, and you must comply with the applicable open source license policy applied to IoTware Project. IoTware Project is, in principle, subject to GNU Lesser General Public License version 2.1 (LGPLv2.1). If you have acquired all or a part of the IoTware Project in any way and it is subject to a license other than the open source license described above, please contact the following address for the technical support and other inquiries before use, and check the usage information.
*/
#include "interface_oal.h"

iw_task_t create_task(iw_task_fn_t fn, const char *name, unsigned int stack_size, void *arg,
		      unsigned int priority, void *ext)
{
	return iw_create_task(fn, name, stack_size, arg, priority, ext);
}

void delete_task(iw_task_t task)
{
	iw_delete_task(task);
}

void exit_task(void)
{
	iw_exit_task();
}

void suspend_task(iw_task_t task)
{
	iw_suspend_task(task);
}

void resume_task(iw_task_t task)
{
	iw_resume_task(task);
}

iw_error_t get_task_handle(iw_task_t *handle)
{
	return iw_get_task_handle(handle);
}

iw_error_t get_task_free_stack(iw_task_t handle, unsigned int *free_stack)
{
	return iw_get_task_free_stack(handle, free_stack);
}

void *alloc_mem(unsigned int size)
{
	return iw_alloc(size);
}

void free_mem(void *ptr)
{
	iw_free(ptr);
}

iw_tick_t get_tick_count(void)
{
	return iw_get_tick_count();
}

unsigned int get_time_ms(void)
{
	return iw_get_time_ms();
}

void delay_us(unsigned int us)
{
	iw_delay_us(us);
}

void delay_ms(unsigned int ms)
{
	iw_delay_ms(ms);
}

void sleep(unsigned int ms)
{
	iw_sleep(ms);
}

bool get_rtc_time(DATE_TIME_T *dt)
{
	return iw_get_rtc_time(dt);
}

bool set_rtc_time(DATE_TIME_T *dt)
{
	return iw_set_rtc_time(dt);
}

iw_queue_t create_queue(unsigned int q_size, unsigned int item_size)
{
	return iw_create_queue(q_size, item_size);
}

void delete_queue(iw_queue_t q)
{
	iw_delete_queue(q);
}

iw_error_t send_queue(iw_queue_t q, void *ptr, unsigned int tick, iw_task_t receive_task)
{
	return iw_send_queue(q, ptr, tick, receive_task);
}

iw_error_t recv_queue(iw_queue_t q, void *ptr, int block)
{
	return iw_recv_queue(q, ptr, block);
}

iw_error_t create_sem(iw_sem_t *sem, unsigned int max_count, unsigned int init_count)
{
	return iw_create_sem(sem, max_count, init_count);
}

void delete_sem(iw_sem_t sem)
{
	iw_delete_sem(sem);
}

iw_error_t lock_sem(iw_sem_t sem, unsigned int tick)
{
	return iw_lock_sem(sem, tick);
}

iw_error_t unlock_sem(iw_sem_t sem, iw_task_t receive_task)
{
	return iw_unlock_sem(sem, receive_task);
}

iw_error_t create_mutex(iw_mutex_t *mutex)
{
	return iw_create_mutex(mutex);
}

void delete_mutex(iw_mutex_t mutex)
{
	iw_delete_mutex(mutex);
}

iw_error_t lock_mutex(iw_mutex_t mutex)
{
	return iw_lock_mutex(mutex);
}

iw_error_t unlock_mutex(iw_mutex_t mutex)
{
	return iw_unlock_mutex(mutex);
}

iw_error_t init_list(iw_list_t *list)
{
	return iw_init_list(list);
}

iw_error_t add_list_node(iw_list_t *list, iw_list_node_t *node)
{
	return iw_add_list_node(list, node);
}

iw_error_t remove_list_node(iw_list_t *list, iw_list_node_t *node)
{
	return iw_remove_list_node(list, node);
}

void get_cmd_str(char *buf)
{
	iw_get_cmd_str(buf);
}
