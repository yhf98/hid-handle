
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

#include "Interface.h"

#define    CMD_OBJ_DEL                   (0xA0)
#define    CMD_SHOW_PNG                  (0xA1)
#define    CMD_SHOW_JPG                  (0xA2)
#define    CMD_PLAY_VIDEO                (0xA3)
#define    CMD_PLAY_AUDIO                (0xA4)
#define    CMD_SHOW_LINE_CHART           (0xA5)
#define    CMD_SHOW_TXT                  (0xA6)
#define    CMD_SOFT_UPDATE               (0xA7)
#define    CMD_SHOW_STRA_CHART           (0xA8)
#define    CMD_SHOW_METTER_CRC           (0xA9)
#define    CMD_SHOW_METTER_DASHBOARD     (0xAA)
#define    CMD_STOP_VIDEO			     (0xAB)
#define    CMD_STOP_PHOTO			     (0xAC)
#define    CMD_STOP_AUDIO			     (0xAD)
#define    CMD_ELEM_UPDATE			     (0xAE)

int hid_io_control(unsigned int cmd, const char* file_name, char* control_buff, unsigned int control_buff_len);

int hmi_para_to_buff(obj_attr_t *para ,  char buff[],unsigned int * send_len)
{
	int i   = 0;
	int len = 0;
	
	len = 0;
	buff[len+0] =  (para->obj_id>>0)&0xff;
	buff[len+1] =  (para->obj_id>>8)&0xff;

	len = len + 2;
	buff[len+0] =  (para->obj_type>>0)&0xff;
	buff[len+1] =  (para->obj_type>>8)&0xff;

	len = len + 2;
	memcpy(&buff[len+0],para->obj_name,MAX_OBJ_NAME_LEN);
	
	len = len + MAX_OBJ_NAME_LEN; 
	buff[len+0] =  (para->obj_x>>0)&0xff;
	buff[len+1] =  (para->obj_x>>8)&0xff;
	buff[len+2] =  (para->obj_y>>0)&0xff;
	buff[len+3] =  (para->obj_y>>8)&0xff;
	buff[len+4] =  (para->obj_w>>0)&0xff;
	buff[len+5] =  (para->obj_w>>8)&0xff;
	buff[len+6] =  (para->obj_h>>0)&0xff;
	buff[len+7] =  (para->obj_h>>8)&0xff;

	len = len + 8; 
	buff[len+0]  =  para->obj_opa;
	buff[len+1]  =  para->obj_r;
	buff[len+2]  =  para->obj_g;
	buff[len+3]  =  para->obj_b;
	buff[len+4]  =  para->obj_opa_2;
	buff[len+5]  =  para->obj_r_2;
	buff[len+6]  =  para->obj_g_2;
	buff[len+7]  =  para->obj_b_2;
	buff[len+8]  =  para->obj_font_size;
	buff[len+9]  =  para->obj_font_r;
	buff[len+10] =  para->obj_font_g;
	buff[len+11] =  para->obj_font_b;


	len = len + 12;
	buff[len+0] =  (para->obj_range_min>>0)&0xff;
	buff[len+1] =  (para->obj_range_min>>8)&0xff;

	len = len + 2;
	buff[len+0] =  (para->obj_range_max>>0)&0xff;
	buff[len+1] =  (para->obj_range_max>>8)&0xff;
	len = len + 2;
	buff[len+0] =  (para->obj_angle_range>>0)&0xff;
	buff[len+1] =  (para->obj_angle_range>>8)&0xff;	
	len = len + 2;
	buff[len+0] =  (para->obj_rotation>>0)&0xff;
	buff[len+1] =  (para->obj_rotation>>8)&0xff;	
	len = len + 2;
	buff[len+0] =  (para->obj_width>>0)&0xff;
	buff[len+1] =  (para->obj_width>>8)&0xff;	
	len = len + 2;
	buff[len+0] =  (para->obj_point_x_num>>0)&0xff;
	buff[len+1] =  (para->obj_point_x_num>>8)&0xff;	
	len = len + 2;
	buff[len+0] =  (para->obj_point_y_num>>0)&0xff;
	buff[len+1] =  (para->obj_point_y_num>>8)&0xff;
	len = len + 2;
	buff[len+0] =  (para->obj_time>>0)&0xff;
	buff[len+1] =  (para->obj_time>>8)&0xff;

		
	for(i=0;i<MAX_OBJ_VAR_LEN;i++)
		para->obj_var[i] = i;
	
	for(i=0;i<MAX_OBJ_VAR_LEN;i++){
		len = len + 2;
		buff[len+0]  =	(para->obj_var[i]>>0)&0xff;
		buff[len+1]  =	(para->obj_var[i]>>8)&0xff;
	}

	len = len + 2;
	buff[len+0]  =	(para->obj_event>>0)&0xff;
	buff[len+1]  =	(para->obj_event>>8)&0xff;

	len = len + 2;
	buff[len+0]  =	(para->obj_action>>0)&0xff;
	buff[len+1]  =	(para->obj_action>>8)&0xff;

	len = len + 2;
	memcpy(&buff[len+0],para->obj_data,MAX_OBJ_DATA_LEN);

	len = len + MAX_OBJ_DATA_LEN;
	memcpy(&buff[len+0],para->obj_reserve,MAX_RESERVE_LEN);

	len = len + MAX_RESERVE_LEN;

	*send_len = len;


	return 0;

}

