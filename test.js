/*
 * @Author: yaohengfeng 1921934563@qq.com
 * @Date: 2023-01-13 10:53:22
 * @LastEditors: yaohengfeng 1921934563@qq.com
 * @LastEditTime: 2023-03-17 14:18:59
 * @FilePath: \hid-handle\test.js
 * @Description: test.js
 */
const hid = require('./hidhandle.js')

// console.info(native);

// let res = native.hid_write_file_handle("safasf","asfas",1);

// console.info(res)

// const hid = require('H:\\hid-handle\\build\\Release\\hidhandle.node')
let hidHandle = hid.HidHandle()

console.info(hidHandle)

// console.info(hidHandle.hid_write_file_handle("H:/hid-handle/test.js", "test.js", 0))

let element = {
    obj_id: 0,     
	obj_type: 1,
	obj_name: 'label1',
	obj_x: 0,
	obj_y: 25,
	obj_w: 200,
	obj_h: 20,
	obj_opa: 'good',
	obj_r: '0' ,
	obj_g: '0',
	obj_b: '255',
	obj_opa_2: '' ,
	obj_r_2: '',
	obj_g_2: '',
	obj_b_2: ''	,
	obj_font_size: '',
	obj_font_r: '',
	obj_font_g: '',
	obj_font_b: '',	
	obj_range_min: 0,
	obj_range_max: 1000000,
	obj_angle_range: 0,
  	obj_rotation: 0,
  	obj_width: 0,
	obj_point_x_num: 0,
	obj_point_y_num: 0,
	obj_time: 0,
	obj_var: 0,
	obj_event: 0,
	obj_action: 0,
	obj_data: '',
	obj_reserve:''
}
console.info(hidHandle.hmi_add_obj_handle(element))


