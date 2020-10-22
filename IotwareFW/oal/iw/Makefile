################################################################################
# MAKEFILE FOR IW LIBRARY
################################################################################

# Check essentials
ifndef IW_HOME 
  $(error IW_HOME is not defined. please set value for IW_HOME)
endif # !IW_HOME

IW_LIBRARY_HOME := oal/iw
#IW_LIBRARY_HOME := \
	$(patsubst $(IW_HOME)/%, %, \
	$(shell dirname \
	$(realpath \
	$(lastword \
	$(MAKEFILE_LIST)))))

#===============================================================================
# INCLUDES FOR IW LIBRARY
#===============================================================================

IW_INCS += $(IW_LIBRARY_HOME)

#===============================================================================
# SOURCES FOR IW LIBRARY
#===============================================================================

ifndef IW_BSP
  $(error IW_BSP is not defined. please set value for IW_BSP)
else
  IW_SRCS += $(IW_LIBRARY_HOME)/iw_bsp.c
  IW_SRCS += $(IW_LIBRARY_HOME)/iw_debug.c
  IW_SRCS += $(IW_LIBRARY_HOME)/iw_dev.c
  IW_SRCS += $(IW_LIBRARY_HOME)/iw_dev_batt.c
  IW_SRCS += $(IW_LIBRARY_HOME)/iw_dev_led.c
  IW_SRCS += $(IW_LIBRARY_HOME)/iw_dev_net.c
  IW_SRCS += $(IW_LIBRARY_HOME)/iw_dev_sensor.c
  IW_SRCS += $(IW_LIBRARY_HOME)/iw_if_adc.c
  IW_SRCS += $(IW_LIBRARY_HOME)/iw_if_gpio.c
  IW_SRCS += $(IW_LIBRARY_HOME)/iw_if_i2c.c
  IW_SRCS += $(IW_LIBRARY_HOME)/iw_if_uart.c
  IW_SRCS += $(IW_LIBRARY_HOME)/iw_main.c
endif # !IW_BSP || IW_BSP

ifndef IW_OS
  $(warning 'IW_OS' is not defined. 'iwlib' not supports OS)
else
  IW_SRCS += $(IW_LIBRARY_HOME)/iw_oal_fifo.c
  IW_SRCS += $(IW_LIBRARY_HOME)/iw_oal_init.c
  IW_SRCS += $(IW_LIBRARY_HOME)/iw_oal_list.c
  IW_SRCS += $(IW_LIBRARY_HOME)/iw_oal_mem.c
  IW_SRCS += $(IW_LIBRARY_HOME)/iw_oal_mutex.c
  IW_SRCS += $(IW_LIBRARY_HOME)/iw_oal_queue.c
  IW_SRCS += $(IW_LIBRARY_HOME)/iw_oal_sem.c
  IW_SRCS += $(IW_LIBRARY_HOME)/iw_oal_task.c
  IW_SRCS += $(IW_LIBRARY_HOME)/iw_oal_time.c
  IW_SRCS += $(IW_LIBRARY_HOME)/iw_sys.c
endif # !IW_OS | IW_OS

