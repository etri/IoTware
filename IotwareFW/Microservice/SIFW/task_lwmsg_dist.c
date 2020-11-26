/*
Copyright (C) <2020> <ETRI>

This License and Service Agreement (LSA) applies to all works and their derivative works based on source form version and object form version of IoTware Project. Currently, the LSA for IoTware Project has two policies, 'Open Source License' and 'Commercial License'. Therefore, all works including the source code and executable code of IoTware Project and derivative works based thereon are subject to either 'Open Source License' or 'Commercial License' depending on the user's needs and purpose. Details related to the selection of the applicable license are specified in this LSA. If you use any or all of IoTware Project in any form, you are deemed to have consented to this LSA. If you breach any of the terms and conditions set forth in this LSA, you are solely responsible for any losses or damages incurred by Electronics and Communications Research Institute (ETRI), and ETRI assume no responsibility for you or any third party.
If you use the source form version or object form version of IoTware Project in whole or in part to develop a code or a derivative work, and you want to commercialize the result in some form, you will be covered under a commercial license. And if you are subject to a commercial license, the contract for the use of IoTware Project is subject to TECHNOLOGY LICENSE AGREEMENT of ETRI. You acknowledge that ETRI has all legal rights, title and interest, including intellectual property rights in the IoTware Project (regardless of whether such intellectual property rights are registered or where such rights exist) and agree with no objection thereto. Except as provided in a subsidiary agreement, nothing in this LSA grants you the right to use IoTware Project or the name, service mark, logo, domain name and other unique identification marks of ETRI.
If you use the source form version or object form version of IoTware Project in whole or in part to develop a code or a derivative work, and you do not commercialize the result in any form, you will be covered under an open source license. IoTware Project is in accordance with Free Software Foundation (FSF)'s open source policy, and is allowed to use it in the appropriate scope and manner, and you must comply with the applicable open source license policy applied to IoTware Project. IoTware Project is, in principle, subject to GNU Lesser General Public License version 2.1 (LGPLv2.1). If you have acquired all or a part of the IoTware Project in any way and it is subject to a license other than the open source license described above, please contact the following address for the technical support and other inquiries before use, and check the usage information.
*/

#include <stdio.h>

#include "iw_fw_microservices.h"
#include "iw_fw_lwMsgUtils.h"
#include "ms_generator.h"

#define NEW_LWM_OBJECT_FROM_PARAM(__param) new_lwm_object_from_type_value(__param->type, __param->value)


extern iw_queue_t xNetworkRx;
extern iw_queue_t xNetworkTx;
extern iw_task_t xNetworkRxTask;
extern iw_task_t xNetworkTxTask;

/**
 * @brief Task 생성/실행/정지 관련 작업 수행
 * @details Task 생성/실행/정지에 대한 경량형 메시지를 처리하고 실행하는 함수
 * @param _if 처리할 Interface ID 값
 * @param _dri 처리할 경량형 메시지의 dri 값
 * @param _msg 처리할 경량형 메시지
 * @return response code 처리 결과 응답 값
 * 성공 시 200, 실패 시 400 계열 응답 코드
 */
static int _lwmsg_operation(const int _if, const char *const _dri, PLWMessage _msg)
{
	int rsc = eLWM_RSC_BAD_REQ;
	BOOL(*operation)(const PSTMicroServiceInfo) = NULL;

	if (_if == eLWM_IF_MS_CREATION) {
		_DPRINTK("MICROSERVICE CREATION RECEIVED\n");
		operation = create_micro_service;
	}
	else if (_if == eLWM_IF_MS_RUN) {
		_DPRINTK("MICROSERVICE RUN RECEIVED\n");
		operation = run_micro_service;
	}
	else if (_if == eLWM_IF_MS_STOP) {
		_DPRINTK("MICROSERVICE STOP RECEIVED\n");
		operation = stop_micro_service;
	}
	else if (_if == eLWM_IF_MS_DELETE) {
		_DPRINTK("MICROSERVICE DELETED RECEIVED\n");
		operation = del_micro_service;
	}

	if (_msg) {
		PLWMObject cmis = _GET_LWM_OBJECT_FROM_DICT(&_msg->body, "mis");
		if (cmis && IS_LWM_ARRAY(cmis)) {
			PLWMObject _msid = NULL;
			// count 만큼 리턴 메시지 배열 만듦
			LWM_FOREACH(cmis->value, pxItem) {
				_msid = IW_GET_LIST_NODE_DATA(pxItem);
				const PSTMicroServiceInfo pmsi = get_micro_service_info_by_id(GET_LWM_STRING_VALUE(_msid));
				if (pmsi) {
					if (operation(pmsi))
						rsc = 200;
					// rtn return
				}
				else {
					rsc = 404;
				}
			}
		}
	}

	return rsc;
}

