
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

//******************************************************************

static hmi_page_t*  page_head = NULL;
static hmi_page_t   hmi_main_page  ;

//******************************************************************

hmi_element_t* Hmi_elem_create(obj_attr_t para)
{
	hmi_element_t* obj = NULL;  

	obj = (hmi_element_t *)malloc(sizeof(hmi_element_t));
	if(!obj){
		printf("create error.\n");
		return 0;
	}
	
	memset(obj, 0, sizeof(hmi_element_t));
	slist_init(&(obj->elem_l_tail));

	memcpy(&obj->elem_attr,&para,sizeof(obj_attr_t));
	
	return obj;
}



int Hmi_elem_del(hmi_element_t* obj)
{
	if(!obj){
		printf("invalid para.\n");
		return -1;
	}

	free(obj);
	obj = NULL;

	return 0;
}



//******************************************************************

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

//**********************************************************************


//**********************************************************************
//hmi page 

int hmi_struct_size_info(void)
{
	printf("hmi_page_t size=%d\n",sizeof(hmi_page_t));

	return 0;
}

int hmi_page_manage_init(void)
{
	page_head = (hmi_page_t *)malloc(sizeof(hmi_page_t));
	if (page_head == NULL) {
		printf("invalid elem head parameter.\n");
		return -1;
	}

	page_head->page_id  = 0xA000;
	page_head->page_num = 0;

	slist_init(page_head->page_elem_head);

	return 0;
}

int hmi_page_init(hmi_page_t *page,unsigned int id)
{
	unsigned i = 0;

	if(!page){
		printf("invalid parameter.\n");
		return -1;
	}

	memset(page, 0, sizeof(hmi_page_t));

	page->page_id 		= id;
	page->page_elem_num = 0;

	page->page_elem_head = (slist_t *)malloc(sizeof(slist_t));
	if (page->page_elem_head == NULL) {
		printf("invalid elem head parameter.\n");
		return -1;
	}

	slist_init(page->page_elem_head);

	page_head = page;

	return 0;
}


int hmi_page_free(hmi_page_t *page)
{
	if(!page){
		printf("invalid parameter.\n");
		return -1;
	}

	if(page->page_elem_head){
		free(page->page_elem_head);
		page->page_elem_head = NULL;
	}

	return 0;
}	

hmi_page_t* hmi_page_get_default(unsigned int id)
{

	return page_head;	
}

hmi_page_t* hmi_page_get_by_id(unsigned int id)
{
	hmi_page_t* temp = NULL;




	return temp;
}


int hmi_page_add_elem(hmi_page_t *page,obj_attr_t para)
{
	int i = 0;

	hmi_element_t *elem_obj = NULL;
	
	if (page == NULL) {
		printf("invalid page.\n");
		return -1;
	}

	if (page->page_elem_num >= MAX_PAGE_ELEM_NUM) {
		printf("Exceeded the maximum number of elements.\n");
		return -1;
	}

	elem_obj = Hmi_elem_create(para);
	if(elem_obj == NULL){
		printf("elem create error.\n");
		return -1;
	}

	slist_add_tail(page->page_elem_head ,&(elem_obj->elem_l_tail));

	page->page_elem_num++;

	return 0;
}


int hmi_page_del_elem(hmi_page_t *page , hmi_element_t *obj)
{
	int ret = 0;
	
	slist_t* node;
	hmi_element_t* elem = NULL;

	if(page == NULL){
		printf("page invalid page.\n");
		return -1;
	}
	if(obj == NULL){
		printf("obj invalid page.\n");
		return -1;
	}

	if(slist_isempty(page->page_elem_head)){
		printf("list empty.\n");
		return -1;
	}

	slist_for_each(node, (page->page_elem_head))
	{
		elem = slist_entry(node, hmi_element_t, elem_l_tail);
		if(elem == obj){
			ret = 1;
			break;
		}
	}

	if(ret){
		slist_remove(page->page_elem_head, &(obj->elem_l_tail));
		page->page_elem_num--;
	}

	Hmi_elem_del(obj);
	obj = NULL;

	return 0;
}

