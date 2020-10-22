################################################################################
# MAKEFILE FOR INTERFACES
################################################################################

# Check essentials
ifndef IW_HOME
  $(error 'IW_HOME' is not defined. please set value for 'IW_HOME')
endif # !IW_HOME

IW_INTERFACE_HOME := oal/interface
#IW_INTERFACE_HOME := \
	$(patsubst $(IW_HOME)/%, %, \
	$(shell dirname \
	$(realpath \
	$(lastword \
	$(MAKEFILE_LIST)))))

#===============================================================================
# INCLUDES FOR INTERFACES
#===============================================================================

IW_INCS += $(IW_INTERFACE_HOME)

#===============================================================================
# SOURCES FOR INTERFACES
#===============================================================================

IW_SRCS += $(IW_INTERFACE_HOME)/interface_oal.c
IW_SRCS += $(IW_INTERFACE_HOME)/interface_sensor.c
IW_SRCS += $(IW_INTERFACE_HOME)/interface_gpio.c
IW_SRCS += $(IW_INTERFACE_HOME)/interface_net.c
IW_SRCS += $(IW_INTERFACE_HOME)/interface_sys.c
IW_SRCS += $(IW_INTERFACE_HOME)/interface_led.c

