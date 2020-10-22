/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/
// #pragma once

#ifndef __PRIV_LOG_H
#define __PRIV_LOG_H

/*------------------------------------------------------------------------------
 * includes
 *----------------------------------------------------------------------------*/

#include "iw_common.h"

/*------------------------------------------------------------------------------
 * pre-processors 
 *----------------------------------------------------------------------------*/

#ifdef LOG_LEVEL

#define log_error(fmt, ...)    				\
	iw_printf("\x1b[31m" "E %s:%d " fmt "\x1b[0m", 	\
	          __func__, __LINE__, ##__VA_ARGS__)

#if (LOG_LEVEL > 0)
#define log_warn(fmt, ...)				\
	iw_printf("\x1b[33m" "W %s:%d " fmt "\x1b[0m",	\
		  __func__, __LINE__, ##__VA_ARGS__)
#else
#define log_warn(fmt, ...)
#endif /* LOG_LEVEL > 0 */

#if (LOG_LEVEL > 1)
#define log_info(fmt, ...)				\
	iw_printf("\x1b[32m" "I %s:%d " fmt "\x1b[0m",	\
		  __func__, __LINE__, ##__VA_ARGS__)
#else
#define log_info(fmt, ...)
#endif /* LOG_LEVEL > 1 */

#if (LOG_LEVEL > 2)
#define log_debug(fmt, ...)				\
	iw_printf("\x1b[36m" "D %s:%d " fmt "\x1b[0m",	\
		  __func__, __LINE__, ##__VA_ARGS__)
#else
#define log_debug(fmt, ...)
#endif /* LOG_LEVEL > 2 */

#endif /* LOG_LEVEL */

#endif /* !__PRIV_LOG_H */