hmi_element_t* hmi_page_get_elem_by_id(hmi_page_t* page, unsigned int elem_id)
{
	int ret = 0;
	
	unsigned int i = 0;

	slist_t *node;
	hmi_element_t*  elem = NULL;

	if (page == NULL) {
		printf("invalid page.\n");
		return 0;
	}
	
	if (page->page_elem_head == NULL) {
		printf("invalid elem list tail para.\n");
		return 0;
	}
	
	if(slist_isempty(page->page_elem_head)){
		printf("list empty.\n");
		return 0;
	}

    slist_for_each(node, (page->page_elem_head))
    {
        elem = slist_entry(node,hmi_element_t, elem_l_tail);
        if(elem->elem_attr.obj_id == elem_id){
			ret = 1;
			break;
        }
    }

	if(ret){
		return elem;
	}else{
		return 0;
	}
	
}


int hmi_page_trav_elem(hmi_page_t *page)
{
	slist_t* node;
	hmi_element_t* elem = NULL;

	if (page == NULL) {
		printf("page invalid page.\n");
		return -1;
	}

	if(slist_isempty(page->page_elem_head)){
		printf("list empty.\n");
		return -1;
	}

#if 0
	printf("==================================\n");
	printf("elem_num=%d\n",page->page_elem_num);
	printf("==================================\n");
#endif

	printf("num=%d\n",page->page_elem_num);


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
		}
	}


	return 0;
}


int hmi_page_del_all_elem(hmi_page_t *page)
{
	slist_t* node;
	hmi_element_t* elem = NULL;

	if (page == NULL) {
		printf("page invalid page.\n");
		return -1;
	}

	if(slist_isempty(page->page_elem_head)){
		printf("list empty.\n");
		return -1;
	}

	printf("num=%d\n",page->page_elem_num);

	slist_for_each(node, (page->page_elem_head))
	{
		elem = slist_entry(node, hmi_element_t, elem_l_tail);
		if(elem){
			printf("elem_id=%d\n",elem->elem_attr.obj_id);
			hmi_page_del_elem(page,elem);	
		}
	}


	return 0;
}



int  hmi_page_update_elem_var(hmi_page_t *page,unsigned int id,obj_attr_t para)
{
	slist_t* node;
	hmi_element_t* elem = NULL;

	if (page == NULL) {
		printf("page invalid page.\n");
		return -1;
	}

	if(slist_isempty(page->page_elem_head)){
		printf("list empty.\n");
		return -1;
	}

	//printf("num=%d\n",page->page_elem_num);
	//printf("id=%d\n", id);

	slist_for_each(node, (page->page_elem_head))
	{
		elem = slist_entry(node, hmi_element_t, elem_l_tail);
		if((elem)&&(elem->elem_attr.obj_id == id)){

			memcpy(&(elem->elem_attr.obj_var),&(para.obj_var),MAX_OBJ_VAR_LEN);

			//printf("var0=%d\n", elem->elem_attr.obj_var[0]);
			//printf("var1=%d\n", elem->elem_attr.obj_var[1]);
			//printf("var2=%d\n", elem->elem_attr.obj_var[2]);
		}
	}


	return 0;
}


int  hmi_page_update_elem_data(hmi_page_t *page,unsigned int id,obj_attr_t para)
{
	slist_t* node;
	hmi_element_t* elem = NULL;

	if (page == NULL) {
		printf("page invalid page.\n");
		return -1;
	}

	if(slist_isempty(page->page_elem_head)){
		printf("list empty.\n");
		return -1;
	}

	//printf("num=%d\n",page->page_elem_num);
	//printf("id=%d\n", id);

	slist_for_each(node, (page->page_elem_head))
	{
		elem = slist_entry(node, hmi_element_t, elem_l_tail);
		if((elem)&&(elem->elem_attr.obj_id == id)){

			memcpy(&(elem->elem_attr.obj_data),&(para.obj_data),MAX_OBJ_DATA_LEN);

			//printf("obj_data=%s\n",elem->elem_attr.obj_data);
		}
	}


	return 0;
}


