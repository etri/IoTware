
/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#ifndef	IW_OAL_H
#define	IW_OAL_H

/**
 * \addtogroup	OS Abstraction Layer
 * @brief	OS abstraction APIs for FreeRTOS, NanoQplus, RIOT, ApacheMynewt, Contiki
 * @{
 */

#include "kernel_oal.h"


/**
 * @brief	Priorities of task
 */
enum iw_oal_task_priority_e
{
	IW_TSK_PRIO_IDLE	= OAL_TSK_PRIO_IDLE_,	/*!< lowest priority at which the idle task will execute */
	IW_TSK_PRIO_MAX		= OAL_TSK_PRIO_MAX_,	/*!< highest priority at which a task will execute */
};

typedef struct
{
    volatile uint32_t put_pos;
    volatile uint32_t get_pos;
    uint32_t block_size;
    uint32_t block_count;
    uint8_t *block_pdata;
} iw_fifo_t;

typedef struct iw_list_node {
    struct iw_list_node *next;     /**< pointer to next list entry */
    struct iw_list_node *previous;     /**< pointer to next list entry */
    void *pData;
} iw_list_node_t;

typedef struct iw_list {
    iw_list_node_t *head;
    unsigned char nodeNum;
} iw_list_t;


/**
 * @brief	max delay tick count
 */
#define IW_INFINITE			OAL_INFINITE_

/**
 * @brief	minimal size of stack for task
 */
#define	IW_MINIMAL_STACK_SIZE	OAL_MINIMAL_STACK_SIZE_

/**
 * @brief	OS tick period in mili-second
 */
#define	IW_TICK_PERIOD_MS		OAL_TICK_PERIOD_MS_

/**
 * @brief	OK, normal, success and no problem
 */
#define	oal_true				oal_true_

/**
 * @brief	error, abnormal, failure whatever a root cause is
 */
#define	oal_false				oal_false_

/**
 * @brief	Task function that created task will run
 */
typedef void (*iw_task_fn_t)(void *);

/**
 * @brief	Handle to the task
 */
//typedef void *oal_task_t;

/**
 * @brief	Handle to Queue
 */
typedef void *iw_queue_t;

/**
 * @brief	Handle to semaphore
 */
typedef void *iw_sem_t;

/**
 * @brief	Handle to mutex
 */
typedef void *iw_mutex_t;

/**
 * @brief	type definition for OS tick count
 */
typedef uint32_t iw_tick_t;

/*--------- TASK -----------*/
#if (IW_FREERTOS == 1)
//typedef void *iw_task_t;
#elif (IW_NANOQPLUS == 1)
//typedef uint32_t iw_task_t;
#elif (IW_RIOT == 1)
//typedef uint32_t iw_task_t;
//typedef int16_t iw_task_t;
#else
#endif

/**
 * @brief	init the rtos
 * @return	NONE
 */
void iw_init_os(void);

char *iw_os_name(void);

/**
 * @brief	start the rtos scheduler
 * @return	NONE
 */
void iw_start_scheduler(void);

/**
 * @brief	request to yield
 */
void iw_yield (void);

/**
 * @brief	request to yield in ISR
 */
void iw_yield_isr (int arg);

/**
 * @brief	enter critical section
 */
void iw_enter_critical(void);

/**
 * @brief	exit critical section
 */
void iw_exit_critical(void);

/**
 * @brief	create a new task
 * @param   [in]    fn          pointer to the task entry function
  * @param	[in]	name		a descriptive name for the task
 * @param   [in]    stack_size  the number of words to allocate for use as task's stack
 * @param	[in]	arg			a value that will be passed into the created task as the task's parameter
 * @param	[in]	priority	the priority at which the created task will execute
 * @param	[in]	ext			pointer to any data for extension. it's not used for FreeRTOS.
 * @return	handle to the created task if the task was created successfully, or NULL in failure.
 *
 */
