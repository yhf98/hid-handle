/*
 * @Author: yaohengfeng 1921934563@qq.com
 * @Date: 2023-01-13 10:46:10
 * @LastEditors: yaohengfeng 1921934563@qq.com
 * @LastEditTime: 2023-03-22 15:07:04
 * @FilePath: \hid-handle\src\hidhandle.h
 * @Description: hidhandle.h
 */
#ifndef HIDHANDLE_H
#define HIDHANDLE_H

#include <vector>
#include "hmi/hmi_core.h"

using namespace std;

int hid_write_file_handle(const char *full_path, const char *file_name, unsigned int file_type);

int hid_io_control_handle(unsigned int cmd, const char *file_name, char *reserve, unsigned int rese_len);

int hid_write_buff_handle(unsigned char *buff, const unsigned int buff_len, unsigned int file_type);

int hmi_send_wifi_info_handle(const char* wifiname, const char* wifipasswd);

int generate_ui_handle(vector<obj_attr_t>& paras, const char *pkg_path);

int hmi_page_update_elem_var_handle(unsigned int id, obj_attr_t para);

int hmi_unpacket_file_handle(const char * filepath, const char * out_path);

int hmi_update_obj_var_handle();

#endif