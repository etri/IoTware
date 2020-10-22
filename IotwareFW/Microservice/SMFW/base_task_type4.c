/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "iw_fw_microservices.h"


void base_task_type4(void *pvParameters)
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
		//_DPRINT(pparam->task_info, "queue_num == %d\n", pparam->queue_num);

		if (pparam->queue_num == -1)
			continue;

		iw_error_t recv = recv_queue(xActivatedQueue, pparam->input_data, FALSE);

		if (recv == IW_SUCCESS) {
			SET_TASK_RUNNING(pparam->task_info);
			STProcessingReturn beforeReturn;
			STProcessingReturn afterReturn;
			iw_tick_t xCurrWakeTime = 0;
			iw_tick_t xFirstWakeTime = get_tick_count();
			uint32_t count = 0;

			do {
				memset(&beforeReturn, 0, sizeof(STProcessingReturn));
				memset(&afterReturn, 0, sizeof(STProcessingReturn));
				iw_tick_t xMaxPeriod = IW_MS_TO_TICKS(pparam->total_sleep_ms * 1000);
				count++;
				xCurrWakeTime = get_tick_count();

				if (xMaxPeriod != 0 && (xCurrWakeTime - xFirstWakeTime) >= xMaxPeriod)
					break;
				if (pparam->total_sleep_count != 0 && count > pparam->total_sleep_count)
					break;
				if(pparam->task_info->state == STOPPED){
					_DPRINT(pparam->task_info, "%s Task Stopped\n", pparam->task_info->name);
					break;
				}

				if (pparam->task_info->before_fn)
					beforeReturn = pparam->task_info->before_fn(pparam);

				if (beforeReturn.user_data) {
					// Send data to all output port
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

				sleep(pparam->sleep_ms);
					
			} while (TRUE);
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
