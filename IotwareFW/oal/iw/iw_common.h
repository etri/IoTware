/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/
/*
 * @file		iw_common.h
 * @author		jaewon choi <jsrc17@to21.co.kr>
 * @version		1.0
 */

#ifndef IW_COMMON_H
#define IW_COMMON_H

/******************************************************************************
 * Pre-Processors
 ******************************************************************************/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

/******************************************************************************
 * Pre-Definitions
 ******************************************************************************/

typedef enum {
	IW_FAIL = -1,
	IW_SUCCESS = 0,
} iw_error_t;

/******************************************************************************
 * External Functions
 ******************************************************************************/

void iw_printf(char *format, ...);

#define	DEBUG_OFF	0		/* 1 = disable debug log */
#define	PRINTF	(DEBUG_OFF==1) ? (void)0 : iw_printf

#endif /* IW_COMMON_H */