//**********************************************************************
int  hmi_init(void)
{
	hmi_page_init(&hmi_main_page,0x01);

	
	


	return 0;
}
//**********************************************************************

int hmi_add_obj(hmi_page_t *page,obj_attr_t para)
{
	int ret = -1;
	
	ret = hmi_page_add_elem(page,para);
	if(ret < 0){
		printf("add error\n");
		return -1;
	}

	return 0;
}

int hmi_del_obj(hmi_page_t *page,unsigned int obj_id)
{
	int ret = -1;

	hmi_element_t* elem = NULL;
	
	elem = hmi_page_get_elem_by_id(page,obj_id);
	if(!elem){
		printf("no obj\n");	
		return -1;
	}

	hmi_page_del_elem(page,elem);	


	return 0;
}









#if 0
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
#endif


#if 0
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
#endif

#if 0
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
#endif


int hmi_config_wifi_info(const char *wifi_name, const char *wifi_pwd)
{
	obj_attr_t para ;

	memset(&para,0,sizeof(obj_attr_t));

	para.obj_id   = 0;
	para.obj_type = HMI_OBJ_TYPE_WIFI_INFO;
	
	memcpy(&para.obj_wifi_name, "jieshen", strlen("jieshen"));
	memcpy(&para.obj_wifi_pass, "Jieshen168", strlen("Jieshen168"));

	hmi_add_obj(hmi_page_get_default(0),para);


	return 0;
}


int hmi_laber_create_test(unsigned int id)
{
	obj_attr_t para ;

	memset(&para,0,sizeof(obj_attr_t));

	para.obj_id   = id;
	para.obj_type = HMI_OBJ_TYPE_LABEL;

	memcpy(&para.obj_name,"label",strlen("label"));
	
	para.obj_x	  = 0;
	para.obj_y	  = 0;
	para.obj_w	  = 640;
	para.obj_h	  = 80;
	
	para.obj_opa		 = 0;
	para.obj_r			 = 0;
	para.obj_g			 = 0;
	para.obj_b			 = 255;

	para.obj_opa_2		 = 255;
	para.obj_r_2		 = 0;
	para.obj_g_2		 = 0;
	para.obj_b_2		 = 0;

	memcpy(&para.obj_font_name, "aa.ttf", strlen("aa.ttf"));

	para.obj_font_size   = 80;
	para.obj_font_r		 = 0;
	para.obj_font_g		 = 255;
	para.obj_font_b		 = 0;	
	
	para.obj_time        = 1000;

	para.obj_point_x_num = 20;
	para.obj_point_y_num = 5;

	para.obj_range_min	 = 0;
	para.obj_range_max	 = 100000;
	
	para.obj_event		 = HMI_OBJ_EVENT_DEFAULT;
	para.obj_action 	 = 0;

	memcpy(&para.obj_data, "label包图小白体.ttf测试", strlen("label包图小白体.ttf测试"));

	hmi_add_obj(hmi_page_get_default(0),para);


	return 0;
}

