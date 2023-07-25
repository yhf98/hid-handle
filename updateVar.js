/*
 * @Author: yaohengfeng 1921934563@qq.com
 * @Date: 2023-01-13 10:53:22
 * @LastEditors: 姚恒锋 1921934563@qq.com
 * @LastEditTime: 2023-07-25 14:01:16
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
    let num = Math.random() * 100;
    let data = [
      {
          "obj_id": 1,
          "obj_type": 3,
          "obj_name": "网络",
          "obj_x": 0,
          "obj_y": 0,
          "obj_w": 960,
          "obj_h": 320,
          "obj_opa": 255,
          "obj_r": 0,
          "obj_g": 0,
          "obj_b": 0,
          "obj_opa_2": 255,
          "obj_r_2": 0,
          "obj_g_2": 0,
          "obj_b_2": 0,
          "obj_font_size": 20,
          "obj_font_r": 0,
          "obj_font_g": 0,
          "obj_font_b": 0,
          "obj_range_min": 0,
          "obj_range_max": 100,
          "obj_angle_range": 0,
          "obj_rotation": 0,
          "obj_width": 960,
          "obj_point_x_num": 5,
          "obj_point_y_num": 5,
          "obj_time": 500,
          "obj_var": [
            num
          ],
          "obj_event": 0,
          "obj_action": 0,
          "obj_data": String(num),
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
}, 100)