void  hid_cmd_elem_update(void)
{
	int i = 0;
	unsigned int  page_id 	 = 0;
	unsigned int  elem_id 	 = 0;
	unsigned int  data_type  = 0;
	unsigned int  elem_data_len   = 0;

	unsigned int  var0   = 0;
	unsigned int  var1 	 = 0;
	unsigned int  var2   = 0;

   char control_buff[24];

	memset(control_buff,0,sizeof(control_buff));

	for( i=0;i<8;i++)
	{

		page_id = 0;
		elem_id = 0x011 + i ;
		
		data_type = HMI_OBJ_DATA_DEFAULT;
		
		elem_data_len = 3;
		srand(time(0));
		var0 = rand()%100;
		var1 = rand()%100;
		var2 = rand()%100;

		control_buff[0] = ((page_id>>0)&0xff);
		control_buff[1] = ((page_id>>8)&0xff);

		control_buff[2] = ((elem_id>>0)&0xff);
		control_buff[3] = ((elem_id>>8)&0xff);

		control_buff[4] = ((data_type>>0)&0xff);
		control_buff[5] = ((data_type>>8)&0xff);

		control_buff[6] = ((elem_data_len>>0)&0xff);
		control_buff[7] = ((elem_data_len>>8)&0xff);

		control_buff[8] = ((var0>>0)&0xff);
		control_buff[9] = ((var0>>8)&0xff);

		control_buff[10] = ((var1>>0)&0xff);
		control_buff[11] = ((var1>>8)&0xff);

		control_buff[12] = ((var2>>0)&0xff);
		control_buff[13] = ((var2>>8)&0xff);	

		hid_io_control(CMD_ELEM_UPDATE, " ", control_buff, sizeof(control_buff));

	}

}