/**
 * @brief 파라미터 정보 구조체에 값 저장
 * @details 파라미터 정보 구조체에 char * 문자열 리터럴을 올바른 데이터 타입의 값으로 컨버팅 해 저장
 * @param __param 파라미터 정보 구조체 포인터
 * @param __str_value 저장할 값이 문자열 형태로 저장된 문자열 리터럴
 */
static void _parameter_set_by_string(const PSTParameter __param, const char *const __str_value)
{
	long decimal_value = 0;
	int8_t int8_value = 0;
	uint8_t uint8_value = 0;
	int16_t int16_value = 0;
	uint16_t uint16_value = 0;
	int32_t int32_value = 0;
	uint32_t uint32_value = 0;
	float float_value = 0;
	double double_value = 0;

	if (IS_DECIMAL(__param->type)) {
		decimal_value = atol(__str_value);
	}
	else if (__param->type == eP_FLOAT) {
		double_value = atof(__str_value);
		float_value = (float)double_value;
	}
	// 복사할 사이즈가 다르기때문에 이런식으로 작성
	switch (__param->type) {
		case eP_INT8:
			int8_value = (int8_t)decimal_value;
			memcpy(__param->value, (const AUTO *)&int8_value, __param->value_size);
			break;
		case eP_UINT8:
			uint8_value = (uint8_t)decimal_value;
			memcpy(__param->value, (const AUTO *)&uint8_value, __param->value_size);
			break;
		case eP_INT16:
			int16_value = (int16_t)decimal_value;
			memcpy(__param->value, (const AUTO *)&int16_value, __param->value_size);
			break;
		case eP_UINT16:
			uint16_value = (uint16_t)decimal_value;
			memcpy(__param->value, (const AUTO *)&uint16_value, __param->value_size);
			break;
		case eP_INT32:
			int32_value = (int32_t)decimal_value;
			memcpy(__param->value, (const AUTO *)&int32_value, __param->value_size);
			break;
		case eP_UINT32:
			uint32_value = (uint32_t)decimal_value;
			memcpy(__param->value, (const AUTO *)&uint32_value, __param->value_size);
			break;
		case eP_FLOAT:
			memcpy(__param->value, (const AUTO *)&float_value, __param->value_size);
			break;
		case eP_DOUBLE:
			memcpy(__param->value, (const AUTO *)&double_value, __param->value_size);
			break;
		case eP_STRING:
			memcpy(__param->value, (const AUTO *)&__str_value, __param->value_size);
			break;
		default:
			break;
	}
}
/**
 * @brief Parameter Set 관련 작업 수행
 * @details Parameter Set에 대한 경량형 메시지를 처리하고 실행하는 함수
 * @param _if 처리할 Interface ID 값
 * @param _dri 처리할 경량형 메시지의 dri 값
 * @param _msg 처리할 경량형 메시지
 * @return response code 처리 결과 응답 값
 * 성공 시 200, 실패 시 400 계열 응답 코드
 */
