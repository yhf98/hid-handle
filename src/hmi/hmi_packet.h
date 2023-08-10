/*
 * @Author: 姚恒锋 1921934563@qq.com
 * @Date: 2023-07-25 16:58:04
 * @LastEditors: 姚恒锋 1921934563@qq.com
 * @LastEditTime: 2023-08-10 10:05:03
 * @FilePath: \hid-handle\src\hmi\hmi_packet.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */


#ifndef  __HMI_PACKET_H__
#define  __HMI_PACKET_H__






int hmi_packet_file(hmi_page_t *page, const char * rootpath);
int hmi_unpacket_file(const char * filepath, const char * out_path);


#endif




