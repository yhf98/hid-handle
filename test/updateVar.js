/*
 * @Author: yaohengfeng 1921934563@qq.com
 * @Date: 2023-01-13 10:53:22
 * @LastEditors: 姚恒锋 1921934563@qq.com
 * @LastEditTime: 2023-07-26 16:52:08
 * @FilePath: \hid-handle\test.js
 * @Description: test.js
 */
const random = require('string-random');
const hid = require("../main.js");

let count = 0;
setInterval(() => {


  hid.HidHandle().then(fn => {
    let num = parseInt(Math.random() * 100);
    let data = [
      {
        "obj_id": 8,
        "obj_type": 6,
        "obj_name": "untitled8",
        "obj_x": 120,
        "obj_y": 60,
        "obj_w": 360,
        "obj_h": 25,
        "obj_opa": 255,

        "obj_r": 255,
        "obj_g": 0,
        "obj_b": 0,

        "obj_opa_2": 100,

        "obj_r_2": 255,
        "obj_g_2": 0,
        "obj_b_2": 0,

        "obj_font_size": 20,

        "obj_font_r": 0,
        "obj_font_g": 0,
        "obj_font_b": 100,
        
        "obj_range_min": 0,
        "obj_range_max": 50,
        "obj_angle_range": 0,
        "obj_rotation": 0,
        "obj_width": 360,
        "obj_point_x_num": 5,
        "obj_point_y_num": 5,
        "obj_time": 500,
          "obj_var": [
              num
          ],
          "obj_event": 0,
          "obj_action": 0,
          "obj_data": String(num) + "%",
          "obj_reserve": "",
          "obj_url": "",
          "obj_field": "",
          "obj_align_reserve": ""
      }
  ]
    fn.hmi_batch_update_screen_data(data)
    count++
    console.info(`==================执行了【${num}】=======================次`)
  })

}, 1000)