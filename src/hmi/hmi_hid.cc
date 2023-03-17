
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

#include "hmi_hid.h"



unsigned char  writebuff[8192]  = { 0x02,0x12,0x23,0x34,0x56};
unsigned char  readbuff[2048]   = { 0x00,0x00,0,0,0 };

//**************************************************************************************************************

static const unsigned char crc8Table[256] =
{
	0x00, 0x07, 0x0E, 0x09, 0x1C, 0x1B, 0x12, 0x15, 0x38, 0x3F, 0x36, 0x31, 0x24, 0x23, 0x2A, 0x2D,
	0x70, 0x77, 0x7E, 0x79, 0x6C, 0x6B, 0x62, 0x65, 0x48, 0x4F, 0x46, 0x41, 0x54, 0x53, 0x5A, 0x5D,
	0xE0, 0xE7, 0xEE, 0xE9, 0xFC, 0xFB, 0xF2, 0xF5, 0xD8, 0xDF, 0xD6, 0xD1, 0xC4, 0xC3, 0xCA, 0xCD,
	0x90, 0x97, 0x9E, 0x99, 0x8C, 0x8B, 0x82, 0x85, 0xA8, 0xAF, 0xA6, 0xA1, 0xB4, 0xB3, 0xBA, 0xBD,
	0xC7, 0xC0, 0xC9, 0xCE, 0xDB, 0xDC, 0xD5, 0xD2, 0xFF, 0xF8, 0xF1, 0xF6, 0xE3, 0xE4, 0xED, 0xEA,
	0xB7, 0xB0, 0xB9, 0xBE, 0xAB, 0xAC, 0xA5, 0xA2, 0x8F, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9D, 0x9A,
	0x27, 0x20, 0x29, 0x2E, 0x3B, 0x3C, 0x35, 0x32, 0x1F, 0x18, 0x11, 0x16, 0x03, 0x04, 0x0D, 0x0A,
	0x57, 0x50, 0x59, 0x5E, 0x4B, 0x4C, 0x45, 0x42, 0x6F, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7D, 0x7A,
	0x89, 0x8E, 0x87, 0x80, 0x95, 0x92, 0x9B, 0x9C, 0xB1, 0xB6, 0xBF, 0xB8, 0xAD, 0xAA, 0xA3, 0xA4,
	0xF9, 0xFE, 0xF7, 0xF0, 0xE5, 0xE2, 0xEB, 0xEC, 0xC1, 0xC6, 0xCF, 0xC8, 0xDD, 0xDA, 0xD3, 0xD4,
	0x69, 0x6E, 0x67, 0x60, 0x75, 0x72, 0x7B, 0x7C, 0x51, 0x56, 0x5F, 0x58, 0x4D, 0x4A, 0x43, 0x44,
	0x19, 0x1E, 0x17, 0x10, 0x05, 0x02, 0x0B, 0x0C, 0x21, 0x26, 0x2F, 0x28, 0x3D, 0x3A, 0x33, 0x34,
	0x4E, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5C, 0x5B, 0x76, 0x71, 0x78, 0x7F, 0x6A, 0x6D, 0x64, 0x63,
	0x3E, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2C, 0x2B, 0x06, 0x01, 0x08, 0x0F, 0x1A, 0x1D, 0x14, 0x13,
	0xAE, 0xA9, 0xA0, 0xA7, 0xB2, 0xB5, 0xBC, 0xBB, 0x96, 0x91, 0x98, 0x9F, 0x8A, 0x8D, 0x84, 0x83,
	0xDE, 0xD9, 0xD0, 0xD7, 0xC2, 0xC5, 0xCC, 0xCB, 0xE6, 0xE1, 0xE8, 0xEF, 0xFA, 0xFD, 0xF4, 0xF3
};




unsigned char crc8_calc(unsigned char * pdata, unsigned int len)
{
	unsigned char   crc8 = 0xff;

	while(len --)
	{
		crc8 = crc8 ^ (*pdata);
		crc8 = crc8Table[crc8];
		pdata++;
	}

	return crc8;
}

