
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

#include "hmi_core.h"


#define  RES_START_CODE        (0x0001c6d7)
#define  RES_END_CODE          (0x0002e8f9)

#define  FILE_START_CODE       (0x0003A0A1)
#define  FILE_END_CODE         (0x0004A2A3)


int hmi_packet_file(hmi_page_t *page, const char * filepath)
{
	int i = 0;
	slist_t* node;
	hmi_element_t* elem = NULL;

	char * p_buff = NULL;	

	FILE *fp_packet = NULL;
	FILE *fp_png    = NULL;
	FILE *fp_jpg	= NULL;

	unsigned int start_code 	= RES_START_CODE;
	unsigned int end_code		= RES_END_CODE;

	unsigned int file_start_code 	= FILE_START_CODE;
	unsigned int file_end_code		= FILE_END_CODE;	
	char filename[1024]    = {0};
	unsigned int  file_size         = 0;
	unsigned int  file_name_len     = 0;
 	unsigned int  obj_attr_size 	= 0;
	unsigned char obj_reserve[16]	= {0};

	unsigned int  png_elem_num  	= 0;
	unsigned int  jpg_elem_num		= 0;

	memset(obj_reserve,0,sizeof(obj_reserve));
	
	for(i=0;i<sizeof(obj_reserve);i++)
		obj_reserve[i] = i;
	
	if (page == NULL) {
		printf("page invalid page.\n");
		return -1;
	}

	if(slist_isempty(page->page_elem_head)){
		printf("list empty.\n");
		return -1;
	}

	fp_packet = fopen(filepath,"wb");
	if(!fp_packet){
		printf("file open error.\n");
		return -1;
	}
	//**********************************************************

	//1.write start code
	fwrite(&start_code,sizeof(unsigned int),1,fp_packet);
	//**********************************************************
	//2.write obj_attr_t size
	
	printf("obj_attr_t size=%d\n",sizeof(obj_attr_t));

	obj_attr_size  = sizeof(obj_attr_t);
	
	fwrite(&obj_attr_size,sizeof(unsigned int),1,fp_packet);
	//**********************************************************
	//3.write elem num
	
	printf("num=%d\n",page->page_elem_num);

	fwrite(&page->page_elem_num,sizeof(unsigned int),1,fp_packet);
	//**********************************************************
	//4.write png elem num

	png_elem_num = 0;
	slist_for_each(node, (page->page_elem_head))
	{
		elem = slist_entry(node, hmi_element_t, elem_l_tail);
		if(elem){
			if(elem->elem_attr.obj_type == HMI_OBJ_TYPE_IMG)
			{
				png_elem_num ++;
			}	
		}
	}	
	printf("png_elem_num=%d\n",png_elem_num);
	fwrite(&png_elem_num,sizeof(unsigned int),1,fp_packet);
	//**********************************************************
	//5.write jpg elem num

	jpg_elem_num = 0;
	slist_for_each(node, (page->page_elem_head))
	{
		elem = slist_entry(node, hmi_element_t, elem_l_tail);
		if(elem){
			if(elem->elem_attr.obj_type == HMI_OBJ_TYPE_BG_IMAGE)
			{
				jpg_elem_num ++;
			}	
		}
	}	
	
	printf("jpg_elem_num=%d\n",jpg_elem_num);
	fwrite(&jpg_elem_num,sizeof(unsigned int),1,fp_packet);
	//**********************************************************
	//6.write reserve data

	fwrite(obj_reserve,sizeof(obj_reserve),1,fp_packet);
	//**********************************************************
	//6.write png data 
	slist_for_each(node, (page->page_elem_head))
	{
		elem = slist_entry(node, hmi_element_t, elem_l_tail);
		if(elem){
			if(elem->elem_attr.obj_type == HMI_OBJ_TYPE_IMG)
			{
				memset(filename,0,sizeof(filename));
				printf("\nimg:filename: %s\n",filename);
				sprintf(filename, elem->elem_attr.obj_data);

				fp_png = fopen(filename,"rb");
				if(!fp_png){
					printf("file open error %s.\n",filename);
					break;
				}

				fseek(fp_png,0L,SEEK_END);
				file_size = ftell(fp_png);
				fseek(fp_png,0L,SEEK_SET);

				p_buff =(char *) malloc(file_size);
				if(!p_buff){
					printf("malloc buff error %s\n");
					if(fp_png){
						fclose(fp_png);
						fp_png = NULL;
					}
					break;
				}

				fread(p_buff,file_size,1,fp_png);
				if(fp_png){
					fclose(fp_png);
					fp_png = NULL;
				}	
				
				//****************************************************************

				file_name_len = strlen(elem->elem_attr.obj_data);

				//****************************************************************
				//7.file start code
				fwrite(&file_start_code,          sizeof(unsigned int),1,fp_packet);
				fwrite(&elem->elem_attr.obj_id,   sizeof(unsigned int),1,fp_packet);

				fwrite(&file_name_len,		      sizeof(unsigned int),1,fp_packet);
				fwrite(&elem->elem_attr.obj_data, file_name_len,       1,fp_packet);

				fwrite(&file_size,                sizeof(unsigned int),1,fp_packet);
				fwrite(p_buff,                    file_size,           1,fp_packet);
				
				fwrite(&file_end_code,            sizeof(unsigned int),1,fp_packet);
				//****************************************************************
				free(p_buff);
				p_buff = NULL;
				//****************************************************************

			}
		}
	}
	//**********************************************************
	//7.write jpg data 
	slist_for_each(node, (page->page_elem_head))
	{
		elem = slist_entry(node, hmi_element_t, elem_l_tail);
		if(elem){
			if(elem->elem_attr.obj_type == HMI_OBJ_TYPE_BG_IMAGE)
			{
				memset(filename,0,sizeof(filename));

				sprintf(filename, elem->elem_attr.obj_data);
				printf("filename: %s\n",filename);
				fp_jpg = fopen(filename,"rb");
				if(!fp_jpg){
					printf("file open error %s.\n",filename);
					break;;
				}

				fseek(fp_jpg,0L,SEEK_END);
				file_size = ftell(fp_jpg);
				fseek(fp_jpg,0L,SEEK_SET);

				p_buff = (char*)malloc(file_size);
				if(!p_buff){
					printf("malloc buff error %s\n");
					if(fp_jpg){
						fclose(fp_jpg);
						fp_jpg = NULL;
					}
					break;
				}

				fread(p_buff,file_size,1,fp_jpg);
				if(fp_jpg){
					fclose(fp_jpg);
					fp_jpg = NULL;
				}	
				//****************************************************************
				//7.file start code
				fwrite(&file_start_code,       sizeof(unsigned int),1,fp_packet);
				fwrite(&elem->elem_attr.obj_id,sizeof(unsigned int),1,fp_packet);

				fwrite(&file_name_len,		      sizeof(unsigned int),1,fp_packet);
				fwrite(&elem->elem_attr.obj_data, file_name_len,       1,fp_packet);
				
				fwrite(&file_size,             sizeof(unsigned int),1,fp_packet);
				fwrite(p_buff,                 file_size,           1,fp_packet);

				fwrite(&file_end_code,         sizeof(unsigned int),1,fp_packet);
				//****************************************************************
				free(p_buff);
				p_buff = NULL;
				//****************************************************************
			}
		}
	}
	//****************************************************************
	//7.write all obj data info
	slist_for_each(node, (page->page_elem_head))
	{
		elem = slist_entry(node, hmi_element_t, elem_l_tail);
		if(elem){
	#if 0
			printf("elem_id=%d\n",elem->elem_attr.obj_id);
			printf("elem_type=%d\n",elem->elem_attr.obj_type);
			printf("elem_x=%d\n",elem->elem_attr.obj_x);
			printf("elem_y=%d\n",elem->elem_attr.obj_y);
			printf("elem_w=%d\n",elem->elem_attr.obj_w);
			printf("elem_h=%d\n",elem->elem_attr.obj_h);

			printf("elem_opa=%d\n",elem->elem_attr.obj_opa);
			printf("elem_color_r=%d\n",elem->elem_attr.obj_r);
			printf("elem_color_g=%d\n",elem->elem_attr.obj_g);
			printf("elem_color_b=%d\n",elem->elem_attr.obj_b);
			printf("elem_action=%d\n",elem->elem_attr.obj_action);
			printf("==================================\n");
	#endif

			fwrite(&elem->elem_attr,sizeof(obj_attr_t),1,fp_packet);
			
		}
	}

	//**********************************************************

	//end.write end code
	fwrite(&end_code,sizeof(unsigned int),1,fp_packet);
	
	//**********************************************************
	
	printf("packet succeed.\n");

	if(fp_packet){
		fclose(fp_packet);
		fp_packet = NULL;
	}

	return 0;
}




