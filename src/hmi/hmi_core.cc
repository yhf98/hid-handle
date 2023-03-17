
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

	obj->elem_attr.obj_id 	= para.obj_id;
	obj->elem_attr.obj_type = para.obj_type;

	memset(&(obj->elem_attr.obj_name), 0, MAX_OBJ_NAME_LEN);
	memcpy(&(obj->elem_attr.obj_name),&(para.obj_name),MAX_OBJ_NAME_LEN);
	
	obj->elem_attr.obj_x    = para.obj_x;
	obj->elem_attr.obj_y    = para.obj_y;
	obj->elem_attr.obj_w    = para.obj_w;
	obj->elem_attr.obj_h    = para.obj_h;

	obj->elem_attr.obj_opa  = para.obj_opa;
	obj->elem_attr.obj_r    = para.obj_r;
	obj->elem_attr.obj_g   	= para.obj_g;
	obj->elem_attr.obj_b    = para.obj_b;

	obj->elem_attr.obj_opa_2    = para.obj_opa_2;
	obj->elem_attr.obj_r_2      = para.obj_r_2;
	obj->elem_attr.obj_g_2   	= para.obj_g_2;
	obj->elem_attr.obj_b_2      = para.obj_b_2;


	obj->elem_attr.obj_font_size  = para.obj_font_size;
	obj->elem_attr.obj_font_r     = para.obj_font_r;
	obj->elem_attr.obj_font_g     = para.obj_font_g;
	obj->elem_attr.obj_font_b     = para.obj_font_b;	

	obj->elem_attr.obj_range_min  = para.obj_range_min;
	obj->elem_attr.obj_range_max  = para.obj_range_max;
	obj->elem_attr.obj_angle_range  = para.obj_angle_range;
	obj->elem_attr.obj_rotation     = para.obj_rotation;
	obj->elem_attr.obj_width        = para.obj_width;

	obj->elem_attr.obj_point_x_num = para.obj_point_x_num;
	obj->elem_attr.obj_point_y_num = para.obj_point_y_num;
	obj->elem_attr.obj_time        = para.obj_time;

	obj->elem_attr.obj_event      = para.obj_event;
	obj->elem_attr.obj_action     = para.obj_action;

	memset(&(obj->elem_attr.obj_var), 0, MAX_OBJ_VAR_LEN);
	memcpy(&(obj->elem_attr.obj_var),&(para.obj_var),MAX_OBJ_VAR_LEN);

	memset(&(obj->elem_attr.obj_data), 0, MAX_OBJ_DATA_LEN);
	memcpy(&(obj->elem_attr.obj_data),&(para.obj_data),MAX_OBJ_DATA_LEN);

	memset(&(obj->elem_attr.obj_reserve), 0, MAX_RESERVE_LEN);
	memcpy(&(obj->elem_attr.obj_reserve),&(para.obj_reserve),MAX_RESERVE_LEN);

	
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

#if 1
int hmi_create_obj_test(void)
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
	
	para.obj_time       = 200;

	para.obj_range_min	= 0;
	para.obj_range_max	= 100;
	
	para.obj_event		= HMI_OBJ_EVENT_NETTIME_DAY;
	para.obj_action 	= 0;

	hmi_add_obj(hmi_page_get_default(0),para);
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

	hmi_add_obj(hmi_page_get_default(0),para);

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

	hmi_add_obj(hmi_page_get_default(0),para);

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

		hmi_add_obj(hmi_page_get_default(0),para);

	}
#endif


#if 1
	for(i=0;i<8;i++)
	{
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
		
		para.obj_id   = 0x11+i;
		para.obj_type = HMI_OBJ_TYPE_STRAIGHT;


		para.obj_opa	= 0;
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

		hmi_add_obj(hmi_page_get_default(0),para);

	}
#endif


#if 1 //  png obj
	memset(&para,0,sizeof(obj_attr_t));

	para.obj_id = 0x08+0;
	para.obj_type = HMI_OBJ_TYPE_IMG;

	para.obj_x	= 192;
	para.obj_y	= 0;
	para.obj_w	= 320;
	para.obj_h	= 320;

	para.obj_opa	= 0;
	para.obj_r		= 255;
	para.obj_g		= 0;
	para.obj_b		= 0;

	para.obj_opa_2	= 0;
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

	sprintf((para.obj_data), "%s","menu_4.png");

	hmi_add_obj(hmi_page_get_default(0),para);

#endif

#if 1 //  png obj
	memset(&para,0,sizeof(obj_attr_t));

	para.obj_id = 0x08+1;
	para.obj_type = HMI_OBJ_TYPE_IMG;

	para.obj_x	= 192+320;
	para.obj_y	= 0;
	para.obj_w	= 334;
	para.obj_h	= 68;

	para.obj_opa	= 0;
	para.obj_r		= 255;
	para.obj_g		= 0;
	para.obj_b		= 0;

	para.obj_opa_2	= 0;
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

	sprintf((para.obj_data), "%s","audio_spect_4.png");

	hmi_add_obj(hmi_page_get_default(0),para);
	
#endif

#if 1 //  jpg obj
	memset(&para,0,sizeof(obj_attr_t));

	para.obj_id = 0x08+2;
	para.obj_type = HMI_OBJ_TYPE_BG_IMAGE;

	para.obj_x	= 0;
	para.obj_y	= 0;
	para.obj_w	= 960;
	para.obj_h	= 320;

	para.obj_opa	= 0;
	para.obj_r		= 255;
	para.obj_g		= 0;
	para.obj_b		= 0;

	para.obj_opa_2	= 0;
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

	sprintf((para.obj_data), "%s","1631551139.jpg");

	hmi_add_obj(hmi_page_get_default(0),para);
		
#endif



	return 0;
}

#endif

#if 0
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
#endif




