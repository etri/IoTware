################################################################################
# MAKEFILE FOR APPLICATION
################################################################################

ifeq ($(strip $(IW_APP_HOME)),)
 $(error IW_APP_HOME is not defined. Please check IW_APP_HOME)
endif # !IW_APP_HOME

#IW_APP_HOME := app
#IW_APP_HOME ?= $(abspath $(lastword $(MAKEFILE_LIST))/../..)
#IW_APP_HOME := \
#	$(patsubst $(IW_HOME)/%, %, \
#	$(shell dirname \
#	$(realpath \
#	$(lastword \
#	$(MAKEFILE_LIST)))))

#===============================================================================
# Includes
#===============================================================================

IW_INCS += $(IW_APP_HOME)/

#===============================================================================
# Sources
#===============================================================================

IW_SRCS += $(IW_APP_HOME)/iw_start.c
IW_SRCS += $(IW_APP_HOME)/start_apps.c
# IW_SRCS += $(IW_APP_HOME)/ms_main_ums.c
# IW_SRCS += $(IW_APP_HOME)/iw_fw_microservices.c

#-------------------------------------------------------------------------------
# Add your source and includes below
#-------------------------------------------------------------------------------

# For example...
# IW_SRCS += $(IW_APP_HOME)/your_source.c
# IW_INCS += $(IW_APP_HOME)/your_include_path