int hmi_laber_create_test2(unsigned int id)
{
	obj_attr_t para ;

	memset(&para,0,sizeof(obj_attr_t));

	para.obj_id   = id;
	para.obj_type = HMI_OBJ_TYPE_LABEL;

	memcpy(&para.obj_name,"label",strlen("label"));
	
	para.obj_x	  = 0;
	para.obj_y	  = 150;
	para.obj_w	  = 640;
	para.obj_h	  = 80;
	
	para.obj_opa		 = 0;
	para.obj_r			 = 0;
	para.obj_g			 = 0;
	para.obj_b			 = 255;

	para.obj_opa_2		 = 255;
	para.obj_r_2		 = 0;
	para.obj_g_2		 = 0;
	para.obj_b_2		 = 0;

	memcpy(&para.obj_font_name, "bb.ttf", strlen("bb.ttf"));

	para.obj_font_size   = 80;
	para.obj_font_r		 = 255;
	para.obj_font_g		 = 0;
	para.obj_font_b		 = 0;	
	
	para.obj_time        = 1000;

	para.obj_point_x_num = 20;
	para.obj_point_y_num = 5;

	para.obj_range_min	 = 0;
	para.obj_range_max	 = 100000;
	
	para.obj_event		 = HMI_OBJ_EVENT_DEFAULT;
	para.obj_action 	 = 0;

	memcpy(&para.obj_data, "label今年也要加油鸭.ttf测试", strlen("label今年也要加油鸭.ttf测试"));

	hmi_add_obj(hmi_page_get_default(0),para);


	return 0;
}

int hmi_img_create_test(unsigned int id)
{
	obj_attr_t para ;

	memset(&para,0,sizeof(obj_attr_t));

	para.obj_id   = id;
	para.obj_type = HMI_OBJ_TYPE_IMG;
	
	para.obj_x	  = 160;
	para.obj_y	  = 0;
	para.obj_w	  = 320;
	para.obj_h	  = 320;
	
	para.obj_opa	= 0;
	para.obj_r		= 0;
	para.obj_g		= 0;
	para.obj_b		= 255;

	para.obj_opa_2	= 0;
	para.obj_r_2	= 0;
	para.obj_g_2	= 0;
	para.obj_b_2	= 255;

	para.obj_font_size  = 0;
	para.obj_font_r		= 0;
	para.obj_font_g		= 0;
	para.obj_font_b		= 0;	
	
	para.obj_time       = 1000;

	para.obj_point_x_num = 0;
	para.obj_point_y_num = 0;

	para.obj_range_min	= 0;
	para.obj_range_max	= 0;
	
	para.obj_event		= HMI_OBJ_EVENT_DEFAULT;
	para.obj_action 	= 0;

	memcpy(para.obj_data, "menu_4.png", strlen("menu_4.png"));


	hmi_add_obj(hmi_page_get_default(0),para);



	return 0;
}


int hmi_chart_line_create_test(unsigned int id)
{
	int len = 0;
	
	obj_attr_t para ;

	memset(&para,0,sizeof(obj_attr_t));

	para.obj_id   = id;
	para.obj_type = HMI_OBJ_TYPE_BROKEN;

	memcpy(&para.obj_name,"chart line",strlen("chart line"));
	
	para.obj_x	  = 0;
	para.obj_y	  = 0;
	para.obj_w	  = 320;
	para.obj_h	  = 320;
	
	para.obj_opa		 = 0;
	para.obj_r			 = 0;
	para.obj_g			 = 0;
	para.obj_b			 = 255;

	para.obj_opa_2		 = 255;
	para.obj_r_2		 = 0;
	para.obj_g_2		 = 0;
	para.obj_b_2		 = 255;

	para.obj_font_size   = 20;
	para.obj_font_r		 = 0;
	para.obj_font_g		 = 255;
	para.obj_font_b		 = 0;

	para.obj_rise_r = 255;
	para.obj_rise_g = 0;
	para.obj_rise_b = 0;	

	para.obj_fall_r = 0;
	para.obj_fall_g = 255;
	para.obj_fall_b = 0; 	
	
	para.obj_time        = 1000;

	para.obj_point_x_num = 5;
	para.obj_point_y_num = 5;

	para.obj_range_min	 = 0;
	para.obj_range_max	 = 10;
	
	para.obj_event		 = HMI_OBJ_EVENT_URL_DATA;
	para.obj_action 	 = 0;

	len = strlen("https://stock-f-reminr-server-aybofoxbug.us-west-1.fcapp.run?BABA");

	memcpy(&para.obj_url,"https://stock-f-reminr-server-aybofoxbug.us-west-1.fcapp.run?BABA",len);
	memcpy(&para.obj_field,"increase",strlen("increase"));


	hmi_add_obj(hmi_page_get_default(0),para);

	return 0;
}

