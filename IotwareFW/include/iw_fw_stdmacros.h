/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#pragma once

#include <stdbool.h>
#include <stdint.h>

#ifndef BOOL
typedef _Bool BOOL;
#endif

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#ifndef BYTE
typedef uint8_t BYTE;
#endif

#ifndef FLOAT
typedef float FLOAT;
#endif

#ifndef DOUBLE
typedef double DOUBLE;
#endif

#ifndef UINT8
typedef uint8_t UINT8;
#endif

#ifndef UINT16
typedef uint16_t UINT16;
#endif

#ifndef UINT32
typedef uint32_t UINT32;
#endif

#ifndef INT8
typedef int8_t INT8;
#endif

#ifndef INT16
typedef int16_t INT16;
#endif

#ifndef INT32
typedef int32_t INT32;
#endif

#ifndef EVENT
typedef uint8_t EVENT;
#endif

#ifndef STRING
typedef char *STRING;
#endif

#ifndef AUTO
#define AUTO void *
#endif


#ifndef ZeroMemory
#define ZeroMemory(__target,__size) memset(__target,0,__size)
#endif
