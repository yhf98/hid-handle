

#ifndef  __HMI_PACKET_H__
#define  __HMI_PACKET_H__

int hmi_packet_file(hmi_page_t *page, const char * rootpath);
int hmi_unpacket_file(const char * filepath, const char * out_path);

#endif



