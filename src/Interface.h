
#ifndef __INTERFACE_H_
#define __INTERFACE_H_




enum obj_type
{
    HMI_OBJ_TYPE_UNKNOWN  = 0x00,
	HMI_OBJ_TYPE_LABEL,
	HMI_OBJ_TYPE_IMG,
	HMI_OBJ_TYPE_BROKEN,
	HMI_OBJ_TYPE_CRC,
	HMI_OBJ_TYPE_STRAIGHT,
    HMI_OBJ_TYPE_PROGRESS_BAR,
    HMI_OBJ_TYPE_BG_IMAGE,
}obj_type_e;



enum obj_event
{
    HMI_OBJ_EVENT_DEFAULT  = 0x00,
	HMI_OBJ_EVENT_SYSTICK,
	HMI_OBJ_EVENT_NETTIME_DAY,
	HMI_OBJ_EVENT_NETTIME_HOUR,


}obj_event_e;


enum obj_data
{
    HMI_OBJ_DATA_DEFAULT  = 0x00,
	HMI_OBJ_DATA_BUFF,

}obj_data_e;




#define  MAX_PAGE_NUM        (30)
#define  MAX_PAGE_ELEM_NUM   (100)

#define  MAX_RESERVE_LEN     (10)

#define  MAX_OBJ_VAR_LEN      (10)
#define  MAX_OBJ_NAME_LEN     (10)
#define  MAX_OBJ_DATA_LEN     (100)



typedef struct obj_attr
{
	unsigned int   obj_id;
	unsigned int   obj_type;
			 char  obj_name[MAX_OBJ_NAME_LEN];

	unsigned int   obj_x;
	unsigned int   obj_y;
	unsigned int   obj_w;
	unsigned int   obj_h;

	unsigned char  obj_opa;
	unsigned char  obj_r;
	unsigned char  obj_g;
	unsigned char  obj_b;	

	unsigned char  obj_opa_2;
	unsigned char  obj_r_2;
	unsigned char  obj_g_2;
	unsigned char  obj_b_2;	

	unsigned char  obj_font_size;
	unsigned char  obj_font_r;
	unsigned char  obj_font_g;
	unsigned char  obj_font_b;	

	unsigned int   obj_range_min;
	unsigned int   obj_range_max;
	unsigned int   obj_angle_range;
  	unsigned int   obj_rotation;
  	unsigned int   obj_width;

	unsigned int   obj_point_x_num;
	unsigned int   obj_point_y_num;

	unsigned int   obj_time;
	
	unsigned int   obj_var[MAX_OBJ_VAR_LEN];

	unsigned int   obj_event;
	unsigned int   obj_action;
			 char  obj_data[MAX_OBJ_DATA_LEN];
	
			 char  obj_reserve[MAX_RESERVE_LEN];

}obj_attr_t;


int hmi_para_to_buff(obj_attr_t* para, unsigned char buff[]);


#endif
