/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "iw_fw_microservices.h"


void base_task_type2(void *pvParameters)
{
	PSTProcessingParam pparam = (PSTProcessingParam)pvParameters;

	//_DPRINT(pparam->task_info, "xActivatedQueue == %s\n",xActivatedQueue?"not null":"null");

	int loop_condition=TRUE;
	while(loop_condition == FALSE){
		SET_TASK_WAITING(pparam->task_info);
		iw_error_t recv = recv_queue(GET_INPUT_PORT_QUEUE(pparam->task_info, 0), pparam->input_data, TRUE);
		
		if (recv == IW_SUCCESS) {
			SET_TASK_RUNNING(pparam->task_info);
			STProcessingReturn beforeReturn;
			STProcessingReturn afterReturn;

			pparam->queue_num = 0;

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
