/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "iw_common.h"
#include "iw_oal.h"


iw_error_t iw_init_list(iw_list_t *list)
{
	if(list == NULL)
		return IW_FAIL;
	
    list->head = NULL;
    list->nodeNum = 0;

	return IW_SUCCESS;
}

iw_error_t iw_add_list_node(iw_list_t *list, iw_list_node_t *node)
{
	if((list == NULL) || (node == NULL))
		return IW_FAIL;
	
    node->next = list->head;
    node->previous = NULL;
    node->next->previous = node;
    list->head = node;
    list->nodeNum++;
    return IW_SUCCESS;    
}

iw_error_t iw_remove_list_node(iw_list_t *list, iw_list_node_t *node)
{
	if((list == NULL) || (node == NULL))
		return IW_FAIL;
	
    if(node == list->head){
        list->head = node->next;
        node->next->previous = NULL;
    }else{
        node->next->previous = node->previous;
        node->previous->next = node->next;
    }
    list->nodeNum--;
    return IW_SUCCESS;
}
