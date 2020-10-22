
/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
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
