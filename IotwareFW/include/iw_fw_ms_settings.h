/*
Copyright (C) <2020> <ETRI>

This License and Service Agreement (LSA) applies to all works and their derivative works based on source form version and object form version of IoTware Project. Currently, the LSA for IoTware Project has two policies, 'Open Source License' and 'Commercial License'. Therefore, all works including the source code and executable code of IoTware Project and derivative works based thereon are subject to either 'Open Source License' or 'Commercial License' depending on the user's needs and purpose. Details related to the selection of the applicable license are specified in this LSA. If you use any or all of IoTware Project in any form, you are deemed to have consented to this LSA. If you breach any of the terms and conditions set forth in this LSA, you are solely responsible for any losses or damages incurred by Electronics and Communications Research Institute (ETRI), and ETRI assume no responsibility for you or any third party.
If you use the source form version or object form version of IoTware Project in whole or in part to develop a code or a derivative work, and you want to commercialize the result in some form, you will be covered under a commercial license. And if you are subject to a commercial license, the contract for the use of IoTware Project is subject to TECHNOLOGY LICENSE AGREEMENT of ETRI. You acknowledge that ETRI has all legal rights, title and interest, including intellectual property rights in the IoTware Project (regardless of whether such intellectual property rights are registered or where such rights exist) and agree with no objection thereto. Except as provided in a subsidiary agreement, nothing in this LSA grants you the right to use IoTware Project or the name, service mark, logo, domain name and other unique identification marks of ETRI.
If you use the source form version or object form version of IoTware Project in whole or in part to develop a code or a derivative work, and you do not commercialize the result in any form, you will be covered under an open source license. IoTware Project is in accordance with Free Software Foundation (FSF)'s open source policy, and is allowed to use it in the appropriate scope and manner, and you must comply with the applicable open source license policy applied to IoTware Project. IoTware Project is, in principle, subject to GNU Lesser General Public License version 2.1 (LGPLv2.1). If you have acquired all or a part of the IoTware Project in any way and it is subject to a license other than the open source license described above, please contact the following address for the technical support and other inquiries before use, and check the usage information.
*/

#pragma once

#include "interface_all.h"

#ifndef IW_SUCCESS
#define IW_TSK_PRIO_IDLE 0
#endif

#define IW_MS_TO_TICKS(ms) ( (iw_tick_t) ( (ms) / (IW_TICK_PERIOD_MS) ) )

#if 1
#define _DPRINTK(fmt, ...) iw_printf("[D][%s] " fmt, __func__, ## __VA_ARGS__)
#define _DPRINT(__taskinfo, fmt, ...) iw_printf("[D][%s:%s] " fmt, __taskinfo->parent->name, __taskinfo->name, ## __VA_ARGS__)
#define _EPRINTK(fmt, ...) iw_printf("[E][%s] " fmt, __func__, ## __VA_ARGS__)
#define _EPRINT(__taskinfo, fmt, ...) iw_printf("[E][%s:%s] " fmt, __taskinfo->parent->name, __taskinfo->name, ## __VA_ARGS__)
#else
#define _DPRINTK(fmt, ...) 
#define _DPRINT(__taskinfo, fmt, ...) 
#define _EPRINTK(fmt, ...) 
#define _EPRINT(__taskinfo, fmt, ...) 
#endif

/* The number of items the queue can hold at once. */
#define mainQUEUE_LENGTH					( 20 )
#define setpollQUEUE_LENGTH					10
#define setpollDONT_BLOCK 					0

#define MS_MAX_NAME_LENGTH					30
#define MS_TASK_PRIORITY					(IW_TSK_PRIO_IDLE+1)

