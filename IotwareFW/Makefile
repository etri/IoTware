###############################################################################
# MAKEFILE FOR IOTWARE
###############################################################################

#==============================================================================
# DEFAULT VALUE
#==============================================================================

PROJECT_PATH ?= $(IOTWARE)/App

# export LOWPOWER_ENABLE := 0
# export LOWPOWER_ENABLE := 1

#TARGET_BSP ?= $(strip stm32)
TARGET_BSP ?= $(strip nordic)
#TARGET_BSP ?= $(strip telechips)
ifeq ($(TARGET_BSP),telechips)
  IW_EMULATOR := $(strip 1)
endif # TARGET_BSP ?= telechips

#TARGET_OS ?= $(strip freertos)
#TARGET_OS ?= $(strip riot)
#TARGET_OS ?= $(strip nanoqplus)
TARGET_OS ?= $(strip apachemynewt)
#TARGET_OS ?= $(strip contiki)

TARGET_BOARD ?= $(strip iotware)
#TARGET_BOARD ?= disposal_iot

SENS_CO2 ?= $(strip y)
#SENS_CO2 ?= n

SENS_HUMI ?= $(strip y)
#SENS_HUMI ?= n

SENS_TEMP ?= $(strip y)
#SENS_TEMP ?= n

# XXX: not supported
#NET_RF ?= y
#NET_RF ?= n

NET_BLE ?= $(strip y)
#NET_BLE ?= n

NET_LORA ?= $(strip y)
#NET_LORA ?= n

NET_WIFI ?= $(strip y )
#NET_WIFI ?= n

DEV_CLASS ?= $(strip 0)
#DEV_CLASS ?= 1
#DEV_CLASS ?= 2

#LITE_MODE ?= $(strip y)
LITE_MODE ?= $(strip n)

#RMS ?= $(strip y)
RMS ?= $(strip n)

#AMS ?= $(strip y)
AMS ?= $(strip n)

#OLMS ?= $(strip y)
OLMS ?= $(strip n)

#DCMS ?= $(strip y)
DCMS ?= $(strip n)

#TCMS ?= $(strip y)
TCMS ?= $(strip n)

#PPCMS ?= $(strip y)
PPCMS ?= $(strip n)

# check the values is set
ifndef PROJECT_PATH
 $(error PROJECT_PATH is not defined)
endif # !PROJECT_PATH

ifndef TARGET_BSP
 $(error TARGET_OS is not defined)
endif # !TARGET_BSP

ifndef TARGET_OS
 $(error TARGET_OS is not defined)
endif # !TARGET_OS

#==============================================================================
# PUBLIC CONSTANTS
#==============================================================================

#------------------------------------------------------------------------------
# PATHS
#------------------------------------------------------------------------------

export IOTWARE ?= $(abspath $(lastword $(MAKEFILE_LIST))/../..)

export IW_HOME := $(IOTWARE)/IotwareFW
export IW_EXT_HOME := $(IOTWARE)/External
export IW_APP_HOME := $(strip $(PROJECT_PATH))

export IW_MK_PATH := $(IOTWARE)/Make

#------------------------------------------------------------------------------
# OPTIONS
#------------------------------------------------------------------------------

export IW_BSP := $(TARGET_BSP)
export IW_OS := $(TARGET_OS)

export BOARD := $(TARGET_BOARD)

export JTAG_LINK := $(strip ft2232)

export IW_SENSOR_CO2 := $(SENS_CO2)
export IW_SENSOR_HUMIDITY := $(SENS_HUMI)
export IW_SENSOR_TEMPERATURE := $(SENS_TEMP)

# XXX: not supported
#export IW_NETWORK_RF = $(NET_RF)

export IW_NETWORK_BLE := $(NET_BLE)
export IW_NETWORK_LORA := $(NET_LORA)
export IW_NETWORK_WIFI := $(NET_WIFI)

export IW_DEV_CLASS := $(DEV_CLASS)

export IW_MODE_LITE := $(LITE_MODE)

export IW_SVC_RMS := $(RMS)
export IW_SVC_AMS := $(AMS)
export IW_SVC_OLMS := $(OLMS)
export IW_SVC_DCMS := $(DCMS)
export IW_SVC_TCMS := $(TCMS)
export IW_SVC_PPCMS := $(PPCMS)

#==============================================================================
# PRIVATE VARIABLES
#==============================================================================

IW_BSP_PATH := $(IW_HOME)/oal/bsp
IW_EXT_BSP_PATH	:= $(IW_EXT_HOME)/bsp/$(IW_BSP)

IW_OS_PATH := $(IW_HOME)/oal/kernel
ifeq ($(IW_OS),freertos)
 IW_EXT_OS_PATH		:= $(IW_EXT_HOME)/kernel/$(IW_OS)_$(IW_BSP)
else
 IW_EXT_OS_PATH		:= $(IW_EXT_HOME)/kernel/$(IW_OS)
endif # IW_OS? 

#==============================================================================
# COMMANDS
#==============================================================================

$(info ==============================)
$(info "IOTWARE 1.0 Release")
$(info ==============================)

all: build

build: build-all

build-all: disp-all
	make -C $(IW_MK_PATH) -f $(IW_MK_PATH)/Makefile all

clean: disp-target
	make -C $(IW_MK_PATH) -f $(IW_MK_PATH)/Makefile clean

