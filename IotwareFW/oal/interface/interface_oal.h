/*
Copyright (C) <2020> <ETRI>

This License and Service Agreement (LSA) applies to all works and their derivative works based on source form version and object form version of IoTware Project. Currently, the LSA for IoTware Project has two policies, 'Open Source License' and 'Commercial License'. Therefore, all works including the source code and executable code of IoTware Project and derivative works based thereon are subject to either 'Open Source License' or 'Commercial License' depending on the user's needs and purpose. Details related to the selection of the applicable license are specified in this LSA. If you use any or all of IoTware Project in any form, you are deemed to have consented to this LSA. If you breach any of the terms and conditions set forth in this LSA, you are solely responsible for any losses or damages incurred by Electronics and Communications Research Institute (ETRI), and ETRI assume no responsibility for you or any third party.
If you use the source form version or object form version of IoTware Project in whole or in part to develop a code or a derivative work, and you want to commercialize the result in some form, you will be covered under a commercial license. And if you are subject to a commercial license, the contract for the use of IoTware Project is subject to TECHNOLOGY LICENSE AGREEMENT of ETRI. You acknowledge that ETRI has all legal rights, title and interest, including intellectual property rights in the IoTware Project (regardless of whether such intellectual property rights are registered or where such rights exist) and agree with no objection thereto. Except as provided in a subsidiary agreement, nothing in this LSA grants you the right to use IoTware Project or the name, service mark, logo, domain name and other unique identification marks of ETRI.
If you use the source form version or object form version of IoTware Project in whole or in part to develop a code or a derivative work, and you do not commercialize the result in any form, you will be covered under an open source license. IoTware Project is in accordance with Free Software Foundation (FSF)'s open source policy, and is allowed to use it in the appropriate scope and manner, and you must comply with the applicable open source license policy applied to IoTware Project. IoTware Project is, in principle, subject to GNU Lesser General Public License version 2.1 (LGPLv2.1). If you have acquired all or a part of the IoTware Project in any way and it is subject to a license other than the open source license described above, please contact the following address for the technical support and other inquiries before use, and check the usage information.
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