int hid_io_control(hid_device* hid_handle ,unsigned int cmd,const char *file_name,  char *control_buff,unsigned int control_buff_len)
{
	int ret = -1;
	
	unsigned int file_len = 0;

	tran_packet_t m_packet;
	ack_packet_t  a_packet;

	unsigned long long time_s;
	unsigned long long time_e;

	printf("rese_len=%d.\n",control_buff_len);

	if(control_buff_len < 16){
		printf("para error.\n");
		return SYS_PARA_ERROR;
	}
	
	if(control_buff_len > READ_DATA_LEN){
		printf("para error.\n");
		return SYS_PARA_ERROR;
	}	

	if(!hid_handle){
		printf("hid device error.\n");
		return SYS_DEVICE_ERROR;
	}

	memset((unsigned char*)(&m_packet), 0, sizeof(tran_packet_t));
	memset((unsigned char*)(&a_packet), 0, sizeof(ack_packet_t));

	file_len = strlen(file_name);
	printf("file_len=%d\n",file_len);
	printf("file_name=%s\n",file_name);


	time_s = GetTickCount64();				

	m_packet.hid_ep_id    = HID_END_POINT;         //hid-endpoint
	m_packet.start_byte   = TRAN_END_FRAME_HEDA; //start frame head byte
	m_packet.tran_type    = TRAN_TYPE_CMD;         //trans data type
	m_packet.err_code     = 0;
	m_packet.tran_index   = 0;  
	m_packet.end_byte     = TRAN_END_FRAME_TAIL;
	//****************************************************************************
	//cmd type
	m_packet.tran_data[0] = cmd;

	//control para len and para.
	m_packet.tran_data[1] = ((control_buff_len&0xff000000)>>24);
	m_packet.tran_data[2] = ((control_buff_len&0x00ff0000)>>16);
	m_packet.tran_data[3] = ((control_buff_len&0x0000ff00)>>8);
	m_packet.tran_data[4] = ((control_buff_len&0x000000ff)>>0);
	memcpy((unsigned char*)(&m_packet.tran_data[5]),control_buff,control_buff_len);
	//****************************************************************************

	//file name len and file name.
	m_packet.tran_data[5+control_buff_len] = ((file_len&0xff000000)>>24);
	m_packet.tran_data[6+control_buff_len] = ((file_len&0x00ff0000)>>16);
	m_packet.tran_data[7+control_buff_len] = ((file_len&0x0000ff00)>>8);
	m_packet.tran_data[8+control_buff_len] = ((file_len&0x000000ff)>>0);	
	memcpy((unsigned char*)(&m_packet.tran_data[9+control_buff_len]),file_name,file_len);
	//****************************************************************************

	
	//****************************************************************************
	//crc calc

	m_packet.tran_data_len = control_buff_len + file_len + 4 + 4 + 1;
	m_packet.crc8 = crc8_calc((unsigned char*)(&m_packet), CRC_CALC_LEN);
	//****************************************************************************

	a_packet.err_code  = TRAN_ERR_NUKNOWN;
	while(a_packet.err_code){
		ret = hid_write(hid_handle, (unsigned char*)(&m_packet), HID_WRITE_BUFF_LEN);
		if(ret == -1){
			printf("device error.\n");
			return SYS_DEVICE_ERROR;
		}
	
		hid_read_timeout(hid_handle, (unsigned char*)(&a_packet), HID_READ_BUFF_LEN,100);
		switch(a_packet.err_code){
		case TRAN_NO_ERR:{
	
			break;
		}
		case TRAN_ERR_CRC:{
	
			break;
		}		
		case TRAN_ERR_HEAD:{
	
			break;
		}		
		case TRAN_ERR_NUKNOWN:{
	
			break;
		}	
		case TRAN_ERR_TIMEOUT:{
			return SYS_FAILED;
		}
		default:break;
		}
		printf("tran_index=%d,data_len=%d,\n", m_packet.tran_index,m_packet.tran_data_len);
	}
	
    time_e = GetTickCount64();	
	
	printf("file trans succeed,time=%lld ms.\n",time_e - time_s);

	return SYS_SUCCEED;

}