burn: disp-target
	make -C $(IW_MK_PATH) -f $(IW_MK_PATH)/Makefile burn

burn-stflash:
	make -C $(IW_MK_PATH) -f $(IW_MK_PATH)/Makefile burn-stflash

burn-softdevice:
	make -C $(IW_MK_PATH) -f $(IW_MK_PATH)/Makefile burn-softdevice

debug:
	make -C $(IW_MK_PATH) -f $(IW_MK_PATH)/Makefile debug

gdb:
	make -C $(IW_MK_PATH) -f $(IW_MK_PATH)/Makefile gdb

ctags:
	make -C $(IW_MK_PATH) -f $(IW_MK_PATH)/Makefile ctags

check_env:
	make -C $(IW_MK_PATH) -f $(IW_MK_PATH)/Makefile check_env

ctags-clean:
	@rm -f $(IW_HOME)/tags
	
ble-all:
	make -C $(IW_HOME)/board/ble all

ble-clean:
	make -C $(IW_HOME)/board/ble clean

distclean: clean ble-clean ctags-clean
	@echo "clean IW all project OK!"

config: clean
	vi $(IW_MK_PATH)/Makefile

setup-toolchain:
	make -C $(IW_HOME)/tools/toolchain all

project-clean: 
	@rm -rf $(IW_BSP_PATH)/*
	@rm -rf $(IW_OS_PATH)/*

include oal/drivers/Makefile
project-copy: project-clean
	@cp -r $(IW_EXT_BSP_PATH)/* $(IW_BSP_PATH)/
	@cp -r $(IW_EXT_OS_PATH)/* $(IW_OS_PATH)/

project-change: project-copy
	
disp-all: disp-target disp-option disp-sensor disp-network disp-service

disp-target:
	@echo "[Target]"
	@echo
	@echo "BSP		= $(IW_BSP)"
	@echo "OS		= $(IW_OS)"
	@echo "------------------------------"

disp-option:
	@echo "[Option]"
	@echo
	@echo "DEVICE_CLASS	= $(IW_DEV_CLASS)"
	@echo "LITE_MODE	= $(IW_MODE_LITE)"
	@echo "------------------------------"

disp-sensor:
	@echo "[SENSOR]"
	@echo
	@echo "USE_CO2		= $(IW_SENSOR_CO2)"
	@echo "USE_HUMIDITY	= $(IW_SENSOR_HUMIDITY)"
	@echo "USE_TEMPERATURE	= $(IW_SENSOR_TEMPERATURE)"
	@echo "------------------------------"

disp-network:
	@echo "[NETWORK]"
	@echo
	@echo "USE_BLE		= $(IW_NETWORK_BLE)"
	@echo "USE_LORA 	= $(IW_NETWORK_LORA)"
	@echo "USE_WIFI 	= $(IW_NETWORK_WIFI)"
	@echo "------------------------------"

disp-service:
	@echo "[MICRO SERVICE]"
	@echo
	@echo "RMS		= $(IW_SVC_RMS)"
	@echo "AMS		= $(IW_SVC_AMS)"
	@echo "OLMS		= $(IW_SVC_OLMS)"
	@echo "DCMS		= $(IW_SVC_DCMS)"
	@echo "TCMS		= $(IW_SVC_TCMS)"
	@echo "PPCMS		= $(IW_SVC_PPCMS)"
	@echo "------------------------------"

print:
	$(info IOTWARE = $(IOTWARE))
	$(info IW_HOME = $(IW_HOME))
	$(info IW_EXT_HOME = $(IW_EXT_HOME))
	$(info IW_APP_HOME = $(IW_APP_HOME))
	$(info IW_MK_PATH = $(IW_MK_PATH))
	$(info IW_BSP_PATH = $(IW_BSP_PATH))
	$(info IW_OS_PATH = $(IW_OS_PATH))
	$(info IW_EXT_BSP_PATH = $(IW_EXT_BSP_PATH))
	$(info IW_EXT_OS_PATH = $(IW_EXT_OS_PATH))
	$(info IW_BSP = $(IW_BSP))
	$(info IW_OS = $(IW_OS))
	$(info BOARD = $(BOARD))
	$(info JTAG_LINK = $(JTAG_LINK))
	$(info IW_SENSOR_CO2 = $(IW_SENSOR_CO2))
	$(info IW_SENSOR_HUMIDITY = $(IW_SENSOR_HUMIDITY))
	$(info IW_SENSOR_TEMPERATURE = $(IW_SENSOR_TEMPERATURE))
	$(info IW_NETWORK_BLE = $(IW_NETWORK_BLE))
	$(info IW_NETWORK_LORA = $(IW_NETWORK_LORA))
	$(info IW_NETWORK_WIFI = $(IW_NETWORK_WIFI))
	$(info IW_DEV_CLASS = $(IW_DEV_CLASS))
	$(info IW_MODE_LITE = $(IW_MODE_LITE))
	$(info IW_SVC_RMS = $(IW_SVC_RMS))
	$(info IW_SVC_AMS = $(IW_SVC_AMS))
	$(info IW_SVC_OLMS = $(IW_SVC_OLMS))
	$(info IW_SVC_DCMS = $(IW_SVC_DCMS))
	$(info IW_SVC_TCMS = $(IW_SVC_TCMS))
	$(info IW_SVC_PPCMS = $(IW_SVC_PPCMS))