static int _lwmsg_param_set(const int _if, const char *const _dri, PLWMessage _msg)
{
	int rsc = eLWM_RSC_BAD_REQ;

	_DPRINTK("MICROSERVICE TASK PARAM SET RECEIVED\n");

	if (_msg) {
		PLWMObject cmis = _GET_LWM_OBJECT_FROM_DICT(&_msg->body, "mis");
		if (cmis && IS_LWM_ARRAY(cmis) && LWM_ARRAY_LENGTH(cmis) == 1) {
			PLWMObject _msid = NULL;
			LWM_FOREACH(cmis->value, pxItem) {
				_msid = IW_GET_LIST_NODE_DATA(pxItem);
				const PSTMicroServiceInfo pmsi = get_micro_service_info_by_id(GET_LWM_STRING_VALUE(_msid));
				if (pmsi) {
					BOOL atleastFound = FALSE;
	
					PLWMObject _tis = _GET_LWM_OBJECT_FROM_DICT(&_msg->body, "tis");
					if (_tis && IS_LWM_ARRAY(_tis)) {
						PLWMObject _ti = NULL;
						LWM_FOREACH(_tis->value, pxItem) {
							_ti = IW_GET_LIST_NODE_DATA(pxItem);
							PLWMObject _fp = _GET_LWM_OBJECT_FROM_DICT(&_msg->body, "fp");
							if (!_fp) {
								printf("1: fp get error\n");
								rsc = eLWM_RSC_BAD_REQ;
								return rsc;
							}
							PLWMKVObject __fp = NULL;

							size_t tidx = atoi(GET_LWM_STRING_VALUE(_ti));
							PSTTaskInfo ti = get_task_info_by_idx(pmsi, tidx);
							LWM_FOREACH(_fp->value, pxKVItem) {
								__fp = IW_GET_LIST_NODE_DATA(pxKVItem);
								const PSTParameter param = get_parameter_from_param_list(&ti->flexible_params, NULL, __fp->key);
								if (param) {
									atleastFound = TRUE;
									_parameter_set_by_string(param, GET_LWM_STRING_VALUE(__fp->value));
								}
								else {
									_EPRINTK("ms_id: %s, %s param: %s not found\n", pmsi->ms_id, ti->name, __fp->key ? __fp->key : "(null)");
								}
							}
							
							if (!atleastFound) {
								printf("2: MSID or Parameter error\n");
								rsc = eLWM_RSC_NOTFOUND;
								// 해당 ID를 가진 microservice 혹은 parameter를 찾을 수 없음
							}
							else {
								rsc = eLWM_RSC_SUCCESS;
							}
						}
					}
					else{
						printf("5: TIS NOT FOUND\n");
						rsc = eLWM_RSC_BAD_REQ;
					}
				}
				else{
					printf("4: pmsi error\n");
				}
			}
		}
	}else{
		printf("3: Message error\n");
	}

	return rsc;
}

/**
 * @brief Parameter Read 관련 작업 수행
 * @details Parameter Read에 대한 경량형 메시지를 처리하고 실행하는 함수
 * @param _if 처리할 Interface ID 값
 * @param _dri 처리할 경량형 메시지의 dri 값
 * @param _msg 처리할 경량형 메시지
 * @return response code 처리 결과 응답 값
 * 성공 시 200, 실패 시 400 계열 응답 코드
 */
