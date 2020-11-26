/*
Copyright (C) <2020> <ETRI>

This License and Service Agreement (LSA) applies to all works and their derivative works based on source form version and object form version of IoTware Project. Currently, the LSA for IoTware Project has two policies, 'Open Source License' and 'Commercial License'. Therefore, all works including the source code and executable code of IoTware Project and derivative works based thereon are subject to either 'Open Source License' or 'Commercial License' depending on the user's needs and purpose. Details related to the selection of the applicable license are specified in this LSA. If you use any or all of IoTware Project in any form, you are deemed to have consented to this LSA. If you breach any of the terms and conditions set forth in this LSA, you are solely responsible for any losses or damages incurred by Electronics and Communications Research Institute (ETRI), and ETRI assume no responsibility for you or any third party.
If you use the source form version or object form version of IoTware Project in whole or in part to develop a code or a derivative work, and you want to commercialize the result in some form, you will be covered under a commercial license. And if you are subject to a commercial license, the contract for the use of IoTware Project is subject to TECHNOLOGY LICENSE AGREEMENT of ETRI. You acknowledge that ETRI has all legal rights, title and interest, including intellectual property rights in the IoTware Project (regardless of whether such intellectual property rights are registered or where such rights exist) and agree with no objection thereto. Except as provided in a subsidiary agreement, nothing in this LSA grants you the right to use IoTware Project or the name, service mark, logo, domain name and other unique identification marks of ETRI.
If you use the source form version or object form version of IoTware Project in whole or in part to develop a code or a derivative work, and you do not commercialize the result in any form, you will be covered under an open source license. IoTware Project is in accordance with Free Software Foundation (FSF)'s open source policy, and is allowed to use it in the appropriate scope and manner, and you must comply with the applicable open source license policy applied to IoTware Project. IoTware Project is, in principle, subject to GNU Lesser General Public License version 2.1 (LGPLv2.1). If you have acquired all or a part of the IoTware Project in any way and it is subject to a license other than the open source license described above, please contact the following address for the technical support and other inquiries before use, and check the usage information.
*/

/**
 * @file iw_fw_lwMsgCore.c
 * @brief 경량형 메시지 처리기 송수신 Task
 * @details 네트워크 인터페이스를 통해 송신한/수신할 경량형 메시지를 1차 처리하는 Task
 */
#include <stdlib.h>
#include <string.h>

#include "iw_fw_ms_settings.h"
#include "iw_fw_lwMsgUtils.h"
#include "iw_fw_microservices.h"

/*-----------------------------------------------------------*/
/* Queue Handle Definition */
extern iw_queue_t xNetworkRx;
extern iw_queue_t xNetworkTx;
extern iw_task_t xNetworkTxTask;

static iw_queue_t rXrespMsg = NULL;
/*-----------------------------------------------------------*/


/**
 * @brief 네트워크로부터 수신한 경량형 메시지 처리 Task
 * @details 네트워크로부터 수신한 경량형 메시지를 1차로 받아 처리
 * 수신한 byte array 형태의 메시지를 경량형 메시지 구조체 형태로 파싱
 * Request, Response를 구분하고 아직 기기 등록이 되지 않았다면 다른 처리를 하지 않음
 * @param pvParameters 사용하지 않음
 */
void iw_fw_lwMsgRx(void* pvParameters)
{
	char pcReceivedString[128] = { 0 };

	Boolean loop=TRUE;
	_DPRINTK("LWMSG RX TASK STARTED\n");

	for (PLWMessage lpLWMsg=NULL;loop==TRUE;lpLWMsg=NULL) {
		if (recv_queue(xNetworkRx, pcReceivedString, FALSE) == IW_SUCCESS) {
			lpLWMsg = PARSE_LWM_MESSAGE_STR(pcReceivedString);
			if (lpLWMsg == NULL) {
				_EPRINTK("PARSE_LWM_MESSAGE_STR(\"%s\") failed\n", pcReceivedString);
				continue;
			}

			PLWMObject cdri = _GET_LWM_OBJECT_FROM_DICT(&lpLWMsg->header, "dri");
			PLWMObject crsc = _GET_LWM_OBJECT_FROM_DICT(&lpLWMsg->header, "rsc");

			// recv response
			if (cdri && crsc) {
				_DPRINTK("recv response %s\n", pcReceivedString);
				send_queue(rXrespMsg, (void*)&lpLWMsg, 0, get_lwmsg_send_task());
			}
			else if (!get_device_is_registered()) {
				// 아직 등록 과정이 끝나지 않음
				// 받은 request 버림
				_DPRINTK("ignored recv request %s\n", pcReceivedString);
				DEL_LWM_MESSAGE(lpLWMsg);
			}
			// recv request
			else if (send_queue(get_lwmsg_receive_queue(), (void*)&lpLWMsg, 0, get_lwmsg_receive_task()) != IW_SUCCESS) {
				_EPRINTK("Could not send to the queue.\n");
				DEL_LWM_MESSAGE(lpLWMsg);
			}
		}

		sleep(10);
	}
}


/**
 * @brief 네트워크로 송신할 경량형 메시지 처리 Task
 * @details 네트워크로 송신할 경량형 메시지를 1차로 받아 처리
 * 송신할 경량형 메시지 구조체를 byte array 형태로 직렬화
 * 송신 시도 후 3초간 Response의 수신을 기다리며, 실패한다면 최대 2회 추가 송신
 * 단, 기기 등록 메시지의 경우 무제한 재시도
 * @param pvParameters 사용하지 않음
 */
 #define RETRANSMISSION_COUNT 50
 #define RESONSE_WAIT_TIME 10000