iw_task_t iw_create_task(iw_task_fn_t fn,const char *name, unsigned int stack_size, void *arg,   
                         unsigned int priority, void *ext);

/**
 * @brief	delete task
 * @param	[in]	task	handle to the task being deleted
 * @return	void
 */
void iw_delete_task(iw_task_t task);

/**
 * @brief   exit task
 * @param   [void]
 * @return  void
 */
void iw_exit_task(void);


/**
 * @brief	suspend a task
 * @param	[in]	task		handle to the task being suspended
 * @return	void
 */
void iw_suspend_task(iw_task_t task);

/**
 * @brief	resume a task
 * @param	[in]	task		handle to the task being resumed
 * @return	void
 */
void iw_resume_task(iw_task_t task);

/**
 * @brief   delay a task for given time in mili-second
 * @param   [in]    ms  time in mili-second that calling task should block
 * @return  void
 */
void iw_sleep(unsigned int ms);


/**
 * @brief	get current task's handle
 * @param	[out]	handle		task's handle
 * @return	IW_SUCCESS or IW_FAIL
 */
iw_error_t iw_get_task_handle(iw_task_t *handle);

/**
 * @brief	get free stack size
 * @param	[in]	handle			handle to the task
 * @param	[out]	free_stack		free stack size
 * @return	IW_SUCCESS or IW_FAIL
 */
iw_error_t	iw_get_task_free_stack(iw_task_t handle, unsigned int *free_stack);

#define IW_SIG_ALL		(0xFFFFFFFFul)

/**
 * @brief	send events to task
 * @param	[in]	task		hanle to the task being notified by event
 * @param	[in]	bits		notification value
 * @param	[in]	is_is		flag to indicate that event is sent from ISR
 * @return	oal_true in success\n
 * 			oal_fail in failure
 */
iw_error_t  iw_signal_task(iw_task_t task, uint32_t bits, int32_t is_isr);

/**
 * @brief	wait to event
 * @param	[in]	bits_to_wait	notification value
 * @param	[in]	tick		time in ticks to wait for event
 * @return	event value
 */
uint32_t  iw_wait_signal(uint32_t bits_to_wait, uint32_t tick); 

/*------------ Mutex ------------*/

/**
 * @brief	create a mutex
 * @param	[out]	mutex		pointer to return a handle to new mutex
 * @return	IW_SUCCESS or IW_FAIL
 */
iw_error_t iw_create_mutex(iw_mutex_t *mutex);

/**
 * @brief	delete a mutex
 * @param	[in]	mutex		handle to the mutex being deleted
 * @return	NONE
 */
void iw_delete_mutex(iw_mutex_t mutex);

/**
 * @brief	lock mutex
 * @param	[in]	mutex		handle to the mutex being locked
 * @return	IW_SUCCESS or IW_FAIL
 */
iw_error_t iw_lock_mutex(iw_mutex_t mutex);

/**
 * @brief	lock mutex
 * @param	[in]	mutex		handle to the mutex being locked
 * @param	[in]	ms_to_wait	time in mili-second to wait for lock
 * @return	oal_true in success\n
 * 			oal_false in failure
 */
iw_error_t iw_lock_mutex_ms(iw_mutex_t mutex, unsigned int ms_to_wait);

/**
 * @brief	unlock mutex
 * @param	[in]	mutex		handle to the mutex being unlocked
 * @return	IW_SUCCESS or IW_FAIL
 */
iw_error_t iw_unlock_mutex(iw_mutex_t mutex);


/*--------- Semaphore ----------*/

/**
 * @brief	create a semaphore
 * @param	[out]	sem		pointer to return a handle to new semaphore
 * @param	[in]	max_count	maxim count value that can be reached
 * @param	[in]	init_count	count value assigned to the semaphore when it is created
 * @return	IW_SUCCESS or IW_FAIL
 */
iw_error_t iw_create_sem(iw_sem_t *sem, unsigned int max_count, unsigned int init_count);