static int _lwmsg_param_read(const int _if, const char *const _dri, PLWMessage _msg)
{
	int rsc = eLWM_RSC_BAD_REQ;

	if (_msg) {
		PLWMObject cmis = _GET_LWM_OBJECT_FROM_DICT(&_msg->body, "mis");
		if (cmis && IS_LWM_ARRAY(cmis) && LWM_ARRAY_LENGTH(cmis) == 1) {
			PLWMObject _msid = NULL;
			LWM_FOREACH(cmis->value, pxItem) {
				_msid = IW_GET_LIST_NODE_DATA(pxItem);
				const PSTMicroServiceInfo pmsi = get_micro_service_info_by_id(GET_LWM_STRING_VALUE(_msid));
				if (pmsi) {
					if (pmsi->state != RUNNING) {
						// Microsevice is not running
						rsc = eLWM_RSC_FORBIDDEN;
					}
					else {
						PLWMObject cti = _GET_LWM_OBJECT_FROM_DICT(&_msg->body, "tis");
						PLWMObject cfp = _GET_LWM_OBJECT_FROM_DICT(&_msg->body, "fps");
						PLWMObject csp = _GET_LWM_OBJECT_FROM_DICT(&_msg->body, "sps");
						BOOL is_all = (_if == eLWM_IF_TASK_PARAM_READ_ALL);

						_DPRINTK("MICROSERVICE PARAM READ%s RECEIVED\n", is_all ? " (ALL)" : "");

						if (!cti || !IS_LWM_ARRAY(cti) || IW_GET_LIST_NODE_NUM((PLWMArray)cti->value) != 1) {
							return rsc;
						}
						if (cfp && csp) {
							return rsc;
						}

						//ex) tis=[1,2,3]
						LWM_FOREACH(cti->value, pxTiItem) {
							PLWMObject _ti = IW_GET_LIST_NODE_DATA(pxTiItem);
							size_t tidx = atoi(GET_LWM_STRING_VALUE(_ti));
							PSTTaskInfo ti = get_task_info_by_idx(pmsi, tidx);
							if (is_all) {
								// 현재 상태 바로 전송
								rsc = eLWM_RSC_SUCCESS;
								PLWMessage res_msg = NEW_LWM_RES_MESSAGE(_dri, rsc); // Response메세지 생성
								struct stParameter *pparam = NULL;
								PLWMObject fp_dict = NEW_LWM_DICT_OBJECT();
								PLWMObject sp_dict = NEW_LWM_DICT_OBJECT();

								// flexible_params 메세지 생성
								LIST_FOREACH(&ti->flexible_params, pxParamItem) {
									pparam = IW_GET_LIST_NODE_DATA(pxParamItem);
									PLWMObject value = NEW_LWM_OBJECT_FROM_PARAM(pparam);
									SET_LWM_OBJECT(fp_dict, pparam->alias, value);
									pparam = NULL;
								}
								//static_params 메세지 생성
								LIST_FOREACH(&ti->static_params, pxParamItem) {
									pparam = IW_GET_LIST_NODE_DATA(pxParamItem);
									PLWMObject value = NEW_LWM_OBJECT_FROM_PARAM(pparam);
									SET_LWM_OBJECT(sp_dict, pparam->alias, value);
									pparam = NULL;
								}
								SET_LWM_BODY_OBJECT(res_msg, "fp", fp_dict); // fp 채움
								SET_LWM_BODY_OBJECT(res_msg, "sp", sp_dict); // sp 채움
								RESPONSE_LWMSG(res_msg); // Response 보냄 - 1세부
								rsc = 300;
							}
							else {
								const struct stParameter *pparam = NULL;
								iw_list_t *cp = cfp ? cfp->value : csp->value;
								PLWMObject cp_dict = NEW_LWM_DICT_OBJECT();
								BOOL found = FALSE;
								LWM_FOREACH(cp, pxParamItem) {
									PLWMObject cparam_name = IW_GET_LIST_NODE_DATA(pxParamItem);
									found = FALSE;
									// static parameter 리스트 flexible parameter 리스트
									pparam = get_parameter_from_param_list((cfp ? &ti->flexible_params : &ti->static_params), NULL, GET_LWM_STRING_VALUE(cparam_name));
									if (pparam) {
										PLWMObject value = NEW_LWM_OBJECT_FROM_PARAM(pparam); // 경량형 메세지에 파라미터 값 넣어 경령형 메세지 생성
										SET_LWM_OBJECT(cp_dict, pparam->alias, value); // cp_dict 채움
										pparam = NULL;
										found = TRUE;
									}
								}
								if (found) {
									rsc = eLWM_RSC_SUCCESS;
									// Response메세지에 셋팅(__dict, __key, __value)
									PLWMessage res_msg = NEW_LWM_RES_MESSAGE(_dri, rsc);
									SET_LWM_BODY_OBJECT(res_msg, cfp ? "fp" : "sp", cp_dict); // fp or sp 채움
									RESPONSE_LWMSG(res_msg); // Response 보냄 - 1세부
									rsc = 300;
								}
								else {
									// Parameter Not Found
									DEL_LWM_OBJECT(cp_dict);
									rsc = eLWM_RSC_NOTFOUND;
								}
							}
						}
					}
				}
				else {
					// Microservice Not Found
					rsc = eLWM_RSC_NOTFOUND;
				}
			}
		}
	}
	else {
		_DPRINTK("MICROSERVICE PARAM READ (invalid) RECEIVED\n");
	}

	return rsc;
}

/**
 * @brief Output Parameter Read관련 작업 수행
 * @details Output Parameter Read에 대한 경량형 메시지를 처리하고 실행하는 함수
 * @param _if 처리할 Interface ID 값
 * @param _dri 처리할 경량형 메시지의 dri 값
 * @param _msg 처리할 경량형 메시지
 * @return response code 처리 결과 응답 값
 * 성공 시 200, 실패 시 400 계열 응답 코드
 */