void  hid_cmd_elem_update_buff(void)
{
	int i = 0;
	unsigned int  page_id 	 = 0;
	unsigned int  elem_id 	 = 0;
	unsigned int  data_type  = 0;
	unsigned int  elem_data_len   = 0;

	unsigned int  var0   = 0;
	unsigned int  var1 	 = 0;
	unsigned int  var2   = 0;
	unsigned int  var3   = 0;

	char control_buff[24];

	memset(control_buff,0,sizeof(control_buff));

	srand(time(0));
	var0 = rand()%100;
	var1 = rand()%100;
	var2 = rand()%100;
	var3 = rand()%100;

	printf("var0=%d\n",var0);
	printf("var1=%d\n",var1);
	printf("var2=%d\n",var2);
	printf("var3=%d\n",var3);


	for( i=0;i<4;i++)
	{

		page_id = 0;
		elem_id = 0x07 + i ;
		
		data_type = HMI_OBJ_DATA_BUFF;
		
		elem_data_len = 3;

		control_buff[0] = ((page_id>>0)&0xff);
		control_buff[1] = ((page_id>>8)&0xff);

		control_buff[2] = ((elem_id>>0)&0xff);
		control_buff[3] = ((elem_id>>8)&0xff);

		control_buff[4] = ((data_type>>0)&0xff);
		control_buff[5] = ((data_type>>8)&0xff);

		control_buff[6] = ((elem_data_len>>0)&0xff);
		control_buff[7] = ((elem_data_len>>8)&0xff);

		switch(i){
		case 0:{
			sprintf(control_buff+8,"内存:%d%",var0);
			break;
		}
		case 1:{
			sprintf(control_buff+8,"CPU:%d%",var1);
			break;
		}
		case 2:{
			sprintf(control_buff+8,"NET:%d%",var2);
			break;
		}
		case 3:{
			sprintf(control_buff+8,"磁盘:%d%",var3);	
			break;
		}
		default:break;
		}

		hid_io_control(CMD_ELEM_UPDATE, " ", control_buff, sizeof(control_buff));

	}


	for( i=0;i<4;i++)
	{

		page_id = 0;
		elem_id = 0x07 + i ;
		data_type = HMI_OBJ_DATA_DEFAULT;
		elem_data_len = 3;


		control_buff[0] = ((page_id>>0)&0xff);
		control_buff[1] = ((page_id>>8)&0xff);

		control_buff[2] = ((elem_id>>0)&0xff);
		control_buff[3] = ((elem_id>>8)&0xff);

		control_buff[4] = ((data_type>>0)&0xff);
		control_buff[5] = ((data_type>>8)&0xff);

		control_buff[6] = ((elem_data_len>>0)&0xff);
		control_buff[7] = ((elem_data_len>>8)&0xff);


		switch(i){
		case 0:{
			control_buff[8] = ((var0>>0)&0xff);
			control_buff[9] = ((var0>>8)&0xff);
			
			control_buff[10] = ((var0>>0)&0xff);
			control_buff[11] = ((var0>>8)&0xff);
			
			control_buff[12] = ((var0>>0)&0xff);
			control_buff[13] = ((var0>>8)&0xff);
			break;
		}
		case 1:{
			control_buff[8] = ((var1>>0)&0xff);
			control_buff[9] = ((var1>>8)&0xff);
			
			control_buff[10] = ((var1>>0)&0xff);
			control_buff[11] = ((var1>>8)&0xff);
			
			control_buff[12] = ((var1>>0)&0xff);
			control_buff[13] = ((var1>>8)&0xff);
			break;
		}
		case 2:{
			control_buff[8] = ((var2>>0)&0xff);
			control_buff[9] = ((var2>>8)&0xff);
			
			control_buff[10] = ((var2>>0)&0xff);
			control_buff[11] = ((var2>>8)&0xff);
			
			control_buff[12] = ((var2>>0)&0xff);
			control_buff[13] = ((var2>>8)&0xff);
			break;
		}
		case 3:{
			control_buff[8] = ((var3>>0)&0xff);
			control_buff[9] = ((var3>>8)&0xff);
			
			control_buff[10] = ((var3>>0)&0xff);
			control_buff[11] = ((var3>>8)&0xff);
			
			control_buff[12] = ((var3>>0)&0xff);
			control_buff[13] = ((var3>>8)&0xff);
			break;
		}
		default:break;
		}

		hid_io_control(CMD_ELEM_UPDATE, " ", control_buff, sizeof(control_buff));

	}


}

int hmi_update_firmware(void)
{
	int i = 0;
	unsigned int len = 0;
	obj_attr_t para ;
	
	char control_buff[1024] = {0};

	memset(&para,0,sizeof(obj_attr_t));
	memset(control_buff,0,sizeof(control_buff));

	hmi_para_to_buff(&para,control_buff,&len);
	hid_io_control(CMD_SOFT_UPDATE, " ", control_buff,len );

	return 0;
}

