
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


int hmi_packet_file(hmi_page_t *page, const char *rootpath)
{
	int i = 0;
	int total = 0;
	slist_t* node;
	hmi_element_t* elem = NULL;

	char * p_buff = NULL;	

	FILE *fp_packet = NULL;
	FILE *fp_png    = NULL;
	FILE *fp_jpg	= NULL;
	FILE *fp_video	= NULL;
	FILE *fp_music	= NULL;
	FILE *fp_font	= NULL;

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

	unsigned int  video_elem_num  	= 0;
	unsigned int  music_elem_num    = 0;	
	unsigned int  font_elem_num	    = 0;
	
	unsigned int  progress_file_elem_num = 0;

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

	char hbin_path[100];
	sprintf(hbin_path, "%s/hbin/hmi_res.hbin", rootpath);

	fp_packet = fopen(hbin_path, "wb+");
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
	//6.write video elem num

	video_elem_num = 0;
	slist_for_each(node, (page->page_elem_head))
	{
		elem = slist_entry(node, hmi_element_t, elem_l_tail);
		if(elem){
			if(elem->elem_attr.obj_type == HMI_OBJ_TYPE_BG_VIDEO)
			{
				video_elem_num ++;
			}	
		}
	}	
	
	printf("video_elem_num=%d\n",video_elem_num);
	fwrite(&video_elem_num,sizeof(unsigned int),1,fp_packet);
	//**********************************************************
	//7.write music elem num

	music_elem_num = 0;
	slist_for_each(node, (page->page_elem_head))
	{
		elem = slist_entry(node, hmi_element_t, elem_l_tail);
		if(elem){
			if(elem->elem_attr.obj_type == HMI_OBJ_TYPE_BG_MUSIC)
			{
				music_elem_num ++;
			}	
		}
	}	
	
	printf("music_elem_num=%d\n",music_elem_num);
	fwrite(&music_elem_num,sizeof(unsigned int),1,fp_packet);
	//**********************************************************
	//write font num

	font_elem_num = 0;
	slist_for_each(node, (page->page_elem_head))
	{
		elem = slist_entry(node, hmi_element_t, elem_l_tail);
		if(elem){

			if(strlen(elem->elem_attr.obj_font_name) > 0)
			{
				font_elem_num ++;
			}
		}
	}	
	
	printf("font_elem_num=%d\n",font_elem_num);
	fwrite(&font_elem_num,sizeof(unsigned int),1,fp_packet);
	//**********************************************************
	//write progress png num

	progress_file_elem_num = 0;
	slist_for_each(node, (page->page_elem_head))
	{
		elem = slist_entry(node, hmi_element_t, elem_l_tail);
		if(elem){
			if(elem->elem_attr.obj_type == HMI_OBJ_TYPE_PROGRESS_PNG)
			{
				progress_file_elem_num += elem->elem_attr.obj_progress_interval;
			}
		}
	}	
	
	printf("progress_num=%d\n",progress_file_elem_num);
	fwrite(&progress_file_elem_num,sizeof(unsigned int),1,fp_packet);
	//**********************************************************
	//write reserve data

	fwrite(obj_reserve,sizeof(obj_reserve),1,fp_packet);
	//**********************************************************
	//write png data 
	
	slist_for_each(node, (page->page_elem_head))
	{
		elem = slist_entry(node, hmi_element_t, elem_l_tail);
		if(elem){

			printf("obj_type=%d\n",elem->elem_attr.obj_type);
		
			if(elem->elem_attr.obj_type == HMI_OBJ_TYPE_IMG)
			{
				memset(filename, 0, sizeof(filename));

				sprintf(filename, "%s/img/%s", rootpath, elem->elem_attr.obj_data);
				printf("\nimg2:filename: %s\n", filename);
				
				printf("filename=%s\n",filename);

				fp_png = fopen(filename,"rb");
				if(!fp_png){
					printf("file open error %s.\n",filename);
					break;;
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
				memset(filename, 0, sizeof(filename));

				sprintf(filename, "%s/img/%s", rootpath, elem->elem_attr.obj_data);
				printf("filename: %s\n", filename);
				fp_jpg = fopen(filename, "rb");

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

				file_name_len = strlen(elem->elem_attr.obj_data);

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
	//7.write video data
	slist_for_each(node, (page->page_elem_head))
	{
		elem = slist_entry(node, hmi_element_t, elem_l_tail);
		if(elem){
			if(elem->elem_attr.obj_type == HMI_OBJ_TYPE_BG_VIDEO)
			{
				memset(filename,0,sizeof(filename));

				sprintf(filename, "%s/img/%s", rootpath, elem->elem_attr.obj_data);
				printf("===videoPath==: %s \tvideoName=%s \n", filename, elem->elem_attr.obj_data);

				fp_video = fopen(filename,"rb"); 
				if(!fp_video){
					printf("file open error %s.\n",filename);
					break;;
				}

				fseek(fp_video,0L,SEEK_END);
				file_size = ftell(fp_video);
				fseek(fp_video,0L,SEEK_SET);

				p_buff = (char*)malloc(file_size);
				if(!p_buff){
					printf("malloc buff error %s\n");
					if(fp_video){
						fclose(fp_video);
						fp_video = NULL;
					}
					break;
				}

				fread(p_buff,file_size,1,fp_video);
				if(fp_video){
					fclose(fp_video);
					fp_video = NULL;
				}	
				
				//****************************************************************

				file_name_len = strlen(elem->elem_attr.obj_data);

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
	//8.write music data
	slist_for_each(node, (page->page_elem_head))
	{
		elem = slist_entry(node, hmi_element_t, elem_l_tail);
		if(elem){
			if(elem->elem_attr.obj_type == HMI_OBJ_TYPE_BG_MUSIC)
			{
				memset(filename,0,sizeof(filename));
				sprintf(filename, "%s/img/%s", rootpath, elem->elem_attr.obj_data);

				fp_music = fopen(filename,"rb");
				if(!fp_music){
					printf("file open error %s.\n",filename);
					break;;
				}

				fseek(fp_music,0L,SEEK_END);
				file_size = ftell(fp_music);
				fseek(fp_music,0L,SEEK_SET);

				p_buff = (char*)malloc(file_size);
				if(!p_buff){
					printf("malloc buff error %s\n");
					if(fp_music){
						fclose(fp_music);
						fp_music = NULL;
					}
					break;
				}

				fread(p_buff,file_size,1,fp_music);
				if(fp_music){
					fclose(fp_music);
					fp_music = NULL;
				}	
				
				//****************************************************************

				file_name_len = strlen(elem->elem_attr.obj_data);

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
#if 0
	//write font 
	slist_for_each(node, (page->page_elem_head))
	{
		elem = slist_entry(node, hmi_element_t, elem_l_tail);
		if(elem){
			if(strlen(elem->elem_attr.obj_font_name) > 0)
			{
				memset(filename,0,sizeof(filename));
				sprintf(filename, "%s/fonts/%s", rootpath, elem->elem_attr.obj_font_name);

				printf("[font]: %s\n", filename);
				printf("[fontPath]: %s\n", elem->elem_attr.obj_font_name);

				fp_font = fopen(filename,"rb");
				if(!fp_font){
					printf("file open error %s.\n",filename);
					break;;
				}

				fseek(fp_font,0L,SEEK_END);
				file_size = ftell(fp_font);
				fseek(fp_font,0L,SEEK_SET);

				p_buff = (char*)malloc(file_size);
				if(!p_buff){
					printf("malloc buff error %s\n");
					if(fp_font){
						fclose(fp_font);
						fp_font = NULL;
					}
					break;
				}

				fread(p_buff,file_size,1,fp_font);
				if(fp_font){
					fclose(fp_font);
					fp_font = NULL;
				}	
				
				//****************************************************************

				file_name_len = strlen(elem->elem_attr.obj_font_name);

				//****************************************************************
				//7.file start code
				fwrite(&file_start_code,       sizeof(unsigned int),1,fp_packet);
				fwrite(&elem->elem_attr.obj_id,sizeof(unsigned int),1,fp_packet);

				fwrite(&file_name_len,		      sizeof(unsigned int),1,fp_packet);
				fwrite(&elem->elem_attr.obj_font_name, file_name_len,       1,fp_packet);
				
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
#endif
	//****************************************************************
	//write progress png file 
	slist_for_each(node, (page->page_elem_head))
	{
		elem = slist_entry(node, hmi_element_t, elem_l_tail);
		if(elem){		
			if(elem->elem_attr.obj_type == HMI_OBJ_TYPE_PROGRESS_PNG)
			{

				if(elem->elem_attr.obj_progress_interval)
				{
					total  = elem->elem_attr.obj_progress_interval;
				}
				else
				{
					total = 0;
					printf("interval error\n");
				}

				printf("total=%d\n",total);

				//******************************************************************************
				for(i=0;i<total;i++)
				{
					if(strlen(elem->elem_attr.obj_progress_name[i]) > 0)
					{
						memset(filename,0,sizeof(filename));
						sprintf(filename, "%s/img/%s", rootpath, elem->elem_attr.obj_progress_name[i]);
						// sprintf(filename, "./res/%s", elem->elem_attr.obj_progress_name[i]);
					
						fp_font = fopen(filename,"rb");
						if(!fp_font){
							printf("file open error %s.\n",filename);
							break;;
						}
					
						fseek(fp_font,0L,SEEK_END);
						file_size = ftell(fp_font);
						fseek(fp_font,0L,SEEK_SET);
					
						p_buff = (char*)malloc(file_size);
						if(!p_buff){
							printf("malloc buff error %s\n");
							if(fp_font){
								fclose(fp_font);
								fp_font = NULL;
							}
							break;
						}
					
						fread(p_buff,file_size,1,fp_font);
						if(fp_font){
							fclose(fp_font);
							fp_font = NULL;
						}	
						
						//****************************************************************
					
						file_name_len = strlen(elem->elem_attr.obj_progress_name[i]);
					
						//****************************************************************
						//7.file start code
						fwrite(&file_start_code,	   sizeof(unsigned int),1,fp_packet);
						fwrite(&elem->elem_attr.obj_id,sizeof(unsigned int),1,fp_packet);
					
						fwrite(&file_name_len,			  sizeof(unsigned int),1,fp_packet);
						fwrite(&elem->elem_attr.obj_progress_name[i], file_name_len,		1,fp_packet);
						
						fwrite(&file_size,			   sizeof(unsigned int),1,fp_packet);
						fwrite(p_buff,				   file_size,			1,fp_packet);
					
						fwrite(&file_end_code,		   sizeof(unsigned int),1,fp_packet);
						//****************************************************************
						free(p_buff);
						p_buff = NULL;
						//****************************************************************
					}

				}
				//******************************************************************************
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
			printf("obj_attr_t size	   = %d\n", sizeof(obj_attr_t));
	#endif
			printf("\nfwrite:===: obj_data=%s \n",elem->elem_attr.obj_data);
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
	//**********************************************************

	return 0;
}




int hmi_unpacket_file(const char *filepath, const char *out_path)
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

	unsigned int  video_elem_num  	= 0;
	unsigned int  music_elem_num    = 0;	
	unsigned int  font_elem_num     = 0;	
	unsigned int  progress_file_elem_num = 0;

	
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
	//6.read video elem num

	fread(&video_elem_num,sizeof(unsigned int),1,fp_unpacket);
	
	printf("video_elem_num=%d\n",video_elem_num);
	//**********************************************************
	//7.read music elem num

	fread(&music_elem_num,sizeof(unsigned int),1,fp_unpacket);
	
	printf("music_elem_num=%d\n",music_elem_num);
	//**********************************************************

	fread(&font_elem_num,sizeof(unsigned int),1,fp_unpacket);
	
	printf("font_elem_num=%d\n",font_elem_num);
	//**********************************************************
	fread(&progress_file_elem_num,sizeof(unsigned int),1,fp_unpacket);
	
	printf("progress_file=%d\n",progress_file_elem_num);
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
		// sprintf(file_name,"./output/%s",file_temp);
		sprintf(file_name, "%s/%s", out_path, file_temp);

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
		// sprintf(file_name,"./output/%s",file_temp);
		sprintf(file_name, "%s/%s", out_path, file_temp);
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
	//7.read video data.
	for(i=0;i<video_elem_num;i++)
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
		// sprintf(file_name,"./output/%s",file_temp);
		sprintf(file_name, "%s/%s", out_path, file_temp);
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
	//8.read music data.
	for(i=0;i<music_elem_num;i++)
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
		// sprintf(file_name,"./output/%s",file_temp);
		sprintf(file_name, "%s/%s", out_path, file_temp);
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
	//.read font data.
	for(i=0;i<font_elem_num;i++)
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
		// sprintf(file_name,"./output/%s",file_temp);
		sprintf(file_name, "%s/%s", out_path, file_temp);
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
	//.read progress file data.
	for(i=0;i<progress_file_elem_num;i++)
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
		// sprintf(file_name,"./output/%s",file_temp);
		sprintf(file_name, "%s/%s", out_path, file_temp);
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
		
		printf("================================%d================================\n",i);
		printf("obj_attr_t size	   = %d\n", sizeof(obj_attr_t));
		printf("obj_id[%d]=0x%x\n",i,elem_attr.obj_id);
		printf("obj_type[%d]=0x%x\n",i,elem_attr.obj_type);
		#if 1
		printf("obj_name=%s\n",elem_attr.obj_name);		
		printf("obj_data=%s\n",elem_attr.obj_data);

		printf("obj_wifi_name=%s\n",elem_attr.obj_wifi_name);		
		printf("obj_wifi_pass=%s\n",elem_attr.obj_wifi_pass);

		
		printf("obj_url=%s\n",elem_attr.obj_url);
		printf("obj_field=%s\n",elem_attr.obj_field);

		printf("obj_x=%d\n", elem_attr.obj_x);
		printf("obj_y=%d\n", elem_attr.obj_y);
		printf("obj_w=%d\n", elem_attr.obj_w);
		printf("obj_h=%d\n", elem_attr.obj_h);

		printf("obj_opa=%d\n", elem_attr.obj_opa);
		printf("obj_r=%d\n", elem_attr.obj_r);
		printf("obj_g=%d\n", elem_attr.obj_g);
		printf("obj_b=%d\n", elem_attr.obj_b);

		printf("obj_opa_2=%d\n", elem_attr.obj_opa_2);
		printf("obj_r_2=%d\n", elem_attr.obj_r_2);
		printf("obj_g_2=%d\n", elem_attr.obj_g_2);
		printf("obj_b_2=%d\n", elem_attr.obj_b_2);

		printf("obj_opa_3=%d\n", elem_attr.obj_opa_3);
		printf("obj_r_3=%d\n", elem_attr.obj_r_3);
		printf("obj_g_3=%d\n", elem_attr.obj_g_3);
		printf("obj_b_3=%d\n", elem_attr.obj_b_3);

		
		printf("obj_font_name=%s\n",elem_attr.obj_font_name);
		printf("obj_font_r=%d\n", elem_attr.obj_font_r);
		printf("obj_font_g=%d\n", elem_attr.obj_font_g);
		printf("obj_font_b=%d\n", elem_attr.obj_font_b);

		printf("obj_time=%d\n", elem_attr.obj_time);
		printf("obj_range_min=%d\n", elem_attr.obj_range_min);
		printf("obj_range_max=%d\n", elem_attr.obj_range_max);

		printf("obj_var[0]=%d\n", elem_attr.obj_var[0]);
		printf("obj_var[1]=%d\n", elem_attr.obj_var[1]);
		printf("obj_var[2]=%d\n", elem_attr.obj_var[2]);
		printf("obj_var[3]=%d\n", elem_attr.obj_var[3]);
		printf("obj_var[4]=%d\n", elem_attr.obj_var[4]);
		printf("obj_var[5]=%d\n", elem_attr.obj_var[5]);
		printf("obj_var[6]=%d\n", elem_attr.obj_var[6]);
		printf("obj_var[7]=%d\n", elem_attr.obj_var[7]);
		printf("obj_var[8]=%d\n", elem_attr.obj_var[8]);
		printf("obj_var[9]=%d\n", elem_attr.obj_var[9]);

		
		printf("obj_event=%d\n", elem_attr.obj_event);
		printf("================================================================\n");
		#endif
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