static int _lwmsg_output_param_read(const int _if, const char *const _dri, PLWMessage _msg)
{
	int rsc = eLWM_RSC_BAD_REQ;

	_DPRINTK("MICROSERVICE OUTPUT PARAM READ RECEIVED\n");

	if (_msg) {
		PLWMObject cmis = _GET_LWM_OBJECT_FROM_DICT(&_msg->body, "mis");
		if (cmis && IS_LWM_ARRAY(cmis)) {
			PLWMObject _msid = NULL;
			//ex) tis=[1,2,3]
			LWM_FOREACH(cmis->value, pxItem) {
				_msid = IW_GET_LIST_NODE_DATA(pxItem);
				const PSTMicroServiceInfo pmsi = get_micro_service_info_by_id(GET_LWM_STRING_VALUE(_msid));
				if (pmsi && pmsi->state == RUNNING) {
					// 어차피 MS 내부로 옮길거라 그냥 주석 처리
					// // 최소 하나 이상의 Task가 반복 수행(type 3 or 4) 중임
					if (IS_MS_TASKS_RUNNING(pmsi)) {
						if (strcmp(pmsi->latest_dri, _dri) == 0) {
							rsc = 300;
						}
						else {
							_DPRINTK("Microservice NO RUNNING\n");
							rsc = eLWM_RSC_BAD_REQ;
						}
					}
					else {
						LPCTaskInfo ti = NULL;
						PLWMObject cparam_name = NULL;
						// Task 리스트 , task_lwmsg_input 하는 task 에게만 전송
						LIST_FOREACH(&pmsi->tasks, pxItem) {
							ti = IW_GET_LIST_NODE_DATA(pxItem);
							if (HAS_LWMSG_INPUT_QUEUE(ti)) { //task에 경량형 메시지 입력 전용 포트 번호가 있는 지 확인
								// 빈 경량형 메세지 생성
								PLWMessage pmsg = NEW_LWM_REQ_MESSAGE(_if, _dri, NULL);
								send_queue(GET_LWMSG_INPUT_QUEUE(ti), &pmsg, 0U, ti->task_t);
							}
						}
						// 가장 최근 처리한 메시지
						strcpy(pmsi->latest_dri, _dri);
						rsc = 300;
					}
					if (rsc == 300) {
						rsc = 200;
						PLWMObject op_dict = NEW_LWM_DICT_OBJECT();
						LPCParameter poparam = &pmsi->output_param;
						if (poparam->value) {
							PLWMObject _value = NEW_LWM_OBJECT_FROM_PARAM(poparam);
							SET_LWM_OBJECT(op_dict, poparam->name, _value);
							PLWMessage res_msg = NEW_LWM_RES_MESSAGE(_dri, rsc);
							SET_LWM_BODY_OBJECT(res_msg, "op", op_dict); // op_dict 채움
							RESPONSE_LWMSG(res_msg);
						}
						rsc = 300;
					}
				}
				else if (pmsi) {
					_DPRINTK("Microservice NOT RUNNING state\n");
					rsc = 400;
				}
				else {
					_DPRINTK("Microservice NO RUNNING 404\n");
					rsc = 404;
				}
			}
		}
	}

	return rsc;
}

/**
 * @brief Task Offloading 테스트 처리 프로시져
 * @details Task Offloading 테스트 처리 프로시져
 * 
 * if == 10이 Task를 종료시키며, if == 11이 Task를 실행시킴
 * @param _if 처리할 Interface ID 값
 * @param _dri 처리할 경량형 메시지의 dri 값
 * @param _msg 처리할 경량형 메시지
 * @return response code 처리 결과 응답 값
 * 성공 시 200, 실패 시 400 계열 응답 코드
 */
static int _lwmsg_offloarding(const int _if, const char *const _dri, PLWMessage _msg)
{
	int rsc = 400;

	_DPRINTK("MICROSERVICE OFFLOARDING(%d) RECEIVED\n", _if);

	if (_msg) {
		PLWMObject cmis = _GET_LWM_OBJECT_FROM_DICT(&_msg->body, "mis");
		if (cmis && IS_LWM_ARRAY(cmis) && LWM_ARRAY_LENGTH(cmis) == 1) {
			PLWMObject _msid = NULL;
			LWM_FOREACH(cmis->value, pxItem) {
				_msid = IW_GET_LIST_NODE_DATA(pxItem);
				const PSTMicroServiceInfo pmsi = get_micro_service_info_by_id(GET_LWM_STRING_VALUE(_msid));
				PLWMObject ctis = _GET_LWM_OBJECT_FROM_DICT(&_msg->body, "tis"); // Task Index
				if (pmsi && ctis && IS_LWM_ARRAY(ctis)) {
					PLWMObject __tidx = NULL;
					BOOL bSuccess = FALSE;
					LWM_FOREACH(ctis->value, pxTaskItem) {
						__tidx = IW_GET_LIST_NODE_DATA(pxTaskItem);
						int index = atoi(GET_LWM_STRING_VALUE(__tidx));
						_DPRINTK("offloading __if %d index %d\n", _if, index);
						PSTTaskInfo ti = get_task_info_by_idx(pmsi, index);
						if (ti) {
							rsc = 200;
							if (_if == eLWM_IF_TASK_OFF) {
								bSuccess = task_onoff(ti, FALSE);
							}
							else {
								bSuccess = task_onoff(ti, TRUE);
							}
							if (!bSuccess) {
								_DPRINTK("offloading task_onoff(%s) fail!\n", ti->name);
								rsc = 400;
								break;
							}
						}
						else {
							_DPRINTK("offloading get_task_info_by_idx(%s, %d) fail!\n", pmsi->ms_id, index);
							rsc = 404;
							break;
						}
					}
				}
				else {
					rsc = 404;
				}
			}
		}
	}

	return rsc;
}

