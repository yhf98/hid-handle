/*
 * @Author: yaohengfeng 1921934563@qq.com
 * @Date: 2023-01-13 10:53:22
 * @LastEditors: 姚恒锋 1921934563@qq.com
 * @LastEditTime: 2023-05-31 10:31:11
 * @FilePath: \hid-handle\test.js
 * @Description: test.js
 */
const random = require('string-random');
const hid = require("../main.js");
let hidHandle = hid.HidHandle();

let count = 0;
setInterval(() => {
  let data = [
    {
      "obj_id": 0,
      "obj_type": 1,
      "obj_name": "code",
      "obj_x": 0,
      "obj_y": 0,
      "obj_w": 100,
      "obj_h": 30,
      "obj_opa": 255,
      "obj_r": 196,
      "obj_g": 39,
      "obj_b": 39,
      "obj_opa_2": 255,
      "obj_r_2": 0,
      "obj_g_2": 0,
      "obj_b_2": 0,
      "obj_font_size": 12,
      "obj_font_r": 107,
      "obj_font_g": 245,
      "obj_font_b": 178,
      "obj_range_min": 0,
      "obj_range_max": 100,
      "obj_angle_range": 0,
      "obj_rotation": 0,
      "obj_width": 100,
      "obj_point_x_num": 5,
      "obj_point_y_num": 5,
      "obj_time": 1000,
      "obj_var": [
        0
      ],
      "obj_event": 0,
      "obj_action": 0,
      "obj_data": random(16),
      "obj_reserve": ""
    },
    {
      "obj_id": 1,
      "obj_type": 1,
      "obj_name": "code",
      "obj_x": 100,
      "obj_y": 100,
      "obj_w": 100,
      "obj_h": 30,
      "obj_opa": 255,
      "obj_r": 196,
      "obj_g": 39,
      "obj_b": 39,
      "obj_opa_2": 255,
      "obj_r_2": 0,
      "obj_g_2": 0,
      "obj_b_2": 0,
      "obj_font_size": 12,
      "obj_font_r": 107,
      "obj_font_g": 245,
      "obj_font_b": 178,
      "obj_range_min": 0,
      "obj_range_max": 100,
      "obj_angle_range": 0,
      "obj_rotation": 0,
      "obj_width": 100,
      "obj_point_x_num": 5,
      "obj_point_y_num": 5,
      "obj_time": 1000,
      "obj_var": [
        0
      ],
      "obj_event": 0,
      "obj_action": 0,
      "obj_data": `执行了【${count}】`,
      "obj_reserve": ""
    }
  ]
  hidHandle.hmi_batch_update_screen_data(data)
  count++
  console.info(`==================执行了【${count}】=======================次`)
}, 1500)