int hmi_create_label(void)
{
	int i = 0;
	unsigned int len = 0;
	obj_attr_t para ;
	
	char control_buff[1024] = {0};

	memset(&para,0,sizeof(obj_attr_t));
	memset(control_buff,0,sizeof(control_buff));

#if 1
	para.obj_id   = 0x01;
	para.obj_type = HMI_OBJ_TYPE_LABEL;
	
	para.obj_x	  = 0;
	para.obj_y	  = 25;
	para.obj_w	  = 192;
	para.obj_h	  = 20;
	
	para.obj_opa	= 0;
	para.obj_r		= 0;
	para.obj_g		= 0;
	para.obj_b		= 255;

	para.obj_font_size  = 0;
	para.obj_font_r		= 255;
	para.obj_font_g		= 0;
	para.obj_font_b		= 0;	
	

	para.obj_time      = 200;

	para.obj_range_min	= 0;
	para.obj_range_max	= 100;
	
	para.obj_event		= HMI_OBJ_EVENT_NETTIME_DAY;
	para.obj_action 	= 0;

	hmi_para_to_buff(&para,control_buff,&len);
	printf("len=%d.\n",len);
	hid_io_control(CMD_SHOW_TXT, " ", control_buff,len );
#endif


#if 1

	para.obj_id = 0x02;
	para.obj_type = HMI_OBJ_TYPE_LABEL;

	para.obj_x	= 0;
	para.obj_y	= 25*2+20*1;
	para.obj_w	= 192;
	para.obj_h	= 20;

	para.obj_opa	= 0;
	para.obj_r		= 255;
	para.obj_g		= 0;
	para.obj_b		= 0;

	para.obj_font_size	= 0;
	para.obj_font_r 	= 0;
	para.obj_font_g 	= 0;
	para.obj_font_b 	= 255;	

	para.obj_time = 200;

	para.obj_range_min	= 0;
	para.obj_range_max	= 100;

	para.obj_event		= HMI_OBJ_EVENT_NETTIME_HOUR;
	para.obj_action 	= 0;
	hmi_para_to_buff(&para,control_buff,&len);
	printf("len=%d.\n",len);
	hid_io_control(CMD_SHOW_TXT, " ", control_buff,len );

#endif

#if 1
	memset(&para,0,sizeof(obj_attr_t));
	para.obj_id = 0x03;
	para.obj_type = HMI_OBJ_TYPE_LABEL;

	para.obj_x	= 0;
	para.obj_y	= 25*3+20*2;
	para.obj_w	= 192;
	para.obj_h	= 20;

	para.obj_opa	= 0;
	para.obj_r		= 0;
	para.obj_g		= 255;
	para.obj_b		= 0;

	para.obj_font_size	= 0;
	para.obj_font_r 	= 255;
	para.obj_font_g 	= 0;
	para.obj_font_b 	= 0;	

	para.obj_time = 200;

	para.obj_range_min	= 0;
	para.obj_range_max	= 100;

	para.obj_event		= HMI_OBJ_EVENT_SYSTICK;
	para.obj_action 	= 0;

	hmi_para_to_buff(&para,control_buff,&len);
	printf("len=%d.\n",len);
	hid_io_control(CMD_SHOW_TXT, " ", control_buff,len );
#endif


#if 1
	
	for(i=0;i<4;i++)
	{
		memset(&para,0,sizeof(obj_attr_t));
		
		para.obj_id = 0x07+i;
		para.obj_type = HMI_OBJ_TYPE_PROGRESS_BAR;
		
		para.obj_x	= 0;
		para.obj_y	= 170+40*i;
		para.obj_w	= 192;
		para.obj_h	= 20;
		
		para.obj_opa	= 255;
		para.obj_r		= 255;
		para.obj_g		= 0;
		para.obj_b		= 0;
		
		para.obj_opa_2	= 255;
		para.obj_r_2	= 0;
		para.obj_g_2	= 0;
		para.obj_b_2	= 255;	

		para.obj_font_size	= 0;
		para.obj_font_r = 0;
		para.obj_font_g = 255;
		para.obj_font_b = 0;		

		para.obj_var[0] = 50;
		para.obj_var[1] = 60;
		para.obj_var[2] = 70;
		
		para.obj_range_min	= 0;
		para.obj_range_max	= 100;
		
		para.obj_angle_range = 360;
		para.obj_rotation  = 90;
		para.obj_width	= 20;
		
		para.obj_point_x_num = 0;
		para.obj_point_y_num = 0;
		
		para.obj_time = 1000;
		
		para.obj_event		= 0;
		para.obj_action 	= 0;

		switch(i){
		case 0:{
			sprintf((para.obj_data), "abcdefg:%d",para.obj_var[0]);
			break;
		}
		case 1:{
			sprintf((para.obj_data), "hijklmn:%d",para.obj_var[0]);
			break;
		}
		case 2:{
			sprintf((para.obj_data), "%s","网络正常");
			break;
		}
		case 3:{
			sprintf((para.obj_data), "磁盘:%d%",para.obj_var[0]);
			break;
		}
		}

		
		hmi_para_to_buff(&para,control_buff,&len);	
		hid_io_control(CMD_SHOW_TXT, " ", control_buff,len );
	}
#endif


#if 1
		for(i=0;i<8;i++){
			memset(&para,0,sizeof(obj_attr_t));
	
			if(i<4)
			{
				para.obj_x	= 192 + (i*192);
				para.obj_y	= 0;
				para.obj_w	= 192;
				para.obj_h	= 160;
	
			}
			else
			{
				para.obj_x	= 192 + (i-4)*192;
				para.obj_y	= 160;
				para.obj_w	= 192;
				para.obj_h	= 160;
	
			}
			
			para.obj_id   = 0x011+i;
			para.obj_type = HMI_OBJ_TYPE_STRAIGHT;
	
	
			para.obj_opa	= 255;
			para.obj_r		= 0;
			para.obj_g		= 255;
			para.obj_b		= 0;
	
			
			para.obj_var[0] = 50;
			para.obj_var[1] = 50;
			para.obj_var[2] = 50;
			
			para.obj_range_min	= 0;
			para.obj_range_max	= 100;
			
			para.obj_angle_range = 360;
			para.obj_rotation  = 90;
			para.obj_width	= 20;
			
			para.obj_point_x_num = 1;
			para.obj_point_y_num = 5;
			
			para.obj_time = 1000;
	
			switch(i){
			case 0:{
				sprintf((para.obj_data), "%s","Bilibili");
				break;
			}
			case 1:{
				sprintf((para.obj_data), "%s","Douyin");
				break;
			}
			case 2:{
				sprintf((para.obj_data), "%s","Currency");
				break;
			}
			case 3:{
				sprintf((para.obj_data), "%s","Stock");
	
				break;
			}
			case 4:{
				sprintf((para.obj_data), "%s","Kickstarter");
	
				break;
			}
			case 5:{
				sprintf((para.obj_data), "%s","IndieGogo");
	
				break;
			}
			case 6:{
				sprintf((para.obj_data), "%s","Stock");
	
				break;
			}
			case 7:{
				sprintf((para.obj_data), "%s","Stock");
	
				break;
			}
			default:break;
			}
			
			
			
			para.obj_event		= 0;
			para.obj_action 	= 0;

			hmi_para_to_buff(&para,control_buff,&len);
			printf("len=%d.\n",len);
			hid_io_control(CMD_SHOW_TXT, " ", control_buff,len );

		}
#endif




	return 0;
}

int hmi_delete_obj(unsigned int id)
{
	int i = 0;
	unsigned int len = 0;
	obj_attr_t para ;
	
	char control_buff[1024] = {0};

	memset(&para,0,sizeof(obj_attr_t));
	memset(control_buff,0,sizeof(control_buff));

#if 1
	para.obj_id   = id;

	hmi_para_to_buff(&para,control_buff,&len);
	printf("len=%d.\n",len);
	hid_io_control(CMD_OBJ_DEL, " ", control_buff,len );
#endif




	return 0;
}

