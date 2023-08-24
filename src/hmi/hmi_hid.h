

#ifndef  __HMI_HID_H__
#define  __HMI_HID_H__

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

#define    CMD_WIFI_INFO			     (0xAF)
#define    CMD_DELETE_FILE			     (0xB0)




#define    SYS_PARA_ERROR         	     (-4)
#define    SYS_NO_FILE        			 (-3)
#define    SYS_DEVICE_ERROR         	 (-2)
#define    SYS_FAILED         			 (-1)
#define    SYS_SUCCEED         			 (0)

#define    HID_WRITE_BUFF_LEN            (1024)
#define    HID_READ_BUFF_LEN             (512)


#define    CRC_CALC_LEN                 (1016)
#define    READ_DATA_LEN                (1000)

#define    HID_END_POINT                0x02 

#define    TRAN_START_FRAME_HEDA        0xFA
#define    TRAN_START_FRAME_TAIL        0xFB

#define    TRAN_END_FRAME_HEDA          0xFF
#define    TRAN_END_FRAME_TAIL          0xFF



#define    TRAN_TYPE_ACK				0x00
#define    TRAN_TYPE_PNG				0x01
#define    TRAN_TYPE_JPG				0x02
#define    TRAN_TYPE_VIDEO				0x03
#define    TRAN_TYPE_AUDIO  			0x04
#define    TRAN_TYPE_FIRMWARE   		0x05
#define    TRAN_TYPE_TXT				0x06
#define    TRAN_TYPE_WEBSITE  			0x07
#define    TRAN_TYPE_CMD     			0x08
#define    TRAN_TYPE_RES_FILE     		0x09


#define    TRAN_NO_ERR   				0x00
#define    TRAN_ERR_CRC					0x01
#define    TRAN_ERR_HEAD				0x02
#define    TRAN_ERR_NUKNOWN  			0x03
#define    TRAN_ERR_TIMEOUT  			0x04



typedef struct tran_packet {
	unsigned char   hid_ep_id;  //1
	unsigned char   start_byte; //1
	unsigned char   tran_type;  //1
	unsigned char	err_code;   //1

	unsigned char   reserve[4];	 //4

	unsigned int    tran_index; //4
	
	unsigned int    tran_data_len;//4
	unsigned char   tran_data[READ_DATA_LEN];
	
	unsigned int    crc8;   	 //4
	unsigned char   reserve1;	 //1
	unsigned char   reserve2;	 //1
	unsigned char   reserve3;	 //1
	unsigned char   end_byte;	 //1

}tran_packet_t;


typedef struct ack_packet {
	unsigned char   hid_ep_id;  //1
	unsigned char   start_byte; //1
	unsigned char   tran_type;  //1
	unsigned char	err_code;   //1

	unsigned char   reserve[4];	 //4

	unsigned int    tran_index; //4
	
	unsigned int    tran_data_len;//4
	unsigned char   tran_data[488];
	
	unsigned int    crc8;   	 //4
	unsigned char   reserve1;	 //1
	unsigned char   reserve2;	 //1
	unsigned char   reserve3;	 //1
	unsigned char   end_byte;	 //1

}ack_packet_t;






int hid_io_control(hid_device* hid_handle ,unsigned int cmd,const char *file_name,  char *control_buff,unsigned int control_buff_len);
int hid_write_buff(  hid_device* hid_handle ,unsigned char *buff,const unsigned int buff_len ,unsigned int file_type);
int hid_write_file(hid_device* hid_handle ,const char *full_path,const char *file_name,unsigned int file_type);
int hmi_send_wifi_info(hid_device* hid_handle, const char* wifiname, const char* wifipasswd);
int hmi_update_obj_data(hid_device* hid_handle);

int hmi_del_all_file(hid_device* hid_handle);




#endif




