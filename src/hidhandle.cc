/*
 * @Author: yaohengfeng 1921934563@qq.com
 * @Date: 2023-01-13 10:45:03
 * @LastEditors: yaohengfeng 1921934563@qq.com
 * @LastEditTime: 2023-03-17 16:34:52
 * @FilePath: \hid-handle\src\hidhandle.cc
 * @Description: hidhandle.cc
 */
#include <windows.h>
#include <winioctl.h>
#include <setupapi.h>
#include <initguid.h>
#include <stdio.h>
#include <hidsdi.h>
#include <tchar.h>
#include <stdlib.h> 
#include <time.h> 

#include <hidapi.h>

#include "hmi/hmi_hid.h"
#include "hmi/hmi_core.h"
#include "hmi/hmi_packet.h"

//**************************************************************************************************************

hid_device* hid_handle   = NULL;

wchar_t manufact[1024]   = { 0 };
wchar_t product[1024]    = { 0 };
wchar_t serial_num[1024] = { 0 };
wchar_t indexed[1024]    = { 0 };

/**
 * 写入文件
 * @full_path 文件路径
 * @file_name 文件名
 * @file_type 文件类型
 */
int hid_write_file_handle(const char *full_path, const char *file_name, unsigned int file_type){
	int ret = -1;
	int string_index = 1;

	printf(" ... hid ...\n");
	printf(" ... full_path =%s", full_path);
	printf(" ... file_name =%s \n", file_name);

	hid_init();

	hid_handle = hid_open(0x264a, 0x232a, NULL);
	if (hid_handle == NULL)
	{
		printf(" open hid error!\n");
		return 0;
	}
	else
	{
		printf(" open hid succeed!\n");
	}

	hid_set_nonblocking(hid_handle, 0);

	hid_get_manufacturer_string(hid_handle, manufact, sizeof(manufact));
	printf("manufact     = %ls\n", manufact);
	hid_get_product_string(hid_handle, product, sizeof(product));
	printf("product      = %ls\n", product);
	hid_get_serial_number_string(hid_handle, serial_num, sizeof(serial_num));
	printf("serial_num   = %ls\n", serial_num);
	hid_get_indexed_string(hid_handle, string_index, indexed, sizeof(indexed));
	printf("indexed      = %ls\n", indexed);

	ret = hid_write_file(hid_handle,full_path, file_name, file_type);

	hid_close(hid_handle);
	hid_handle = NULL;

	return ret;

}

/**
 * 写入buff
 * @buff 字符数组
 * @buff_len 长度
 * @file_type 文件类型
 */
int hid_write_buff_handle(unsigned char *buff, const unsigned int buff_len, unsigned int file_type){
	int ret = -1;
	int string_index = 1;

	printf(" ... hid_write_buff_handle ...\n");

	hid_init();

	hid_handle = hid_open(0x264a, 0x232a, NULL);
	if (hid_handle == NULL)
	{
		printf(" open hid error!\n");
		return 0;
	}
	else
	{
		printf(" open hid succeed!\n");
	}

	hid_set_nonblocking(hid_handle, 0);

	hid_get_manufacturer_string(hid_handle, manufact, sizeof(manufact));
	printf("manufact     = %ls\n", manufact);
	hid_get_product_string(hid_handle, product, sizeof(product));
	printf("product      = %ls\n", product);
	hid_get_serial_number_string(hid_handle, serial_num, sizeof(serial_num));
	printf("serial_num   = %ls\n", serial_num);
	hid_get_indexed_string(hid_handle, string_index, indexed, sizeof(indexed));
	printf("indexed      = %ls\n", indexed);

	ret = hid_write_buff(hid_handle,buff, buff_len, file_type);

	hid_close(hid_handle);
	hid_handle = NULL;

	return ret;

}

/**
 * hid IO 控制
 * @cmd
 * @file_name 文件名
 * @reserve
 * @rese_len
 */
int hid_io_control_handle(unsigned int cmd, const char *file_name, char *reserve, unsigned int rese_len){
	int ret = -1;
	int string_index = 1;

	unsigned char control_buff[24];

	printf(" ... hid_io_control_handle ...\n");

	hid_init();

	hid_handle = hid_open(0x264a, 0x232a, NULL);
	if (hid_handle == NULL)
	{
		printf(" open hid error!\n");
		return 0;
	}
	else
	{
		printf(" open hid succeed!\n");
	}

	hid_set_nonblocking(hid_handle, 0);

	hid_get_manufacturer_string(hid_handle, manufact, sizeof(manufact));
	printf("manufact     = %ls\n", manufact);
	hid_get_product_string(hid_handle, product, sizeof(product));
	printf("product      = %ls\n", product);
	hid_get_serial_number_string(hid_handle, serial_num, sizeof(serial_num));
	printf("serial_num   = %ls\n", serial_num);
	hid_get_indexed_string(hid_handle, string_index, indexed, sizeof(indexed));
	printf("indexed      = %ls\n", indexed);

	while (1)
	{
		ret = hid_io_control(hid_handle,cmd, file_name, reserve, rese_len);
		switch (ret)
		{
		case SYS_NO_FILE:
		{

			break;
		}
		case SYS_FAILED:
		{

			break;
		}
		case SYS_DEVICE_ERROR:
		{
			if (hid_handle)
			{
				hid_close(hid_handle);
				hid_handle = NULL;
				exit(-1);
			}

			break;
		}
		default:
			break;
		}

		Sleep(100);
	}

	hid_close(hid_handle);
	hid_handle = NULL;

	return ret;
}

/**
 * @brief 设置wifi
 * 
 * @param wifiname 
 * @param wifipasswd 
 * @return int 
 */
int hmi_send_wifi_info_handle(const char* wifiname, const char* wifipasswd){
	int ret = -1;
	int string_index = 1;

	printf(" ... hmi_send_wifi_info_handle ...\n");

	hid_init();

	hid_handle = hid_open(0x264a, 0x232a, NULL);
	if (hid_handle == NULL)
	{
		printf(" open hid error!\n");
		return 0;
	}
	else
	{
		printf(" open hid succeed!\n");
	}

	hid_set_nonblocking(hid_handle, 0);

	hid_get_manufacturer_string(hid_handle, manufact, sizeof(manufact));
	printf("manufact     = %ls\n", manufact);
	hid_get_product_string(hid_handle, product, sizeof(product));
	printf("product      = %ls\n", product);
	hid_get_serial_number_string(hid_handle, serial_num, sizeof(serial_num));
	printf("serial_num   = %ls\n", serial_num);
	hid_get_indexed_string(hid_handle, string_index, indexed, sizeof(indexed));
	printf("indexed      = %ls\n", indexed);


	ret = hmi_send_wifi_info(hid_handle, wifiname, wifipasswd);

	hid_close(hid_handle);
	hid_handle = NULL;

	return ret;
	
}

/**
 * @brief 生层UI元素
 * 
 * @param para 
 * @return int 
 */
int hmi_add_obj_handle(obj_attr_t para){
	printf("\n======hmi_add_obj_handle==========\n");

	int ret = -1;
	int string_index = 1;
	hid_init();

	hid_handle = hid_open(0x264a, 0x232a, NULL);
	if (hid_handle == NULL)
	{
		printf(" open hid error!\n");
		return 0;
	}
	else
	{
		printf(" open hid succeed!\n");
	}

	hmi_init();
	hmi_page_t *page = hmi_page_get_default(0);
	ret = hmi_add_obj(page, para);

	hmi_packet_file(page, "./UI/index.hbin");

	return ret;
}