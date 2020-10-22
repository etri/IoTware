################################################################################
# MAKEFILE FOR DCFW
################################################################################

dcfw-src-path := Microservice/DCFW

#===============================================================================
# Includes
#===============================================================================

IW_INCS += $(dcfw-src-path)/

#===============================================================================
# Sources
#===============================================================================

IW_SRCS += $(dcfw-src-path)/iw_fw_collabomsg_internal.c
IW_SRCS += $(dcfw-src-path)/iw_fw_msg_id_x00.c
IW_SRCS += $(dcfw-src-path)/iw_fw_msg_id_x02.c
IW_SRCS += $(dcfw-src-path)/iw_fw_msg_id_x03.c
IW_SRCS += $(dcfw-src-path)/iw_fw_msg_id_x04.c
IW_SRCS += $(dcfw-src-path)/iw_fw_msg_id_x05.c
IW_SRCS += $(dcfw-src-path)/iw_fw_msg_id_x10.c
IW_SRCS += $(dcfw-src-path)/iw_fw_msg_id_x11.c
IW_SRCS += $(dcfw-src-path)/iw_fw_msg_id_x12.c
IW_SRCS += $(dcfw-src-path)/iw_fw_msg_id_x13.c
IW_SRCS += $(dcfw-src-path)/iw_fw_msg_id_x14.c
IW_SRCS += $(dcfw-src-path)/iw_fw_msg_id_x15.c
IW_SRCS += $(dcfw-src-path)/iw_fw_msg_id_x16.c
IW_SRCS += $(dcfw-src-path)/iw_fw_msg_id_x17.c
IW_SRCS += $(dcfw-src-path)/iw_fw_msg_id_x18.c