int hid_write_buff(  hid_device* hid_handle ,unsigned char *buff,const unsigned int buff_len ,unsigned int file_type)
{
	int ret = -1;

	unsigned char *src_buff = NULL;
	
	unsigned int  send_len = 0;

	tran_packet_t m_packet;
	ack_packet_t  a_packet;

	unsigned long long time_s;
	unsigned long long time_e;

	if(!hid_handle){
		printf("hid device error.\n");
		return SYS_DEVICE_ERROR;
	}

	src_buff = buff;
	send_len = buff_len;

	memset((unsigned char*)(&m_packet), 0, sizeof(tran_packet_t));
	memset((unsigned char*)(&a_packet), 0, sizeof(ack_packet_t));


	time_s = GetTickCount64();				

	m_packet.hid_ep_id    = HID_END_POINT;         //hid-endpoint
	m_packet.start_byte   = TRAN_START_FRAME_HEDA; //start frame head byte
	m_packet.tran_type    = file_type;         //trans data type
	m_packet.err_code     = 0;
	m_packet.tran_index   = 0;  
	m_packet.end_byte     = TRAN_START_FRAME_TAIL;

	while(send_len)
	{
		if(send_len >= READ_DATA_LEN)
		{
			memcpy((unsigned char*)(&m_packet.tran_data),src_buff,READ_DATA_LEN);
			send_len = send_len - READ_DATA_LEN;
			src_buff = src_buff + READ_DATA_LEN;

			if(send_len ==  0){
				m_packet.start_byte   = TRAN_END_FRAME_HEDA; 
				m_packet.end_byte	  = TRAN_END_FRAME_TAIL;
			}
			else{
				m_packet.start_byte   = TRAN_START_FRAME_HEDA;
				m_packet.end_byte	  = TRAN_START_FRAME_TAIL;
			}

			m_packet.tran_data_len = READ_DATA_LEN;
			m_packet.tran_index ++; 
			m_packet.crc8 = crc8_calc((unsigned char*)(&m_packet), CRC_CALC_LEN);

			a_packet.err_code  = TRAN_ERR_NUKNOWN;
			while(a_packet.err_code){
				ret = hid_write(hid_handle, (unsigned char*)(&m_packet), HID_WRITE_BUFF_LEN);
				if(ret == -1){
					printf("device error.\n");
					return SYS_DEVICE_ERROR;
				}

				hid_read_timeout(hid_handle, (unsigned char*)(&a_packet), HID_READ_BUFF_LEN,100);
				switch(a_packet.err_code){
				case TRAN_NO_ERR:{
			
					break;
				}
				case TRAN_ERR_CRC:{
			
					break;
				}		
				case TRAN_ERR_HEAD:{
			
					break;
				}		
				case TRAN_ERR_NUKNOWN:{
			
					break;
				}	
				case TRAN_ERR_TIMEOUT:{
					return SYS_FAILED;
				}
				default:break;
				}
				printf("tran_index=%d,data_len=%d,\n", m_packet.tran_index,m_packet.tran_data_len);
			}			
		}
		else
		{
			memcpy((unsigned char*)(&m_packet.tran_data),src_buff,send_len);

			m_packet.tran_data_len = send_len;
			m_packet.start_byte    = TRAN_END_FRAME_HEDA; //start frame head byte
			m_packet.end_byte	   = TRAN_END_FRAME_TAIL; //start frame head byte
			m_packet.tran_index++;
			m_packet.crc8 = crc8_calc((unsigned char*)(&m_packet), CRC_CALC_LEN);
			
			a_packet.err_code  = TRAN_ERR_NUKNOWN;
			while(a_packet.err_code){
				ret = hid_write(hid_handle, (unsigned char*)(&m_packet), HID_WRITE_BUFF_LEN);
				if(ret == -1){
					printf("device error.\n");
					return SYS_DEVICE_ERROR;
				}
				hid_read_timeout(hid_handle, (unsigned char*)(&a_packet), HID_READ_BUFF_LEN,100);
				switch(a_packet.err_code){
				case TRAN_NO_ERR:{
			
					break;
				}
				case TRAN_ERR_CRC:{
			
					break;
				}		
				case TRAN_ERR_HEAD:{
			
					break;
				}		
				case TRAN_ERR_NUKNOWN:{
			
					break;
				}	
				case TRAN_ERR_TIMEOUT:{
					return SYS_FAILED;
				}
				default:break;
				}

				hid_error(hid_handle);
				printf("tran_index=%d,data_len=%d,\n", m_packet.tran_index,m_packet.tran_data_len);
			}

			send_len = 0;
		}
	}

    time_e = GetTickCount64();	
	
	printf("file trans succeed,time=%lld ms.\n",time_e - time_s);

	return SYS_SUCCEED;

}