/**
 * @brief	delete a semaphore
 * @param	[in]	sem		handle of semaphore being deleted
 * @return	NONE
 */
void iw_delete_sem(iw_sem_t sem);

/**
 * @brief	obtain a semaphore
 * @param	[in]	sem		handle of semaphore being locked
 * @param	[in]	tick	time in ticks to wait for the semaphore to become available
 * @return	IW_SUCCESS or IW_FAIL
 */
iw_error_t iw_lock_sem(iw_sem_t sem, unsigned int tick);

/**
 * @brief	release a semaphore
 * @param	[in]	sem		handle of semaphore being unlocked
 * @return	IW_SUCCESS or IW_FAIL
 */
iw_error_t iw_unlock_sem(iw_sem_t sem, iw_task_t receive_task);

/**
 * @brief	release a semaphore. it can be used from an ISR
 * @param	[in]	sem		handle of semaphore being unlocked
 * @param	[out]	pWoken	pointer to return information about woken task
 * @return	oal_true in success\n
 * 			oal_false in failure
 */
iw_error_t	iw_unlock_sem_isr(iw_sem_t sem, int32_t *pWoken);

/*--------- Queue ------------*/
/**
 * @brief	create a new queue
 * @param	[in]	q_size		maximum number of items the queue can hold
 * @param	[in]	item_size	the size in byte required to hold each item in the queue
 * @return	handle to the created queue in success\n
 * 			NULL in failure
 */
iw_queue_t iw_create_queue(unsigned int q_size, unsigned int item_size);

/**
 * @brief	delete a queue
 * @param	[in]	q		handle to the queue being deleted
 * @return	none
 */
void iw_delete_queue(iw_queue_t q);

/**
 * @brief	post an item on a queue
 * @param	[in]	q		handle to the queue on which an item is to be posted
 * @param	[in]	ptr		pointer to the item that is to be posted
 * @param	[in]	tick	maximum time in tick that the calling task should block waiting for space to become available on the queue
 * @param   [in]    receive_task task's handle to receive a item
 * @return	IW_SUCCESS or IW_FAIL
 */
iw_error_t iw_send_queue(iw_queue_t q, void *ptr, unsigned int tick, iw_task_t receive_task);

/**
 * @brief	post an item on a queue. It's save to use this function from ISR.
 * @param	[in]	q		handle to the queue on which an item is to be posted
 * @param	[in]	ptr		a pointer to the item that is to be posted
 * @param	[out]	pWoken	pointer to return information about woken task
 * @return 	oal_true in success\n
 * 			oal_false in failure
 */
iw_error_t iw_send_queue_isr(iw_queue_t q, void *ptr, int32_t *pWoken);

/**
 * @brief	receive an item from queue
 * @param	[in]	q		handle to the queue from which an item is to be received
 * @param	[in]	ptr	    pointer to the buffer into which the received item will be copied
 * @param	[in]	block	1: wait for a message to be received. 
 *                          0: return immediately even if there is no message. 
 * @return	IW_SUCCESS or IW_FAIL
 */
iw_error_t iw_recv_queue(iw_queue_t q, void *ptr, int block);

/**
 * @brief	receive an item from queue without removing the item from the queue
 * @param	[in]	q		handle to the queue from which an item is to be received
 * @param	[in]	potr	pointer to the buffer into which the received item will be copied
 * @param	[in]	tick	maximum time in tick that the calling task should block waiting for an item to receive
 * @return	oal_true in success\n
 * 			oal_false in failure
 */
iw_error_t iw_peek_queue(iw_queue_t q, void *ptr, uint32_t tick);

/**
 * @brief	reset a queue to its original empty state
 * @param	[in]	q		handle to the queue being reset
 * @return	oal_true
 */
iw_error_t iw_reset_queue(iw_queue_t q);

/*--------- Memory -----------*/

