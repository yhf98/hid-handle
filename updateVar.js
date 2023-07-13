/*
 * @Author: yaohengfeng 1921934563@qq.com
 * @Date: 2023-01-13 10:53:22
 * @LastEditors: 姚恒锋 1921934563@qq.com
 * @LastEditTime: 2023-07-12 17:44:44
 * @FilePath: \hid-handle\test.js
 * @Description: test.js
 */
// const random = require('string-random');
const hid = require("./hidhandle.js");
// let hidHandle = hid.HidHandle();

// console.info("handle: ", hidHandle);
let count = 0;
setInterval(() => {
  hid.HidHandle().then(fn => {
    let data = [
      {
        "obj_id": 1,
        "obj_type": 1,
        "obj_name": "untitled0",
        "obj_x": 245,
        "obj_y": 97,
        "obj_w": 200,
        "obj_h": 120,
        "obj_opa": 255,
        "obj_r": 0,
        "obj_g": 0,
        "obj_b": 0,
        "obj_opa_2": 255,
        "obj_r_2": 0,
        "obj_g_2": 0,
        "obj_b_2": 0,
        "obj_font_size": 80,
        "obj_font_r": 255,
        "obj_font_g": 0,
        "obj_font_b": 0,
        "obj_range_min": 0,
        "obj_range_max": 1000000,
        "obj_angle_range": 0,
        "obj_rotation": 0,
        "obj_width": 200,
        "obj_point_x_num": 5,
        "obj_point_y_num": 5,
        "obj_time": 0,
        "obj_var": [
          54.3
        ],
        "obj_event": 4,
        "obj_action": 0,
        "obj_data": "54.3",
        "obj_reserve": "",
        "obj_url": "",
        "obj_field": "",
        "obj_align_reserve": ""
      }
    ]
    fn.hmi_batch_update_screen_data(data)
  })
  count++
  console.info(`==================执行了【${count}】=======================次`)
}, 500)