int hid_write_file(hid_device* hid_handle ,const char *full_path,const char *file_name,unsigned int file_type)
{
	int ret = -1;
	FILE * fp_png = NULL;
	unsigned int  name_len  = 0;
	unsigned int  file_size = 0;
	unsigned int  send_size = 0;

	tran_packet_t m_packet;
	ack_packet_t  a_packet;

	unsigned long long time_s;
	unsigned long long time_e;

	unsigned long long cnt = 0;

	if(!hid_handle){
		printf("hid device error.\n");
		return SYS_DEVICE_ERROR;
	}

	memset((unsigned char*)(&m_packet), 0, sizeof(tran_packet_t));
	memset((unsigned char*)(&a_packet), 0, sizeof(ack_packet_t));

	fp_png = fopen(full_path,"rb");
	if(fp_png == NULL){
		printf("no file,%s\n",full_path);		
		return SYS_NO_FILE;
	}

	fseek(fp_png,0,SEEK_END);
	file_size = ftell(fp_png);
	fseek(fp_png,0,SEEK_SET);


	time_s = GetTickCount64();				

	name_len = strlen(file_name);
	
	printf("name_len=%d,filename=%s\n",name_len, file_name);

	m_packet.hid_ep_id    = HID_END_POINT;         //hid-endpoint
	m_packet.start_byte   = TRAN_START_FRAME_HEDA; //start frame head byte
	m_packet.tran_type    = file_type;         //trans data type
	m_packet.err_code     = 0;
	m_packet.tran_index   = 0;  
	m_packet.tran_data_len = name_len;
	
	memcpy((unsigned char*)(&m_packet.tran_data),file_name,name_len);

	m_packet.crc8 = crc8_calc((unsigned char*)(&m_packet),CRC_CALC_LEN);
	printf("crc8=0x%x\n",m_packet.crc8);

	m_packet.end_byte  = TRAN_START_FRAME_TAIL;

	a_packet.err_code  = TRAN_ERR_NUKNOWN;
	while(a_packet.err_code){
		ret = hid_write(hid_handle, (unsigned char*)(&m_packet), HID_WRITE_BUFF_LEN);
		if(ret == -1){
			printf("device error.\n");
			fclose(fp_png);
			fp_png = NULL;
			return SYS_DEVICE_ERROR;
		}
		hid_read(hid_handle, (unsigned char*)(&a_packet), HID_READ_BUFF_LEN);
	    switch(a_packet.err_code){
		case TRAN_NO_ERR:{

			break;
		}
		case TRAN_ERR_CRC:{

			break;
		}		
		case TRAN_ERR_HEAD:{

			break;
		}		
		case TRAN_ERR_NUKNOWN:{

			break;
		}	
		case TRAN_ERR_TIMEOUT:{
			fclose(fp_png);
			fp_png = NULL;
			return SYS_FAILED;
		}
		default:break;
		}
		printf("tran_index=%d,data_len=%d,\n", m_packet.tran_index,m_packet.tran_data_len);
	}

	while(!feof(fp_png))
	{
		ret = fread((unsigned char*)(&m_packet.tran_data),1,READ_DATA_LEN,fp_png);

		send_size = send_size + ret;

		if(ret == READ_DATA_LEN) 
		{
			if(send_size ==  file_size){
				m_packet.start_byte   = TRAN_END_FRAME_HEDA; 
				m_packet.end_byte	  = TRAN_END_FRAME_TAIL;
			}
			else{
				m_packet.start_byte   = TRAN_START_FRAME_HEDA;
				m_packet.end_byte	  = TRAN_START_FRAME_TAIL;
			}		

			m_packet.tran_data_len = READ_DATA_LEN;
			m_packet.tran_index ++; 
			m_packet.crc8 = crc8_calc((unsigned char*)(&m_packet), CRC_CALC_LEN);

			a_packet.err_code  = TRAN_ERR_NUKNOWN;
			while(a_packet.err_code){
				ret = hid_write(hid_handle, (unsigned char*)(&m_packet), HID_WRITE_BUFF_LEN);
				if(ret == -1){
					printf("device error.\n");
					fclose(fp_png);
					fp_png = NULL;
					return SYS_DEVICE_ERROR;
				}
				hid_read_timeout(hid_handle, (unsigned char*)(&a_packet), HID_READ_BUFF_LEN,100);
				switch(a_packet.err_code){
				case TRAN_NO_ERR:{
			
					break;
				}
				case TRAN_ERR_CRC:{
			
					break;
				}		
				case TRAN_ERR_HEAD:{
			
					break;
				}		
				case TRAN_ERR_NUKNOWN:{
			
					break;
				}	
				case TRAN_ERR_TIMEOUT:{
					fclose(fp_png);
					fp_png = NULL;
					return SYS_FAILED;
				}
				default:break;
				}
				printf("tran_index=%d,data_len=%d,\n", m_packet.tran_index,m_packet.tran_data_len);
			}
		}
		else {
			m_packet.tran_data_len = ret;
			m_packet.start_byte = TRAN_END_FRAME_HEDA; //start frame head byte
			m_packet.end_byte   = TRAN_END_FRAME_TAIL; //start frame head byte
			m_packet.tran_index++;
			m_packet.crc8 = crc8_calc((unsigned char*)(&m_packet), CRC_CALC_LEN);

			a_packet.err_code  = TRAN_ERR_NUKNOWN;
			while(a_packet.err_code){
				ret = hid_write(hid_handle, (unsigned char*)(&m_packet), HID_WRITE_BUFF_LEN);
				if(ret == -1){
					printf("device error.\n");
					fclose(fp_png);
					fp_png = NULL;
					return SYS_DEVICE_ERROR;
				}
				hid_read_timeout(hid_handle, (unsigned char*)(&a_packet), HID_READ_BUFF_LEN,100);
				switch(a_packet.err_code){
				case TRAN_NO_ERR:{
			
					break;
				}
				case TRAN_ERR_CRC:{
			
					break;
				}		
				case TRAN_ERR_HEAD:{
			
					break;
				}		
				case TRAN_ERR_NUKNOWN:{
			
					break;
				}	
				case TRAN_ERR_TIMEOUT:{
					fclose(fp_png);
					fp_png = NULL;
					return SYS_FAILED;
				}
				default:break;
				}
				printf("tran_index=%d,data_len=%d,\n", m_packet.tran_index,m_packet.tran_data_len);
			}
		}
	}

	fclose(fp_png);
	fp_png = NULL;

    time_e = GetTickCount64();	
	
	printf("file trans succeed,time=%lld ms.\n",time_e - time_s);

	return SYS_SUCCEED;

}


int hmi_send_wifi_info(hid_device* hid_handle, const char* wifiname, const char* wifipasswd)
{
	int wifiname_len = 0;
	int wifipwd_len = 0;

	char control_buff[256] = { 0 };

	if (!wifiname) {
		return -1;
	}

	if (!wifipasswd) {
		return -1;
	}

	wifiname_len = strlen(wifiname);
	wifipwd_len = strlen(wifipasswd);

	if (wifiname_len <= 0)
		return -1;

	if (wifipwd_len <= 0) {
		return -1;
	}

	memset(control_buff, 0, sizeof(control_buff));

	control_buff[0] = ((wifiname_len >> 0) & 0xff);
	control_buff[1] = ((wifiname_len >> 8) & 0xff);

	control_buff[2] = ((wifipwd_len >> 0) & 0xff);
	control_buff[3] = ((wifipwd_len >> 8) & 0xff);

	memcpy(&control_buff[4], wifiname, wifiname_len);
	memcpy(&control_buff[4 + wifiname_len], wifipasswd, wifipwd_len);

	hid_io_control(hid_handle, CMD_WIFI_INFO, " ", control_buff, sizeof(control_buff));

	return 0;
}





































