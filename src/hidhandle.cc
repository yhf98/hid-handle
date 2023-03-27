/*
 * @Author: yaohengfeng 1921934563@qq.com
 * @Date: 2023-01-13 10:45:03
 * @LastEditors: yaohengfeng 1921934563@qq.com
 * @LastEditTime: 2023-03-27 14:29:11
 * @FilePath: \hid-handle\src\hidhandle.cc
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/*
 * @Author: yaohengfeng 1921934563@qq.com
 * @Date: 2023-01-13 10:45:03
 * @LastEditors: yaohengfeng 1921934563@qq.com
 * @LastEditTime: 2023-03-22 14:20:25
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
#include <vector>

#include "hmi/hmi_hid.h"
#include "hmi/hmi_core.h"
#include "hmi/hmi_packet.h"

using namespace std;

//**************************************************************************************************************

hid_device *hid_handle = NULL;

wchar_t manufact[1024] = {0};
wchar_t product[1024] = {0};
wchar_t serial_num[1024] = {0};
wchar_t indexed[1024] = {0};

/**
 * 写入文件
 * @full_path 文件路径
 * @file_name 文件名
 * @file_type 文件类型
 */
int hid_write_file_handle(const char *full_path, const char *file_name, unsigned int file_type)
{
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

	ret = hid_write_file(hid_handle, full_path, file_name, file_type);

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
int hid_write_buff_handle(unsigned char *buff, const unsigned int buff_len, unsigned int file_type)
{
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

	ret = hid_write_buff(hid_handle, buff, buff_len, file_type);

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
int hid_io_control_handle(unsigned int cmd, const char *file_name, char *reserve, unsigned int rese_len)
{
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
		ret = hid_io_control(hid_handle, cmd, file_name, reserve, rese_len);
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
int hmi_send_wifi_info_handle(const char *wifiname, const char *wifipasswd)
{
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
int generate_ui_handle(vector<obj_attr_t> &paras, const char *pkg_path)
{
	int ret = -1;
	printf("\n======hmi_add_obj_handle==========\n");

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
	for (const auto &obj : paras)
	{
		ret = hmi_add_obj(page, obj);
		printf("\nobj_id: %d\n", obj.obj_id);
	}

	// hmi_create_obj_test();

	printf("打包地址：%s", pkg_path);

	ret = hmi_packet_file(page, pkg_path);

	hid_close(hid_handle);
	hid_handle = NULL;

	// hmi_unpacket_file("./ui/index1.hbin");

	return ret;
}

int hmi_page_update_elem_var_handle(unsigned int id, obj_attr_t para)
{
	int ret = -1;
	printf("\n======hmi_page_update_elem_var_handle==========\n");

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

	int i = 0;
	unsigned int page_id = 0;
	unsigned int elem_id = 0;
	unsigned int data_type = 0;
	unsigned int elem_data_len = 0;

	unsigned int var0 = 0;
	unsigned int var1 = 0;
	unsigned int var2 = 0;
	unsigned int var3 = 0;

	char control_buff[24];

	memset(control_buff, 0, sizeof(control_buff));

	srand(time(0));
	var0 = rand() % 100;
	var1 = rand() % 100;
	var2 = rand() % 100;
	var3 = rand() % 100;

	printf("var0=%d\n", var0);
	printf("var1=%d\n", var1);
	printf("var2=%d\n", var2);
	printf("var3=%d\n", var3);

	for (i = 0; i < 4; i++)
	{

		page_id = 0;
		elem_id = 0x07 + i;
		printf("\n\n========elem_id=========%d\n\n", elem_id);

		data_type = HMI_OBJ_DATA_BUFF;

		elem_data_len = 3;

		control_buff[0] = ((page_id >> 0) & 0xff);
		control_buff[1] = ((page_id >> 8) & 0xff);

		control_buff[2] = ((elem_id >> 0) & 0xff);
		control_buff[3] = ((elem_id >> 8) & 0xff);

		control_buff[4] = ((data_type >> 0) & 0xff);
		control_buff[5] = ((data_type >> 8) & 0xff);

		control_buff[6] = ((elem_data_len >> 0) & 0xff);
		control_buff[7] = ((elem_data_len >> 8) & 0xff);

		switch (i)
		{
		case 0:
		{
			sprintf(control_buff + 8, "memory:%d%", var0);
			break;
		}
		case 1:
		{
			sprintf(control_buff + 8, "cpu:%d%", var1);
			break;
		}
		case 2:
		{
			sprintf(control_buff + 8, "net:%d%", var2);
			break;
		}
		case 3:
		{
			sprintf(control_buff + 8, "disk:%d%", var3);
			break;
		}
		default:
			break;
		}

		ret = hid_io_control(hid_handle, CMD_ELEM_UPDATE, " ", control_buff, sizeof(control_buff));
	}

	// for (i = 0; i < 4; i++)
	// {

	// 	page_id = 0;
	// 	elem_id = 0x07 + i;
	// 	// data_type = HMI_OBJ_DATA_DEFAULT;
	// 	data_type = HMI_OBJ_DATA_BUFF;

	// 	elem_data_len = 3;

	// 	control_buff[0] = ((page_id >> 0) & 0xff);
	// 	control_buff[1] = ((page_id >> 8) & 0xff);

	// 	control_buff[2] = ((elem_id >> 0) & 0xff);
	// 	control_buff[3] = ((elem_id >> 8) & 0xff);

	// 	control_buff[4] = ((data_type >> 0) & 0xff);
	// 	control_buff[5] = ((data_type >> 8) & 0xff);

	// 	control_buff[6] = ((elem_data_len >> 0) & 0xff);
	// 	control_buff[7] = ((elem_data_len >> 8) & 0xff);

	// 	switch (i)
	// 	{
	// 	case 0:
	// 	{
	// 		control_buff[8] = ((var0 >> 0) & 0xff);
	// 		control_buff[9] = ((var0 >> 8) & 0xff);

	// 		control_buff[10] = ((var0 >> 0) & 0xff);
	// 		control_buff[11] = ((var0 >> 8) & 0xff);

	// 		control_buff[12] = ((var0 >> 0) & 0xff);
	// 		control_buff[13] = ((var0 >> 8) & 0xff);
	// 		break;
	// 	}
	// 	case 1:
	// 	{
	// 		control_buff[8] = ((var1 >> 0) & 0xff);
	// 		control_buff[9] = ((var1 >> 8) & 0xff);

	// 		control_buff[10] = ((var1 >> 0) & 0xff);
	// 		control_buff[11] = ((var1 >> 8) & 0xff);

	// 		control_buff[12] = ((var1 >> 0) & 0xff);
	// 		control_buff[13] = ((var1 >> 8) & 0xff);
	// 		break;
	// 	}
	// 	case 2:
	// 	{
	// 		control_buff[8] = ((var2 >> 0) & 0xff);
	// 		control_buff[9] = ((var2 >> 8) & 0xff);

	// 		control_buff[10] = ((var2 >> 0) & 0xff);
	// 		control_buff[11] = ((var2 >> 8) & 0xff);

	// 		control_buff[12] = ((var2 >> 0) & 0xff);
	// 		control_buff[13] = ((var2 >> 8) & 0xff);
	// 		break;
	// 	}
	// 	case 3:
	// 	{
	// 		control_buff[8] = ((var3 >> 0) & 0xff);
	// 		control_buff[9] = ((var3 >> 8) & 0xff);

	// 		control_buff[10] = ((var3 >> 0) & 0xff);
	// 		control_buff[11] = ((var3 >> 8) & 0xff);

	// 		control_buff[12] = ((var3 >> 0) & 0xff);
	// 		control_buff[13] = ((var3 >> 8) & 0xff);
	// 		break;
	// 	}
	// 	default:
	// 		break;
	// 	}

	// 	hid_io_control(hid_handle, CMD_ELEM_UPDATE, " ", control_buff, sizeof(control_buff));
	// }
	return ret;
}

/**
 * @brief 解压hbin文件
 *
 * @param filepath 文件路径
 * @param out_path 解压路径
 * @return int
 */
int hmi_unpacket_file_handle(const char *filepath, const char *out_path)
{
	int ret = -1;
	int string_index = 1;

	unsigned char control_buff[24];

	printf(" ... hmi_unpacket_file_handle ...\n");

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

	ret = hmi_unpacket_file(filepath, out_path);

	return ret;
}

int hmi_update_obj_var_handle()
{
	int ret = -1;
	int string_index = 1;

	printf(" ... hmi_unpacket_file_handle ...\n");

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

	int i = 0;
	unsigned int page_id = 0;
	unsigned int elem_id = 0;
	unsigned int data_type = 0;
	unsigned int elem_data_len = 0;

	unsigned int var0 = 0;
	unsigned int var1 = 0;
	unsigned int var2 = 0;
	unsigned int var3 = 0;

	char control_buff[64];

	memset(control_buff, 0, sizeof(control_buff));

	srand(time(0));
	var0 = rand() % 100;
	var1 = rand() % 100;
	var2 = rand() % 100;
	var3 = rand() % 100;

	printf("var0=%d\n", var0);
	printf("var1=%d\n", var1);
	printf("var2=%d\n", var2);
	printf("var3=%d\n", var3);

	page_id = 0;
	elem_id = 8;

	data_type = HMI_OBJ_DATA_DEFAULT;

	elem_data_len = 3;

	control_buff[0] = ((page_id >> 0) & 0xff);
	control_buff[1] = ((page_id >> 7) & 0xff);

	control_buff[2] = ((elem_id >> 0) & 0xff);
	control_buff[3] = ((elem_id >> 7) & 0xff);

	control_buff[4] = ((data_type >> 0) & 0xff);
	control_buff[5] = ((data_type >> 7) & 0xff);

	control_buff[6] = ((elem_data_len >> 0) & 0xff);
	control_buff[7] = ((elem_data_len >> 7) & 0xff);

	control_buff[8] = ((var0 >> 0) & 0xff);
	control_buff[9] = ((var0 >> 7) & 0xff);

	control_buff[10] = ((var1 >> 0) & 0xff);
	control_buff[11] = ((var1 >> 7) & 0xff);

	control_buff[12] = ((var2 >> 0) & 0xff);
	control_buff[13] = ((var2 >> 7) & 0xff);

	hid_io_control(hid_handle, CMD_ELEM_UPDATE, " ", control_buff, sizeof(control_buff));

	return 0;
}

/**
 * @brief 更新屏幕元素的数据
 *
 * @param paras
 * @return int int
 */
int hmi_update_screen_data(unsigned int elem_id, const char *data)
{
	int ret = -1;
	printf("\n======hmi_update_screen_data==========\n");

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

	int i = 0;
	unsigned int page_id = 0;
	// unsigned int elem_id = 0;
	unsigned int data_type = 0;
	unsigned int elem_data_len = 0;

	char control_buff[64];

	memset(control_buff, 0, sizeof(control_buff));

	page_id = 0;
	elem_id = 0;

	data_type = HMI_OBJ_DATA_BUFF;

	elem_data_len = 3;

	control_buff[0] = ((page_id >> 0) & 0xff);
	control_buff[1] = ((page_id >> 8) & 0xff);

	control_buff[2] = ((elem_id >> 0) & 0xff);
	control_buff[3] = ((elem_id >> 8) & 0xff);

	control_buff[4] = ((data_type >> 0) & 0xff);
	control_buff[5] = ((data_type >> 8) & 0xff);

	control_buff[6] = ((elem_data_len >> 0) & 0xff);
	control_buff[7] = ((elem_data_len >> 8) & 0xff);

	sprintf(control_buff + 8, "%s", data);

	ret = hid_io_control(hid_handle, CMD_ELEM_UPDATE, " ", control_buff, sizeof(control_buff));

	hid_close(hid_handle);
	hid_handle = NULL;
	delete[] control_buff;

	return ret;
}

/**
 * @brief 批量获取更新屏幕数据
 *
 * @param paras
 * @return int
 */
int hmi_batch_update_screen_data(vector<obj_attr_t> &paras)
{
	int ret = -1;
	printf("\n======hmi_update_screen_data==========\n");

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

	unsigned int page_id = 0;
	unsigned int elem_id = 0;
	unsigned int data_type = 0;
	unsigned int elem_data_len = 0;

	char control_buff[64];
	memset(control_buff, 0, sizeof(control_buff));
	// 更新obj_data
	for (const auto &obj : paras)
	{

		page_id = 0;
		elem_id = obj.obj_id;

		data_type = HMI_OBJ_DATA_BUFF;

		elem_data_len = 3;

		control_buff[0] = ((page_id >> 0) & 0xff);
		control_buff[1] = ((page_id >> 8) & 0xff);

		control_buff[2] = ((elem_id >> 0) & 0xff);
		control_buff[3] = ((elem_id >> 8) & 0xff);

		control_buff[4] = ((data_type >> 0) & 0xff);
		control_buff[5] = ((data_type >> 8) & 0xff);

		control_buff[6] = ((elem_data_len >> 0) & 0xff);
		control_buff[7] = ((elem_data_len >> 8) & 0xff);

		sprintf(control_buff + 8, "%s", obj.obj_data);

		ret = hid_io_control(hid_handle, CMD_ELEM_UPDATE, " ", control_buff, sizeof(control_buff));
	}

	// 更新obj_var
	for (const auto &obj : paras)
	{

		page_id = 0;
		elem_id = obj.obj_id;

		data_type = HMI_OBJ_DATA_DEFAULT;

		elem_data_len = 3;

		control_buff[0] = ((page_id >> 0) & 0xff);
		control_buff[1] = ((page_id >> 8) & 0xff);

		control_buff[2] = ((elem_id >> 0) & 0xff);
		control_buff[3] = ((elem_id >> 8) & 0xff);

		control_buff[4] = ((data_type >> 0) & 0xff);
		control_buff[5] = ((data_type >> 8) & 0xff);

		control_buff[6] = ((elem_data_len >> 0) & 0xff);
		control_buff[7] = ((elem_data_len >> 8) & 0xff);

		control_buff[8] = ((obj.obj_var[0] >> 0) & 0xff);
		control_buff[9] = ((obj.obj_var[0] >> 7) & 0xff);
		
		ret = hid_io_control(hid_handle, CMD_ELEM_UPDATE, " ", control_buff, sizeof(control_buff));
	}


	hid_close(hid_handle);
	hid_handle = NULL;
	delete[] control_buff;

	return ret;
}