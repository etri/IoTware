/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "iw_fw_microservices.h"


void base_task_type1(void *pvParameters)
{
	PSTProcessingParam pparam = (PSTProcessingParam)pvParameters;

	QueueSetHandle_t xQueueSet = xQueueCreateSet(setpollQUEUE_LENGTH);

	for (int i = 0; i < pparam->task_info->input_count; i++) {
		xQueueAddToSet(pparam->task_info->input_queues[i], xQueueSet);
	}

	QueueHandle_t xActivatedQueue = NULL;
	int loop_condition=TRUE;
	while(loop_condition == FALSE){
		SET_TASK_WAITING(pparam->task_info);
		xActivatedQueue = xQueueSelectFromSet(xQueueSet, setpollDONT_BLOCK);
		//_DPRINT(pparam->task_info, "xActivatedQueue == %s\n",xActivatedQueue?"not null":"null");

		if (xActivatedQueue == NULL) {
			sleep(200);
			continue;
		}

		pparam->queue_num = -1;
		for (int i = 0; i < pparam->task_info->input_count; i++) {
			if (pparam->task_info->input_queues[i] == xActivatedQueue) {
				pparam->queue_num = i;
				break;
			}
		}
		//_DPRINT(pparam->task_info, "%s queue_num == %d\n", pparam->task_info->name, pparam->queue_num);

		if (pparam->queue_num == -1)
			continue;

		iw_error_t recv = recv_queue(xActivatedQueue, pparam->input_data, FALSE);
		
		if (recv == IW_SUCCESS) {
			SET_TASK_RUNNING(pparam->task_info);
			STProcessingReturn beforeReturn;
			STProcessingReturn afterReturn;

			memset(&beforeReturn, 0, sizeof(STProcessingReturn));
			memset(&afterReturn, 0, sizeof(STProcessingReturn));

			if (pparam->task_info->before_fn)
				beforeReturn = pparam->task_info->before_fn(pparam);

			if (beforeReturn.user_data) {
				for (int i=0; i < pparam->task_info->output_count; i++) {
					PSTConnectLine cline = pparam->task_info->output_ports[i];
					if (cline) {
						send_queue(cline->dest_task->input_queues[cline->input_port_no], pparam->task_info->output_ptr, 0U, cline->dest_task->task_t);
					}
				}
				beforeReturn.user_data = NULL;
			}

			if (pparam->task_info->after_fn)
				afterReturn = pparam->task_info->after_fn(pparam);

			sleep(200);
		}

		sleep(200);
	}

	if (pparam->user_data)
		free(pparam->user_data);
	pparam->user_data = NULL;
	if (pparam->input_data)
		free(pparam->input_data);
	pparam->input_data = NULL;
	pparam->task_info->state = STOPPED;
	free(pparam);
}