int hmi_chart_line_create_test2(unsigned int id)
{
	int len = 0;
	obj_attr_t para ;

	memset(&para,0,sizeof(obj_attr_t));

	para.obj_id   = id;
	para.obj_type = HMI_OBJ_TYPE_BROKEN;

	memcpy(&para.obj_name,"chart line",strlen("chart line"));
	
	para.obj_x	  = 320;
	para.obj_y	  = 0;
	para.obj_w	  = 320;
	para.obj_h	  = 320;
	
	para.obj_opa		 = 0;
	para.obj_r			 = 0;
	para.obj_g			 = 0;
	para.obj_b			 = 255;

	para.obj_opa_2		 = 255;
	para.obj_r_2		 = 0;
	para.obj_g_2		 = 0;
	para.obj_b_2		 = 255;

	para.obj_font_size   = 20;
	para.obj_font_r		 = 0;
	para.obj_font_g		 = 255;
	para.obj_font_b		 = 0;	

	para.obj_rise_r = 255;
	para.obj_rise_g = 0;
	para.obj_rise_b = 0;	

	para.obj_fall_r = 0;
	para.obj_fall_g = 255;
	para.obj_fall_b = 0; 	

	
	para.obj_time        = 1000;

	para.obj_point_x_num = 5;
	para.obj_point_y_num = 5;

	para.obj_range_min	 = 0;
	para.obj_range_max	 = 10;
	
	para.obj_event		 = HMI_OBJ_EVENT_URL_DATA;
	para.obj_action 	 = 0;

	len = strlen("https://stock-f-reminr-server-aybofoxbug.us-west-1.fcapp.run?00700");

	memcpy(&para.obj_url,"https://stock-f-reminr-server-aybofoxbug.us-west-1.fcapp.run?00700",len);
	memcpy(&para.obj_field,"increase",strlen("increase"));	

	hmi_add_obj(hmi_page_get_default(0),para);

	return 0;
}



int hmi_metter_crc_create_test(unsigned int id)
{
	obj_attr_t para ;

	memset(&para,0,sizeof(obj_attr_t));

	para.obj_id   = id;
	para.obj_type = HMI_OBJ_TYPE_CRC;
	
	para.obj_x	  = 0;
	para.obj_y	  = 0;
	para.obj_w	  = 320;
	para.obj_h	  = 320;
	
	para.obj_opa		 = 255;
	para.obj_r			 = 128;
	para.obj_g			 = 0;
	para.obj_b			 = 0;

	para.obj_opa_2		 = 255;
	para.obj_r_2		 = 0;
	para.obj_g_2		  = 0;
	para.obj_b_2		  = 255;

	para.obj_time        = 1000;

	para.obj_range_min	 = 0;
	para.obj_range_max	 = 360;
	para.obj_var[0]       = 360;

	para.obj_angle_range = 360; //0-360 
  	para.obj_rotation    = 90;
	para.obj_width       = 10;


	hmi_add_obj(hmi_page_get_default(0),para);

	return 0;
}

