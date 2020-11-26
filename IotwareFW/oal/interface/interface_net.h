/*
Copyright (C) <2020> <ETRI>

This License and Service Agreement (LSA) applies to all works and their derivative works based on source form version and object form version of IoTware Project. Currently, the LSA for IoTware Project has two policies, 'Open Source License' and 'Commercial License'. Therefore, all works including the source code and executable code of IoTware Project and derivative works based thereon are subject to either 'Open Source License' or 'Commercial License' depending on the user's needs and purpose. Details related to the selection of the applicable license are specified in this LSA. If you use any or all of IoTware Project in any form, you are deemed to have consented to this LSA. If you breach any of the terms and conditions set forth in this LSA, you are solely responsible for any losses or damages incurred by Electronics and Communications Research Institute (ETRI), and ETRI assume no responsibility for you or any third party.
If you use the source form version or object form version of IoTware Project in whole or in part to develop a code or a derivative work, and you want to commercialize the result in some form, you will be covered under a commercial license. And if you are subject to a commercial license, the contract for the use of IoTware Project is subject to TECHNOLOGY LICENSE AGREEMENT of ETRI. You acknowledge that ETRI has all legal rights, title and interest, including intellectual property rights in the IoTware Project (regardless of whether such intellectual property rights are registered or where such rights exist) and agree with no objection thereto. Except as provided in a subsidiary agreement, nothing in this LSA grants you the right to use IoTware Project or the name, service mark, logo, domain name and other unique identification marks of ETRI.
If you use the source form version or object form version of IoTware Project in whole or in part to develop a code or a derivative work, and you do not commercialize the result in any form, you will be covered under an open source license. IoTware Project is in accordance with Free Software Foundation (FSF)'s open source policy, and is allowed to use it in the appropriate scope and manner, and you must comply with the applicable open source license policy applied to IoTware Project. IoTware Project is, in principle, subject to GNU Lesser General Public License version 2.1 (LGPLv2.1). If you have acquired all or a part of the IoTware Project in any way and it is subject to a license other than the open source license described above, please contact the following address for the technical support and other inquiries before use, and check the usage information.
*/

#ifndef INTERFACE_NET_H
#define INTERFACE_NET_H

#include "iw_common.h"
#include "iw_dev_net.h"

uint8_t get_registered_net_num(void);
iw_error_t get_all_networks_info(registered_net_info_t *info);
iw_error_t init_network(unsigned char dev_id);
iw_error_t is_ready_network(unsigned char dev_id, dev_init_status_t *status); 
iw_error_t deinit_network(unsigned char dev_id);
iw_error_t available_network(unsigned char dev_id, unsigned int *len);
iw_error_t recv_data_network(unsigned char dev_id, char *buf, unsigned int *len);
iw_error_t send_data_network( unsigned char dev_id, char *buf, unsigned int len);
int connect_network(unsigned char dev_id, unsigned int timeout);
iw_error_t disconnect_network(unsigned char dev_id);
iw_error_t is_connected_network(unsigned char dev_id, int *connected);
int scan_network(unsigned char dev_id, int *rssi);	
iw_error_t get_capa_network(unsigned char dev_id, wake_src_type_t *wake_src);
iw_error_t set_config_network(unsigned char dev_id, wake_src_type_t wake_src, void *config);
iw_error_t get_config_network(unsigned char dev_id, wake_src_type_t *wake_src, void *config);
iw_error_t set_pwrmode_network(unsigned char dev_id, net_pwr_mode_t pwr_mode);
iw_error_t get_pwrmode_network(unsigned char dev_id, net_pwr_mode_t *pwr_mode); 
iw_error_t get_rssi_network(unsigned char dev_id, int *rssi);
int connect_server(unsigned char dev_id, char *address, int port, unsigned int timeout, int proto);
int disconnect_server(unsigned char dev_id);
iw_error_t get_network_info(net_type_t type,  char *name, registered_net_info_t *info);
iw_error_t get_network_devid(net_type_t type, char *name, unsigned char *devid);
iw_error_t get_network_dev_id_by_type(net_type_t type, uint8_t *dev_id);
iw_error_t get_network_dev_id_by_name(char *name, uint8_t *dev_id);
int get_connected_network(unsigned char dev_id);

#endif /* INTERFACE_NET_H */
