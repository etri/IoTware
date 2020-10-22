/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/
#ifndef IW_BSP_H
#define IW_BSP_H

#include "bsp_main.h"
#include "bsp_rtc.h"
#include "iw_oal.h"


/******************************************************************************
 * Pre-Processors
 ******************************************************************************/

/* UART Port for debugging */
#define DEBUG_UART_PORT			BSP_UART1
#define DEBUG_UART_BAUDRATE		(115200)


void iw_init_mcu(void);
void iw_init_bsp(void);

bool iw_set_rtc_time(DATE_TIME_T  *dt);
bool iw_get_rtc_time(DATE_TIME_T  *dt);

/**
 * @brief	delay mili-second
 * @param	[in]	ms		time in mili-second to wait for lock
 */
void iw_delay_ms(unsigned int ms);

/**
 * @brief	delay micro-second
 * @param	[in]	us		time in micro-second to wait for lock
 */
void iw_delay_us(unsigned int us);
#endif /* IW_BSP_H */