int hmi_chart_straight_create_test(unsigned int id)
{
	obj_attr_t para ;

	memset(&para,0,sizeof(obj_attr_t));

	para.obj_id   = id;
	para.obj_type = HMI_OBJ_TYPE_STRAIGHT;

	memcpy(&para.obj_name,"straight",strlen("straight"));

	para.obj_x	  = 0;
	para.obj_y	  = 0;
	para.obj_w	  = 960;
	para.obj_h	  = 320;
	
	para.obj_opa		 = 0;
	para.obj_r			 = 0;
	para.obj_g			 = 0;
	para.obj_b			 = 0;

	para.obj_opa_2		 = 255;
	para.obj_r_2		 = 0;
	para.obj_g_2		  = 0;
	para.obj_b_2		  = 255;

	para.obj_opa_3		 = 255;
	para.obj_r_3		 = 255;
	para.obj_g_3		  = 0;
	para.obj_b_3		  = 0;

	para.obj_time        = 1000;
	
	para.obj_point_x_num = 20;
	para.obj_point_y_num = 10;
	
	para.obj_range_min = 0;
	para.obj_range_max = 100000;


	hmi_add_obj(hmi_page_get_default(0),para);

	return 0;
}



int hmi_bar_create_test(unsigned int id)
{
	obj_attr_t para ;

	memset(&para,0,sizeof(obj_attr_t));

	para.obj_id   = id;
	para.obj_type = HMI_OBJ_TYPE_PROGRESS_BAR;


	para.obj_x	  = 0;
	para.obj_y	  = 160;
	para.obj_w	  = 960;
	para.obj_h	  = 80;
	
	para.obj_opa		 = 0;
	para.obj_r			 = 0;
	para.obj_g			 = 0;
	para.obj_b			 = 0;

	para.obj_opa_2		 = 255;
	para.obj_r_2		 = 0;
	para.obj_g_2		 = 0;
	para.obj_b_2		 = 255;

	para.obj_font_size   = 80;

	para.obj_font_r  = 255;
	para.obj_font_g  = 0;
	para.obj_font_b  = 0;

	para.obj_time        = 1000;

	para.obj_range_min = 0;
	para.obj_range_max = 2000;
	para.obj_var[0] = 500;

	memcpy(para.obj_data, "bar 中文 测试sfadfasfdasfasdsaf", strlen("bar 中文 测试sfadfasfdasfasdsaf"));

	hmi_add_obj(hmi_page_get_default(0),para);

	return 0;
}


int hmi_bgimage_create_test(unsigned int id)
{
	obj_attr_t para ;

	memset(&para,0,sizeof(obj_attr_t));

	para.obj_id   = id;
	para.obj_type = HMI_OBJ_TYPE_BG_IMAGE;

	para.obj_x	  = 320;
	para.obj_y	  = 0;
	para.obj_w	  = 320;
	para.obj_h	  = 320;
	
	memcpy(para.obj_data, "00.jpg", strlen("00.jpg"));

	hmi_add_obj(hmi_page_get_default(0),para);

	return 0;
}


int hmi_bgvideo_create_test(unsigned int id)
{
	obj_attr_t para ;

	memset(&para,0,sizeof(obj_attr_t));

	para.obj_id   = id;
	para.obj_type = HMI_OBJ_TYPE_BG_VIDEO;

	para.obj_x	  = 640;
	para.obj_y	  = 0;
	para.obj_w	  = 320;
	para.obj_h	  = 320;
	
	memcpy(para.obj_data, "00.mp4", strlen("00.mp4"));

	hmi_add_obj(hmi_page_get_default(0),para);

	return 0;
}



int hmi_create_obj_test(void)
{
	int i = 0;
	unsigned int len = 0;
	obj_attr_t para ;
	
	char control_buff[1024] = {0};

	memset(&para,0,sizeof(obj_attr_t));
	memset(control_buff,0,sizeof(control_buff));



	hmi_config_wifi_info("wifi", "pwd");

	hmi_laber_create_test(1);
	//hmi_laber_create_test2(2);

	hmi_img_create_test(2);

	hmi_chart_line_create_test(3);
	hmi_chart_line_create_test2(9);

	//hmi_metter_crc_create_test(4);

	//hmi_chart_straight_create_test(5);

	//hmi_bar_create_test(6);

	//hmi_bgimage_create_test(7);

	hmi_bgvideo_create_test(8);
	

	

	return 0;
}


int hmi_delete_obj(unsigned int id)
{


	return 0;
}