int hmi_unpacket_file(const char * filepath)
{
	int i = 0;
	obj_attr_t	   elem_attr;

	FILE *fp_unpacket = NULL;
	FILE *fp_file     = NULL;

	unsigned int start_code 	= 0;
	unsigned int end_code		= 0;
	unsigned int page_elem_num	= 0;

	unsigned int  obj_attr_size 	= 0;
	unsigned char obj_reserve[16]	= {0};

	unsigned int  png_elem_num  	= 0;
	unsigned int  jpg_elem_num		= 0;
	unsigned int file_start_code 	= FILE_START_CODE;
	unsigned int file_end_code		= FILE_END_CODE;	
	unsigned int file_size          = 0;
	unsigned int file_obj_id    	= 0;	
	unsigned int  file_name_len 	= 0;
		char*    file_buff          = NULL;
		char     file_name[1024]    = {0};
		char	 file_temp[1024]	= {0};


	memset(&elem_attr,0,sizeof(obj_attr_t));
	memset(obj_reserve,0,sizeof(obj_reserve));
	
	fp_unpacket = fopen(filepath,"rb");
	if(!fp_unpacket){
		printf("file open error.\n");
		return -1;
	}
	//**********************************************************

	//1.read start code
	fread(&start_code,sizeof(unsigned int),1,fp_unpacket);
	
	printf("start_code=0x%x\n",start_code);
	if(start_code != RES_START_CODE)
	{
		printf("start code error.\n");

		if(fp_unpacket){
			fclose(fp_unpacket);
			fp_unpacket = NULL;
		}
		
		return -1;
	}
	//**********************************************************
	//2.read obj_attr_t size
	
	fread(&obj_attr_size,sizeof(unsigned int),1,fp_unpacket);
	
	printf("obj_attr_size=%d\n",obj_attr_size);
	
	if(obj_attr_size != sizeof(obj_attr_t))
	{
		printf("obj size error.\n");

		if(fp_unpacket){
			fclose(fp_unpacket);
			fp_unpacket = NULL;
		}
		
		return -1;
	}
	//**********************************************************
	//3.read elem num

	fread(&page_elem_num,sizeof(unsigned int),1,fp_unpacket);
	
	printf("page_elem_num=%d\n",page_elem_num);
	//**********************************************************
	//4.read png elem num

	fread(&png_elem_num,sizeof(unsigned int),1,fp_unpacket);
	
	printf("png_elem_num=%d\n",png_elem_num);
	//**********************************************************
	//5.read jpg elem num

	fread(&jpg_elem_num,sizeof(unsigned int),1,fp_unpacket);
	
	printf("jpg_elem_num=%d\n",jpg_elem_num);
	//**********************************************************
	//4.read reserve data

	fread(obj_reserve,sizeof(obj_reserve),1,fp_unpacket);
#if 0
	for(i=0;i<sizeof(obj_reserve);i++)
		printf("obj_reserve[%d]=0x%x\n",i,obj_reserve[i]);
#endif
	//**********************************************************
	//5.read png data.
	for(i=0;i<png_elem_num;i++)
	{
		fread(&file_start_code,sizeof(unsigned int),1,fp_unpacket);
		printf("file_start_code=0x%x\n",file_start_code);
		if(file_start_code != FILE_START_CODE)
		{
			printf("file statr code error.\n");
			break;
		}
		fread(&file_obj_id,sizeof(unsigned int),1,fp_unpacket);
		printf("file_obj_id=0x%x\n",file_obj_id);

		fread(&file_name_len,sizeof(unsigned int),1,fp_unpacket);
		printf("file_name_len=%d\n",file_name_len);

		memset(file_temp,0,sizeof(file_temp));
		fread(file_temp,file_name_len,1,fp_unpacket);
		printf("file_temp=%s\n",file_temp);

		fread(&file_size,sizeof(unsigned int),1,fp_unpacket);
		printf("file_size=%d\n",file_size);	

		file_buff = (char*)malloc(file_size);
		if(!file_buff){
			printf("file buff malloc error.\n");	
			break;
		}
		
		fread(file_buff,file_size,1,fp_unpacket);
		fread(&file_end_code,sizeof(unsigned int),1,fp_unpacket);
		printf("file_end_code=0x%x\n",file_end_code);
		if(file_end_code != FILE_END_CODE)
		{
			printf("file end code error.\n");
			break;
		}

		memset(file_name,0,sizeof(file_name));
		sprintf(file_name,"./output/%s",file_temp);
		printf("file_name=%s\n",file_name);
		fp_file  = fopen(file_name,"wb");
		if(!fp_file){
			printf("write file error %s.\n",file_name);
			break;	
		}
		fwrite(file_buff,file_size,1,fp_file);
		fclose(fp_file);
		fp_file = NULL;		
		free(file_buff);
		file_buff = NULL;
	}
	//**********************************************************
	//6.read jpg data.
	for(i=0;i<jpg_elem_num;i++)
	{
		fread(&file_start_code,sizeof(unsigned int),1,fp_unpacket);
		printf("file_start_code=0x%x\n",file_start_code);
		if(file_start_code != FILE_START_CODE)
		{
			printf("file statr code error.\n");
			break;
		}
		fread(&file_obj_id,sizeof(unsigned int),1,fp_unpacket);
		printf("file_obj_id=0x%x\n",file_obj_id);

		fread(&file_name_len,sizeof(unsigned int),1,fp_unpacket);
		printf("file_name_len=%d\n",file_name_len);

		memset(file_temp,0,sizeof(file_temp));
		fread(file_temp,file_name_len,1,fp_unpacket);
		printf("file_temp=%s\n",file_temp);

		fread(&file_size,sizeof(unsigned int),1,fp_unpacket);
		printf("file_size=%d\n",file_size);	

		file_buff = (char*)malloc(file_size);
		if(!file_buff){
			printf("file buff malloc error.\n");	
			break;
		}
		
		fread(file_buff,file_size,1,fp_unpacket);
		fread(&file_end_code,sizeof(unsigned int),1,fp_unpacket);
		printf("file_end_code=0x%x\n",file_end_code);
		if(file_end_code != FILE_END_CODE)
		{
			printf("file end code error.\n");
			break;
		}

		memset(file_name,0,sizeof(file_name));
		sprintf(file_name,"./output/%s",file_temp);
		printf("file_name=%s\n",file_name);
		fp_file  = fopen(file_name,"wb");
		if(!fp_file){
			printf("write file error %s.\n",file_name);
			break;	
		}
		fwrite(file_buff,file_size,1,fp_file);
		fclose(fp_file);
		fp_file = NULL;
		free(file_buff);
		file_buff = NULL;
	}
	//**********************************************************

	for(i=0;i<page_elem_num;i++)
	{
		fread(&elem_attr,sizeof(obj_attr_t),1,fp_unpacket);
		printf("obj_id[%d]=0x%x\n",i,elem_attr.obj_id);
	}
	//**********************************************************

	//**********************************************************
	//end.write end code
	fread(&end_code,sizeof(unsigned int),1,fp_unpacket);
	printf("end_code=0x%x\n",end_code);
	if(end_code != RES_END_CODE)
	{
		printf("end code error.\n");

		if(fp_unpacket){
			fclose(fp_unpacket);
			fp_unpacket = NULL;
		}
		
		return -1;
	}
	//**********************************************************
	
	printf("unpacket succeed.\n");

	if(fp_unpacket){
		fclose(fp_unpacket);
		fp_unpacket = NULL;
	}

	return 0;
}










