/*
Copyright (C) <2020> <ETRI>

This License and Service Agreement (LSA) applies to all works and their derivative works based on source form version and object form version of IoTware Project. Currently, the LSA for IoTware Project has two policies, 'Open Source License' and 'Commercial License'. Therefore, all works including the source code and executable code of IoTware Project and derivative works based thereon are subject to either 'Open Source License' or 'Commercial License' depending on the user's needs and purpose. Details related to the selection of the applicable license are specified in this LSA. If you use any or all of IoTware Project in any form, you are deemed to have consented to this LSA. If you breach any of the terms and conditions set forth in this LSA, you are solely responsible for any losses or damages incurred by Electronics and Communications Research Institute (ETRI), and ETRI assume no responsibility for you or any third party.
If you use the source form version or object form version of IoTware Project in whole or in part to develop a code or a derivative work, and you want to commercialize the result in some form, you will be covered under a commercial license. And if you are subject to a commercial license, the contract for the use of IoTware Project is subject to TECHNOLOGY LICENSE AGREEMENT of ETRI. You acknowledge that ETRI has all legal rights, title and interest, including intellectual property rights in the IoTware Project (regardless of whether such intellectual property rights are registered or where such rights exist) and agree with no objection thereto. Except as provided in a subsidiary agreement, nothing in this LSA grants you the right to use IoTware Project or the name, service mark, logo, domain name and other unique identification marks of ETRI.
If you use the source form version or object form version of IoTware Project in whole or in part to develop a code or a derivative work, and you do not commercialize the result in any form, you will be covered under an open source license. IoTware Project is in accordance with Free Software Foundation (FSF)'s open source policy, and is allowed to use it in the appropriate scope and manner, and you must comply with the applicable open source license policy applied to IoTware Project. IoTware Project is, in principle, subject to GNU Lesser General Public License version 2.1 (LGPLv2.1). If you have acquired all or a part of the IoTware Project in any way and it is subject to a license other than the open source license described above, please contact the following address for the technical support and other inquiries before use, and check the usage information.
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