void iw_fw_lwMsgTx(void* pvParameters)
{
	const iw_tick_t xTicksToWait = IW_MS_TO_TICKS(100UL);

	_DPRINTK("REQUEST LWMSG TX TASK STARTED\n");

	if (rXrespMsg == NULL) {
		rXrespMsg = create_queue(mainQUEUE_LENGTH, sizeof(PLWMessage));
	}

	Boolean loop=TRUE;

	for (PLWMessage xReceivedStructure=NULL; loop==TRUE ;xReceivedStructure=NULL) {
		if (get_lwmsg_network_enabled() && recv_queue(get_lwmsg_send_queue(), &xReceivedStructure, TRUE) == IW_SUCCESS) {
			char *serialize = SERIALIZE_LWM_MESSAGE(xReceivedStructure);
			if (serialize == NULL) {
				_EPRINTK("SERIALIZE_LWM_MESSAGE(%p) failed\n", xReceivedStructure);
				DEL_LWM_MESSAGE(xReceivedStructure);
				continue;
			}
			// TODO remove
			_DPRINTK("[LWMSG_SEND] %s\n", serialize);

			// TODO: 256 --> MACRO
			if(strlen(serialize)>256)
				_EPRINTK("[LWMSG_SEND_SIZE_ERROR] Smaller then 256Bytes (%d)\n", strlen(serialize));

			if(xReceivedStructure!=NULL){
				PLWMObject send_cif = _GET_LWM_OBJECT_FROM_DICT(&xReceivedStructure->header, "if");
				PLWMObject send_cdri = _GET_LWM_OBJECT_FROM_DICT(&xReceivedStructure->header, "dri");
				PLWMObject send_crsc = _GET_LWM_OBJECT_FROM_DICT(&xReceivedStructure->header, "rsc");
			}

			// send request
			if (send_cif && send_cdri && !send_crsc) {
				int _send_if = GET_LWM_INTERFACE_ID(xReceivedStructure);
				const char *_send_dri = GET_LWM_STRING_VALUE(send_cdri);

				for (int i=0;i<RETRANSMISSION_COUNT || _send_if == 1;i++) {
					if (send_queue(xNetworkTx, serialize, xTicksToWait, xNetworkTxTask) != IW_SUCCESS) {
						_EPRINTK("Could not send to the queue. (%d)\n", i);
						sleep(10);
						continue;
					}

					PLWMessage lwmsg = NULL;
					BOOL is_break = FALSE;
					int some = 0;
					while (some < RESONSE_WAIT_TIME && !is_break) {
						sleep(100);
						some += 100;
						if (recv_queue(rXrespMsg, &lwmsg, FALSE) == IW_SUCCESS) {
							if(lwmsg!=NULL){
								PLWMObject cdri = _GET_LWM_OBJECT_FROM_DICT(&lwmsg->header, "dri");
								PLWMObject crsc = _GET_LWM_OBJECT_FROM_DICT(&lwmsg->header, "rsc");
								const char *_dri = GET_LWM_STRING_VALUE(cdri);
								const char *_rsc = GET_LWM_STRING_VALUE(crsc);
								if (_send_if == 1 && strcmp(_send_dri, _dri) == 0 && strcmp(_rsc, "200") == 0) {
									PLWMObject cdi = _GET_LWM_OBJECT_FROM_DICT(&lwmsg->body, "di");
									const char *_di = GET_LWM_STRING_VALUE(cdi);
									SET_LWM_DI(_di);
									_DPRINTK("catch if(1) response (dri: %s, rsc: %s, di: %s)\n", _dri, _rsc, _di);
									PLWMessage lwm_msg = make_device_information_lwmsg(2, GET_LWM_TX_DRI(), _di);
									if (lwm_msg)
										REQUEST_LWMSG(lwm_msg);
									is_break = TRUE;
								}
								else if (_send_if == 1) {
									_DPRINTK("resend interface id 1 msg (send_dri: %s, dri: %s, rsc: %s)\n", _send_dri, _dri, _rsc);
								}
								else if (_send_if == 2 && strcmp(_send_dri, _dri) == 0 && strcmp(_rsc, "200") == 0) {
									_DPRINTK("catch if(2) response (dri: %s, rsc: %s)\n", _dri, _rsc);
									set_device_is_registered(TRUE);
									is_break = TRUE;
								}
								else if (_send_if == 14 && strcmp(_send_dri, _dri) == 0 && strcmp(_rsc, "200") == 0) {
									_DPRINTK("catch if(14) response (dri: %s, rsc: %s)\n", _dri, _rsc);
									is_break = TRUE;
								}
								else if (strcmp(_rsc, "200") != 0) {
									_EPRINTK("error returned (if: %d, rsc: %s)\n", _send_if, _rsc);
								}
								else {
									_EPRINTK("error unknown response\n");
									is_break = TRUE;
								}

								DEL_LWM_MESSAGE(lwmsg);
							}
						}
					}

					if (is_break) {
						break;
					}
					else if (some >= RESONSE_WAIT_TIME) {
						_EPRINTK("send %d request lwmsg timeout (%d)\n", _send_if, i);
					}
				}
			}
			// send response
			else if (!send_cif && send_cdri && send_crsc) {
				if (send_queue(xNetworkTx, serialize, xTicksToWait, xNetworkTxTask) != IW_SUCCESS) {
					_EPRINTK("Could not send to the queue.\n");
				}
			}

			free(serialize);
			DEL_LWM_MESSAGE(xReceivedStructure);
		}
		else if (get_lwmsg_network_enabled()) {
			_EPRINTK("iw_fw_lwMsgTx.recv_queue() failed\n");
		}
		sleep(10);
	}
}
