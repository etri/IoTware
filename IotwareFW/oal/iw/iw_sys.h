/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
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

