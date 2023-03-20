/*
 * @Author: yaohengfeng 1921934563@qq.com
 * @Date: 2023-01-13 10:53:22
 * @LastEditors: yaohengfeng 1921934563@qq.com
 * @LastEditTime: 2023-03-20 10:13:54
 * @FilePath: \hid-handle\test.js
 * @Description: test.js
 */
const hid = require("./hidhandle.js");

// console.info(native);

// let res = native.hid_write_file_handle("safasf","asfas",1);

// console.info(res)

// const hid = require('H:\\hid-handle\\build\\Release\\hidhandle.node')
let hidHandle = hid.HidHandle();

console.info(hidHandle);

// console.info(hidHandle.hid_write_file_handle("H:/hid-handle/test.js", "test.js", 0))

let element = [
  {
    obj_id: 0,
    obj_type: 1,
    obj_name: "label1",
    obj_x: 0,
    obj_y: 25,
    obj_w: 200,
    obj_h: 20,
    obj_opa: 255,
    obj_r: 255,
    obj_g: 12,
    obj_b: 12,
    obj_opa_2: 0,
    obj_r_2: 0,
    obj_g_2: 0,
    obj_b_2: 0,
    obj_font_size: 0,
    obj_font_r: 0,
    obj_font_g: 0,
    obj_font_b: 0,
    obj_range_min: 0,
    obj_range_max: 1000000,
    obj_angle_range: 0,
    obj_rotation: 0,
    obj_width: 0,
    obj_point_x_num: 0,
    obj_point_y_num: 0,
    obj_time: 0,
    obj_var: [0, 1, 2, 3, 4, 5],
    obj_event: 0,
    obj_action: 0,
    obj_data: "C:/Users/idmin/Pictures/icon.jpg",
    obj_reserve: "abc",
  },
  {
    obj_id: 1,
    obj_type: 2,
    obj_name: "img1",
    obj_x: 192,
    obj_y: 0,
    obj_w: 320,
    obj_h: 320,
    obj_opa: 0,
    obj_r: 255,
    obj_g: 0,
    obj_b: 0,
    obj_opa_2: 0,
    obj_r_2: 0,
    obj_g_2: 0,
    obj_b_2: 255,
    obj_font_size: 0,
    obj_font_r: 0,
    obj_font_g: 0,
    obj_font_b: 255,
    obj_range_min: 0,
    obj_range_max: 1000000,
    obj_angle_range: 0,
    obj_rotation: 0,
    obj_width: 0,
    obj_point_x_num: 0,
    obj_point_y_num: 0,
    obj_time: 0,
    obj_var: [50, 60, 70],
    obj_event: 0,
    obj_action: 0,
    obj_data: "miao.jpg",
    obj_reserve: "/miao.jpg",
  },
];

console.info("================================")
console.info(hidHandle.hmi_add_obj_handle(element));
