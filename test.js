/*
 * @Author: yaohengfeng 1921934563@qq.com
 * @Date: 2023-01-13 10:53:22
 * @LastEditors: yaohengfeng 1921934563@qq.com
 * @LastEditTime: 2023-03-20 16:56:39
 * @FilePath: \hid-handle\test.js
 * @Description: test.js
 */

const hid = require("./hidhandle.js");
let hidHandle = hid.HidHandle();

console.info("handle: ", hidHandle);

let element = [
  {
    obj_id: 0,
    obj_type: 1,
    obj_name: "label1",
    obj_x: 0,
    obj_y: 0,
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
    obj_var: [],
    obj_event: 0,
    obj_action: 0,
    obj_data: "label1",
    obj_reserve: "icon.jpg",
  },
  {
    obj_id: 1,
    obj_type: 1,
    obj_name: "label2",
    obj_x: 0,
    obj_y: 50,
    obj_w: 200,
    obj_h: 50,
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
    obj_time: 200,
    obj_var: [],
    obj_event: 2,
    obj_action: 0,
    obj_data: "系统时间",
    obj_reserve: "",
  },
  {
    obj_id: 2,
    obj_name: "label3",
    obj_type: 6,

    obj_x: 0,
    obj_y: 90,
    obj_w: 192,
    obj_h: 20,

    obj_opa: 255,
    obj_r: 255,
    obj_g: 0,
    obj_b: 0,

    obj_opa_2: 255,
    obj_r_2: 0,
    obj_g_2: 0,
    obj_b_2: 255,

    obj_font_size: 0,
    obj_font_r: 0,
    obj_font_g: 255,
    obj_font_b: 0,

    obj_range_min: 0,
    obj_range_max: 100,
    obj_angle_range: 360,
    obj_rotation: 90,
    obj_width: 20,

    obj_point_x_num: 0,
    obj_point_y_num: 0,

    obj_time: 1000,

    obj_var: [50, 60, 70],

    obj_event: 0,
    obj_action: 0,
    obj_data: "CPU",
    obj_reserve: "",
  },
  {
    obj_id: 3,
    obj_name: "label4",
    obj_type: 6,

    obj_x: 0,
    obj_y: 130,
    obj_w: 192,
    obj_h: 20,

    obj_opa: 255,
    obj_r: 255,
    obj_g: 0,
    obj_b: 0,

    obj_opa_2: 255,
    obj_r_2: 0,
    obj_g_2: 0,
    obj_b_2: 255,

    obj_font_size: 0,
    obj_font_r: 0,
    obj_font_g: 255,
    obj_font_b: 0,

    obj_range_min: 0,
    obj_range_max: 100,
    obj_angle_range: 360,
    obj_rotation: 90,
    obj_width: 20,

    obj_point_x_num: 0,
    obj_point_y_num: 0,

    obj_time: 1000,

    obj_var: [50, 60, 70],

    obj_event: 0,
    obj_action: 0,
    obj_data: "网络",
    obj_reserve: "",
  },
  {
    obj_id: 4,
    obj_name: "label5",
    obj_type: 6,

    obj_x: 0,
    obj_y: 170,
    obj_w: 192,
    obj_h: 20,

    obj_opa: 255,
    obj_r: 255,
    obj_g: 0,
    obj_b: 0,

    obj_opa_2: 255,
    obj_r_2: 0,
    obj_g_2: 0,
    obj_b_2: 255,

    obj_font_size: 0,
    obj_font_r: 0,
    obj_font_g: 255,
    obj_font_b: 0,

    obj_range_min: 0,
    obj_range_max: 100,
    obj_angle_range: 360,
    obj_rotation: 90,
    obj_width: 20,

    obj_point_x_num: 0,
    obj_point_y_num: 0,

    obj_time: 1000,

    obj_var: [50, 60, 70],

    obj_event: 0,
    obj_action: 0,
    obj_data: "内存",
    obj_reserve: "",
  },
  {
    obj_id: 4,
    obj_name: "label15",
    obj_type: 6,

    obj_x: 0,
    obj_y: 210,
    obj_w: 192,
    obj_h: 20,

    obj_opa: 255,
    obj_r: 255,
    obj_g: 0,
    obj_b: 0,

    obj_opa_2: 255,
    obj_r_2: 0,
    obj_g_2: 0,
    obj_b_2: 255,

    obj_font_size: 0,
    obj_font_r: 0,
    obj_font_g: 255,
    obj_font_b: 0,

    obj_range_min: 0,
    obj_range_max: 100,
    obj_angle_range: 360,
    obj_rotation: 90,
    obj_width: 20,

    obj_point_x_num: 0,
    obj_point_y_num: 0,

    obj_time: 1000,

    obj_var: [50, 60, 70],

    obj_event: 0,
    obj_action: 0,
    obj_data: "磁盘",
    obj_reserve: "",
  },

  {
    obj_id: 8,
    obj_name: "graph1",
    obj_type: 5,

    obj_x: 200,
    obj_y: 20,
    obj_w: 192,
    obj_h: 160,

    obj_opa: 0,
    obj_r: 0,
    obj_g: 255,
    obj_b: 0,

    obj_opa_2: 255,
    obj_r_2: 0,
    obj_g_2: 0,
    obj_b_2: 255,

    obj_font_size: 0,
    obj_font_r: 0,
    obj_font_g: 255,
    obj_font_b: 0,

    obj_range_min: 0,
    obj_range_max: 100,
    obj_angle_range: 360,
    obj_rotation: 90,
    obj_width: 20,

    obj_point_x_num: 1,
    obj_point_y_num: 5,

    obj_time: 1000,

    obj_var: [10, 30, 70],

    obj_event: 0,
    obj_action: 0,
    obj_data: "Bilibili",
    obj_reserve: "",
  },


  {
    obj_id: 12,
    obj_type: 2,
    obj_name: "img1",
    obj_x: 500,
    obj_y: 10,
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
    obj_data: "333.png",
    obj_reserve: "1111.png",
  },
]

console.info("============================================================");

// 设置WIFI
// console.info("设置WIFI：")
// console.info(hidHandle.hmi_send_wifi_info_handle("jieshen", "Jieshen168"))

// 生成UI
console.info(hidHandle.hmi_add_obj_handle(element));

// 写入文件更新UI
console.info(
  hidHandle.hid_write_file_handle("./ui/hmi_res.hbin", "hmi_res.hbin", 1)
);
