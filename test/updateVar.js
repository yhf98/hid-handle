/*
 * @Author: yaohengfeng 1921934563@qq.com
 * @Date: 2023-01-13 10:53:22
 * @LastEditors: 姚恒锋 1921934563@qq.com
 * @LastEditTime: 2023-08-30 13:53:45
 * @FilePath: \hid-handle\test.js
 * @Description: test.js
 */
const random = require('string-random');
const hid = require("../main.js");

let count = 0;
setInterval(() => {


  hid.HidHandle().then(fn => {
    let num = parseInt(Math.random() * 5);
    let data = [
      {
          "obj_id": 1,
          "obj_type": 11,
          "obj_name": "untitled0",
          "obj_wifi_name": "jieshen",
          "obj_wifi_pass": "Jieshen168",
          "obj_x": 200,
          "obj_y": 80,
          "obj_w": 360,
          "obj_h": 20,
          "obj_opa": 0,
          "obj_r": 0,
          "obj_g": 0,
          "obj_b": 0,
          "obj_opa_2": 0,
          "obj_r_2": 0,
          "obj_g_2": 0,
          "obj_b_2": 0,
          "obj_opa_3": 255,
          "obj_r_3": 0,
          "obj_g_3": 0,
          "obj_b_3": 0,
          "obj_font_name": "",
          "obj_font_size": 20,
          "obj_font_r": 0,
          "obj_font_g": 0,
          "obj_font_b": 0,
          "obj_val_flag": 0,
          "obj_rise_r": 0,
          "obj_rise_g": 0,
          "obj_rise_b": 0,
          "obj_fall_r": 0,
          "obj_fall_g": 0,
          "obj_fall_b": 0,
          "obj_range_min": 0,
          "obj_range_max": 30,
          "obj_angle_range": 0,
          "obj_rotation": 0,
          "obj_width": 360,
          "obj_progress_interval": 5,
          "obj_point_x_num": 5,
          "obj_point_y_num": 5,
          "obj_time": 3000,
          "obj_var": [
              num
          ],
          "obj_event": 0,
          "obj_action": 0,
          "obj_data": String(num),
          "obj_reserve": "",
          "obj_url": "",
          "obj_field": "",
          "obj_align_reserve": "",
          "obj_rule_name": [
              "1",
              "2",
              "3",
              "4",
              "5"
          ],
          "obj_progress_name": [
              "disk1.png",
              "disk2.png",
              "disk3.png",
              "disk4.png",
              "disk5.png"
          ]
      }
  ]
    fn.hmi_batch_update_screen_data(data)
    count++
    console.info(`==================执行了【${num}】=======================次`)
  })

}, 2000)