/**
 * @brief	allocate memory
 * @param	[in]	size	size of memory in byte to be allocated
 * @return	pointer to allocated memory or NULL in failure
 */
void *iw_alloc(unsigned int size);

/**
 * @brief	free allocated memory
 * @param	[in]	ptr		pointer to allocated memory to be freed
 * @return	NONE
 */
void iw_free(void *ptr);

/**
 * @brief	get free heap size
 * @return	size of free heap
 */
//size_t iw_get_free_heap_size(void);

/**
 * @brief	allocate memory
 * 			the memory is set to zero
 * @param	[in]	size	size of memory in byte to be allocated
 * @return	pointer to allocated memory or NULL in failure
 */
//void *iw_zalloc(unsigned int size);

/**
 * @brief	get tick count since the scheduler started
 * @return	count of ticks
 */
iw_tick_t iw_get_tick_count(void);

/**
 * @brief	get time since the scheduler started
 * @return	time in mili-second since the scheduler started
 */
unsigned int iw_get_time_ms(void);

/**
 * @brief   initialize the list
 * @param   [in]    list
 * @return  IW_SUCCESS or IW_FAIL     
 */
iw_error_t iw_init_list(iw_list_t *list);

/**
 * @brief   add the new node
 * @param   [in]    list     list
 * @param   [in]    node     new node to add to list
 * @return  IW_SUCCESS or IW_FAIL
 */
iw_error_t iw_add_list_node(iw_list_t *list, iw_list_node_t *node);

/**
 * @brief   remove the node
 * @param   [in]    list     list
 * @param   [in]    node     node to remove from list
 * @return  IW_SUCCESS or IW_FAIL
 */
iw_error_t iw_remove_list_node(iw_list_t *list, iw_list_node_t *node);

/**
 * @brief   init fifo
 * @param   [in]    pfifo    fifo pointer
 * @param   [in]    pdata    fifo's data pointer
 * @param   [in]    size     a block size
 * @param   [in]    count    block count
  * @return  IW_SUCCESS or IW_FAIL
 */
iw_error_t iw_init_fifo ( iw_fifo_t *pfifo, void *pdata, unsigned int size, unsigned int count );

/**
 * @brief   put the data to fifo
 * @param   [in]    pfifo     fifo pointer  
 * @param   [in]    pdata     data's pointer to be put
 * @return  IW_SUCCESS or IW_FAIL
 */
iw_error_t iw_put_fifo  ( iw_fifo_t *pfifo, void *pdata );

/**
 * @brief   copy fifo's data
 * @param   [in]    pfifo     fifo pointer
 * @param   [out]   pdata     data's pointer to copy
 * @return  IW_SUCCESS or IW_FAIL
 */
iw_error_t iw_get_fifo  ( iw_fifo_t *pfifo, void *pdata );

/**
 * @brief   get number of items in fifo
 * @param   [in]    pfifo     fifo pointer
 * @return  number of items in fifo
 */
unsigned int iw_count_fifo( iw_fifo_t *pfifo );

/**
 * @brief   flush the fifo
 * @param   [in]    pfifo     fifo pointer to be flushed
 * @return  IW_SUCESS or IW_FAIL  
*/
iw_error_t iw_flush_fifo( iw_fifo_t *pfifo );


#define IW_SET_LIST_NODE_DATA( node, pData )      ( ( node )->pData = ( void * ) ( pData ) )
#define IW_GET_LIST_NODE_DATA( node )   ( ( node )->pData )
#define IW_GET_HEAD( list )    (  ( list )->head )
#define IW_GET_NEXT( node )  ( ( node )->next )
#define IW_GET_PREVIOUS( node )  ( ( node )->previous )
#define IW_GET_LIST_NODE_NUM( list )  ( ( list )->nodeNum )

void iw_get_cmd_str(char *buf);

#include "kernel_func.h"

/**
 *	@}
 */


#endif // IW_OAL_H