/**
 * @brief MicroService 관련 경량형 메시지 처리 Task
 * @details MicroService와 관련된 경량형 메시지를 전달받아 실제 MicroService의
 * 경량형 메시지 처리기로 전달, 처리하는 Task
 * @param pvParameters 사용하지 않음
 */
void lwmsg_dist_task(void *pvParameters)
{
	_DPRINTK("MICRO SERVICE TEST TASK started\n");

	PLWMessage lwmsg = NULL;
	int rsc = 400;
	const char *_if = NULL;
	const char *_dri = NULL;

	while (recv_queue(get_lwmsg_receive_queue(), &lwmsg, TRUE) == IW_SUCCESS) {
		rsc = 400;
		_if = NULL;
		_dri = NULL;

		if (lwmsg) {
			if (IW_GET_LIST_NODE_NUM(&lwmsg->header) > 0) {
				PLWMObject cif = _GET_LWM_OBJECT_FROM_DICT(&lwmsg->header, "if");
				PLWMObject cdri = _GET_LWM_OBJECT_FROM_DICT(&lwmsg->header, "dri");
				PLWMObject crsc = _GET_LWM_OBJECT_FROM_DICT(&lwmsg->header, "rsc");

				if (cif && cdri) {
					_if = GET_LWM_STRING_VALUE(cif);
					int __if = atoi(_if);
					_dri = GET_LWM_STRING_VALUE(cdri);

					if (__if == eLWM_IF_DEV_TASK_INFO_REQUEST) {
						PLWMessage res_msg = make_device_information_lwmsg2(_dri, 200);
						RESPONSE_LWMSG(res_msg);
						rsc = 300;
					}
					else if (__if == eLWM_IF_MS_CREATION || __if == eLWM_IF_MS_RUN || __if == eLWM_IF_MS_STOP || __if == eLWM_IF_MS_DELETE) {
						rsc = _lwmsg_operation(__if, _dri, lwmsg);
					}
					else if (__if == eLWM_IF_TASK_PARAM_SET) {
						rsc = _lwmsg_param_set(__if, _dri, lwmsg);
					}
					else if (__if == eLWM_IF_TASK_PARAM_READ || __if == eLWM_IF_TASK_PARAM_READ_ALL) {
						rsc = _lwmsg_param_read(__if, _dri, lwmsg);
					}
					else if (__if == eLWM_IF_MS_OUTPUT_PARAM_READ) {
						rsc = _lwmsg_output_param_read(__if, _dri, lwmsg);
					}
					else if (__if == eLWM_IF_TASK_ON || __if == eLWM_IF_TASK_OFF) {
						rsc = _lwmsg_offloarding(__if, _dri, lwmsg);
					}
					else {
						_DPRINTK("something received... [if : %d, dri : %s]\n", __if, _dri ? _dri : "(null)");
						rsc = eLWM_RSC_BAD_REQ;
					}
				}
				else {
					rsc = eLWM_RSC_BAD_REQ;
					_EPRINTK("if or dri not found\n");
				}
			}
			else {
				rsc = eLWM_RSC_NOMCMSG;
				_EPRINTK("header/body not found\n");
			}
		}
		else {
			rsc = eLWM_RSC_NOMCMSG;
			_EPRINTK("invalid lwmsg\n");
		}

		if (rsc != 200 && rsc != 300) {
			_EPRINTK("%d failed! [if : %s, dri : %s]\n", rsc, _if ? _if : "(null)", _dri ? _dri : "(null)");
		}

		if (rsc == 300) {
			lwmsg = NULL;
		}
		else if (_dri) {
			PLWMessage res_msg = NEW_LWM_RES_MESSAGE(_dri, rsc);
			RESPONSE_LWMSG(res_msg);
		}

		if (lwmsg)
			DEL_LWM_MESSAGE(lwmsg);
		lwmsg = NULL;

		sleep(10);
	}
}
