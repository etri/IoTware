/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/
#ifndef INTERFACE_OAL_H
#define INTERFACE_OAL_H

#include "iw_common.h"
#include "iw_oal.h"
#include "iw_bsp.h"

/* task function */
iw_task_t create_task(iw_task_fn_t fn,const char *name, unsigned int stack_size, void *arg,   
                         unsigned int priority, void *ext);
void delete_task(iw_task_t task);
void exit_task(void);
void suspend_task(iw_task_t task);
void resume_task(iw_task_t task);
iw_error_t get_task_handle(iw_task_t *handle);
iw_error_t get_task_free_stack(iw_task_t handle, unsigned int *free_stack); 

/* memory function */
void *alloc_mem(unsigned int size);
void free_mem(void *ptr);

/* timer function */
iw_tick_t get_tick_count(void);
unsigned int get_time_ms(void);
void delay_us(unsigned int us);
void delay_ms(unsigned int ms);
void sleep(unsigned int ms);
bool get_rtc_time(DATE_TIME_T  *dt);
bool set_rtc_time(DATE_TIME_T  *dt);

/* message queue function */
iw_queue_t create_queue(unsigned int q_size, unsigned int item_size);
void delete_queue(iw_queue_t q);
iw_error_t send_queue(iw_queue_t q, void *ptr, unsigned int tick, iw_task_t receive_task);
iw_error_t recv_queue(iw_queue_t q, void *ptr, int block);

/* semaphore function */
iw_error_t create_sem(iw_sem_t *sem, unsigned int max_count, unsigned int init_count);
void delete_sem(iw_sem_t sem);
iw_error_t unlock_sem(iw_sem_t sem, iw_task_t receive_task);
iw_error_t lock_sem(iw_sem_t sem, unsigned int tick);

/* mutex function */
iw_error_t create_mutex(iw_mutex_t *mutex);
void delete_mutex(iw_mutex_t mutex);
iw_error_t lock_mutex(iw_mutex_t mutex);
iw_error_t unlock_mutex(iw_mutex_t mutex);

/* list function */
iw_error_t init_list(iw_list_t *list);
iw_error_t add_list_node(iw_list_t *list, iw_list_node_t *node);
iw_error_t remove_list_node(iw_list_t *list, iw_list_node_t *node);


void get_cmd_str(char *buf);

#endif /* INTERFACE_OAL_H */

