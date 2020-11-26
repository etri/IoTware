/*
Copyright (C) <2020> <ETRI>

This License and Service Agreement (LSA) applies to all works and their derivative works based on source form version and object form version of IoTware Project. Currently, the LSA for IoTware Project has two policies, 'Open Source License' and 'Commercial License'. Therefore, all works including the source code and executable code of IoTware Project and derivative works based thereon are subject to either 'Open Source License' or 'Commercial License' depending on the user's needs and purpose. Details related to the selection of the applicable license are specified in this LSA. If you use any or all of IoTware Project in any form, you are deemed to have consented to this LSA. If you breach any of the terms and conditions set forth in this LSA, you are solely responsible for any losses or damages incurred by Electronics and Communications Research Institute (ETRI), and ETRI assume no responsibility for you or any third party.
If you use the source form version or object form version of IoTware Project in whole or in part to develop a code or a derivative work, and you want to commercialize the result in some form, you will be covered under a commercial license. And if you are subject to a commercial license, the contract for the use of IoTware Project is subject to TECHNOLOGY LICENSE AGREEMENT of ETRI. You acknowledge that ETRI has all legal rights, title and interest, including intellectual property rights in the IoTware Project (regardless of whether such intellectual property rights are registered or where such rights exist) and agree with no objection thereto. Except as provided in a subsidiary agreement, nothing in this LSA grants you the right to use IoTware Project or the name, service mark, logo, domain name and other unique identification marks of ETRI.
If you use the source form version or object form version of IoTware Project in whole or in part to develop a code or a derivative work, and you do not commercialize the result in any form, you will be covered under an open source license. IoTware Project is in accordance with Free Software Foundation (FSF)'s open source policy, and is allowed to use it in the appropriate scope and manner, and you must comply with the applicable open source license policy applied to IoTware Project. IoTware Project is, in principle, subject to GNU Lesser General Public License version 2.1 (LGPLv2.1). If you have acquired all or a part of the IoTware Project in any way and it is subject to a license other than the open source license described above, please contact the following address for the technical support and other inquiries before use, and check the usage information.
*/
#ifndef IW_SYS_H
#define IW_SYS_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "iw_dev.h"
#include "iw_dev_net.h"
#include "iw_dev_sensor.h"
#include "iw_oal.h"
#include "iw_bsp.h"

typedef enum
{
	MCU_TYPE_UNKNOWN    = 0,
    MCU_TYPE_STM32L476  ,
    MCU_TYPE_NRF52840   ,
    MCU_TYPE_CORTEX_M0  ,
    MCU_TYPE_TCC_EMUL   ,    
    MCU_TYPE_RESERVED   ,
} mcu_type_t;

typedef enum
{
    POW_RUN_MODE = 0,
    POW_DYNAMIC_MODE,
    POW_LOW_MODE,
    POW_SLEEP_MODE,
} power_mode_t;

typedef enum
{
    AUTOCONTROL_OFF = 0,
    AUTOCONTROL_ON,
} auto_control_t;

// typedef enum
// {
//     AUTOCONTROL_OFFMODE        = 0,
//     AUTOCONTROL_ONMODE         ,
// }autocontrol_mode_t;

// typedef enum
// {
//     LOWPOWER_OFFMODE        = 0,
//     LOWPOWER_ONMODE         ,
// }lowpower_mode_t;

#if (IW_NANOQPLUS == 1)
#define TOTAL_HEAP_SIZE  ((size_t)nos_total_heap_size())
#define FREE_HEAP_SIZE   ((size_t)nos_free_heap_size())

#elif (IW_FREERTOS == 1)

#if ( IW_MCU_STM32 == 1 )

#define TOTAL_HEAP_SIZE ((size_t) xTotalHeapSize)
#define FREE_HEAP_SIZE   ((size_t)xPortGetFreeHeapSize())
extern size_t xPortGetFreeHeapSize(), xTotalHeapSize;

#elif( IW_MCU_NORDIC == 1 )

#define TOTAL_HEAP_SIZE ((size_t) configTOTAL_HEAP_SIZE)
#define FREE_HEAP_SIZE   ((size_t)xPortGetFreeHeapSize())
extern size_t xPortGetFreeHeapSize();

#elif( IW_MCU_TCC == 1 )

#define TOTAL_HEAP_SIZE  ((size_t)(&__UserSpaceEnd) - (size_t)(&__UserSpaceStart))
#define FREE_HEAP_SIZE   ((size_t)xPortGetFreeHeapSize())
extern size_t xPortGetFreeHeapSize();
extern uint32_t __UserSpaceStart, __UserSpaceEnd;

#endif //( IW_MCU_STM32 == 1)

#elif( IW_RIOT == 1 )

#include "kernel_types.h"
#include "thread.h"
extern uint32_t riot_total_heap_size();
extern uint32_t riot_free_heap_size();
#define TOTAL_HEAP_SIZE  ((size_t)riot_total_heap_size())
#define FREE_HEAP_SIZE   ((size_t)riot_free_heap_size())

#elif( IW_APACHEMYNEWT == 1 )

#define TOTAL_HEAP_SIZE  (__HEAP_SIZE)
#define FREE_HEAP_SIZE   (__HEAP_SIZE) // TODO: How to check free heap size

#endif // (IW_NANOQPLUS == 1)


#if ( IW_MCU_NORDIC == 1 || IW_MCU_TCC == 1 )
#define GLOBAL_DATA_SIZE ((size_t)(&__bss_end__) - (size_t)(&__data_start__))
extern uint32_t __bss_end__, __data_start__;

#elif( IW_MCU_STM32 == 1 )

#if (IW_FREERTOS == 1)
#define GLOBAL_DATA_SIZE ((size_t)(&__bss_end__) - (size_t)(&_sdata) - TOTAL_HEAP_SIZE)
#else
#define GLOBAL_DATA_SIZE ((size_t)(&__bss_end__) - (size_t)(&_sdata))
#endif

extern uint32_t __bss_end__, _sdata;

#else
#error "please check your $(IW_CHIP)!!!"
#endif

iw_error_t iw_get_mcu_info(mcu_type_t *id);
const char* iw_get_mcu_name(mcu_type_t type);

iw_error_t iw_set_autocontrol(auto_control_t enable);
iw_error_t iw_get_autocontrol(auto_control_t *enable);
iw_error_t iw_set_power_mode(power_mode_t pwr_mode);
iw_error_t iw_get_power_mode(power_mode_t *pwr_mode);
// iw_error_t iw_get_lowpower_status(lowpower_mode_t *enable);
// iw_error_t iw_set_lowpower_status(lowpower_mode_t enable);

iw_error_t iw_get_battery_status(int *percent, int *time);

iw_error_t iw_get_memory_status(uint32_t *global_data, uint32_t *total_heap, uint32_t *free_heap);

iw_error_t iw_get_network_status(uint8_t dev_id, int *connected, int *strength, wake_src_type_t *wake_src, net_pwr_mode_t *mode);

iw_error_t iw_get_sensor_status(uint8_t dev_id, wake_src_type_t *wake_src, sensor_pwr_mode_t *mode);



void iw_get_cmd_str(char *buf);

#endif /* IW_SYS_H */

