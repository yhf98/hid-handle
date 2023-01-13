#ifndef HIDHANDLE_H
#define HIDHANDLE_H

unsigned char crc8_calc(unsigned char * pdata, unsigned int len);

int hid_io_control(unsigned int cmd,const char *file_name, unsigned char *reserve,unsigned int rese_len);

int hid_write_buff(unsigned char *buff,const unsigned int buff_len ,unsigned int file_type);

int hid_write_file(const char *full_path, const char *file_name, unsigned int file_type);

int init();

int hid_write_file_handle(const char *full_path, const char *file_name, unsigned int file_type);

int hid_io_control_handle(unsigned int cmd,const char *file_name, unsigned char *reserve,unsigned int rese_len);

int hid_write_buff_handle(unsigned char *buff, const unsigned int buff_len , unsigned int file_type);

#endif