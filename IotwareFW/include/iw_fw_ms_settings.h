/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
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

