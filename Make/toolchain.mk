

#PWD_SPLIT	:= $(subst /, , $(PWD))
IW_TARGET	:= IoTWare
export IW_TARGET

IW_MODULE	:= $(IW_TARGET).elf
IW_SRCS	:=
IW_INCS	:=
IW_CFLAGS	:=
IW_LDFLAGS	:=
IW_LDLIBS	:=
IW_OUT		:= out
IW_CROSS	:= arm-none-eabi-

IW_DEBUG	?= y
IW_VERBOSE	?= n
IW_DATE	:= $(shell date "+%Y-%m-%d\ %H:%M:%S")


CC          := $(IW_CROSS)gcc -std=gnu99
#AS          := $(IW_CROSS)as
AS          := $(IW_CROSS)gcc -x assembler-with-cpp
#LD          := $(IW_CROSS)ld
LD          := $(IW_CROSS)gcc
CPP         := $(IW_CROSS)cpp
CXX         := $(IW_CROSS)g++
AR          := $(IW_CROSS)ar
RANLIB      := $(IW_CROSS)ranlib
OBJCOPY     := $(IW_CROSS)objcopy
OBJDUMP     := $(IW_CROSS)objdump
SIZE        := $(IW_CROSS)size
GDB         := $(IW_CROSS)gdb

