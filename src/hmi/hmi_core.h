

#ifndef __HMI_CORE_H__
#define __HMI_CORE_H__

#include "stdio.h"
#include "list.h"
#include "hmi_config.h"
//********************************************************************

#define HMI_OBJ_TYPE_UNKNOWN (0)
#define HMI_OBJ_TYPE_LABEL (1)
#define HMI_OBJ_TYPE_IMG (2)
#define HMI_OBJ_TYPE_BROKEN (3)
#define HMI_OBJ_TYPE_CRC (4)
#define HMI_OBJ_TYPE_STRAIGHT (5)
#define HMI_OBJ_TYPE_PROGRESS_BAR (6)
#define HMI_OBJ_TYPE_BG_IMAGE (7)
#define HMI_OBJ_TYPE_BG_VIDEO (8)
#define HMI_OBJ_TYPE_BG_MUSIC (9)
#define HMI_OBJ_TYPE_WIFI_INFO (10)
#define HMI_OBJ_TYPE_PROGRESS_PNG (11)

#define HMI_OBJ_EVENT_DEFAULT (0)
#define HMI_OBJ_EVENT_SYSTICK (1)
#define HMI_OBJ_EVENT_NETTIME_DAY (2)
#define HMI_OBJ_EVENT_NETTIME_HOUR (3)
#define HMI_OBJ_EVENT_URL_DATA (4)

#define HMI_OBJ_DATA_DEFAULT (0)
#define HMI_OBJ_DATA_BUFF (1)

#define MAX_PAGE_NUM (30)
#define MAX_PAGE_ELEM_NUM (100)

#define MAX_OBJ_VAR_LEN (10)
#define MAX_OBJ_NAME_LEN (10)
#define MAX_OBJ_DATA_LEN (100)
#define MAX_RESERVE_LEN (10)
#define MAX_OBJ_URL_LEN (256)
#define MAX_OBJ_FIELD_LEN (50)

#define  MAX_OBJ_FONT_LEN      	(30)

#define MAX_OBJ_WIFI_LEN (10)

#define MAX_OBJ_PROGRESS_LEN (100)
#define MAX_OBJ_PROGRESS_NAME_LEN (10)

//********************************************************************

#pragma pack(4)

typedef struct obj_attr
{
	unsigned int obj_id;
	unsigned int obj_type;
	char obj_name[MAX_OBJ_NAME_LEN]; //4+4+10

	char obj_wifi_name[MAX_OBJ_WIFI_LEN];
	char obj_wifi_pass[MAX_OBJ_WIFI_LEN];

	unsigned int obj_x;
	unsigned int obj_y;
	unsigned int obj_w;
	unsigned int obj_h; //16

	unsigned char obj_opa;
	unsigned char obj_r;
	unsigned char obj_g;
	unsigned char obj_b;

	unsigned char obj_opa_2;
	unsigned char obj_r_2;
	unsigned char obj_g_2;
	unsigned char obj_b_2;

	unsigned char obj_opa_3;
	unsigned char obj_r_3;
	unsigned char obj_g_3;
	unsigned char obj_b_3;

	char obj_font_name[MAX_OBJ_FONT_LEN];
	unsigned char obj_font_size;
	unsigned char obj_font_r;
	unsigned char obj_font_g;
	unsigned char obj_font_b; //12

	unsigned int obj_val_flag;

	unsigned int obj_rise_r;
	unsigned int obj_rise_g;
	unsigned int obj_rise_b;

	unsigned int obj_fall_r;
	unsigned int obj_fall_g;
	unsigned int obj_fall_b;

	unsigned int obj_range_min;
	unsigned int obj_range_max;
	unsigned int obj_angle_range;
	unsigned int obj_rotation;
	unsigned int obj_width; //20

	unsigned int obj_progress_interval;
	char obj_progress_name[MAX_OBJ_PROGRESS_LEN][MAX_OBJ_PROGRESS_NAME_LEN];

	unsigned int obj_point_x_num;
	unsigned int obj_point_y_num; //8

	unsigned int obj_time; //4

	unsigned int obj_var[MAX_OBJ_VAR_LEN]; //40

	unsigned int obj_event;
	unsigned int obj_action;		 //8
	char obj_data[MAX_OBJ_DATA_LEN]; //100

	char obj_url[MAX_OBJ_URL_LEN];
	char obj_field[MAX_OBJ_FIELD_LEN];

	char obj_reserve[MAX_RESERVE_LEN]; //10

	unsigned int *obj_font;

	char obj_align_reserve[8]; //for obj align

} obj_attr_t;

typedef struct device_info
{
	wchar_t manufact[1024] = {0};
	wchar_t product[1024] = {0};
	wchar_t serial_num[1024] = {0};
	wchar_t indexed[1024] = {0};
} device_info_t;

int hmi_para_to_buff(obj_attr_t *para, unsigned char buff[]);

//********************************************************************

typedef struct hmi_element
{

	obj_attr_t elem_attr;
	slist_t elem_l_tail;

	unsigned char elem_reserve[MAX_RESERVE_LEN];
} hmi_element_t;

//********************************************************************
typedef struct hmi_page
{

	unsigned int page_id;

	unsigned int page_num;

	unsigned int page_elem_num;

	slist_t *page_elem_head;

	unsigned char page_reserve[MAX_RESERVE_LEN];

} hmi_page_t;

//********************************************************************

hmi_element_t *Hmi_elem_create(obj_attr_t para);
int Hmi_elem_del(hmi_element_t *obj);

int hmi_para_to_buff(obj_attr_t *para, char buff[], unsigned int *send_len);
int hmi_struct_size_info(void);
int hmi_page_manage_init(void);
int hmi_page_init(hmi_page_t *page, unsigned int id);
int hmi_page_free(hmi_page_t *page);
hmi_page_t *hmi_page_get_default(unsigned int id);
hmi_page_t *hmi_page_get_by_id(unsigned int id);
int hmi_page_add_elem(hmi_page_t *page, obj_attr_t para);
int hmi_page_del_elem(hmi_page_t *page, hmi_element_t *obj);
hmi_element_t *hmi_page_get_elem_by_id(hmi_page_t *page, unsigned int elem_id);
int hmi_page_trav_elem(hmi_page_t *page);
int hmi_page_del_all_elem(hmi_page_t *page);

int hmi_page_update_elem_var(hmi_page_t *page, unsigned int id, obj_attr_t para);
int hmi_page_update_elem_data(hmi_page_t *page, unsigned int id, obj_attr_t para);

int hmi_init(void);
int hmi_add_obj(hmi_page_t *page, obj_attr_t para);
int hmi_del_obj(hmi_page_t *page, unsigned int obj_id);

int hmi_config_wifi_info(const char *wifi_name, const char *wifi_pwd);

//********************************************************************
//for test

int hmi_create_obj_test(void);

//********************************************************************

#endif
