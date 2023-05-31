/*
 * @Author: 姚恒锋 1921934563@qq.com
 * @Date: 2023-05-31 15:40:04
 * @LastEditors: 姚恒锋 1921934563@qq.com
 * @LastEditTime: 2023-05-31 16:08:00
 * @FilePath: \hid-handle\generate_ui_img.js
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
const hid = require("./hidhandle.js");

let data = [
    {
        "obj_id": 10,
        "obj_type": 2,
        "obj_name": "cpu",
        "obj_x": 0,
        "obj_y": 0,
        "obj_w": 480,
        "obj_h": 320,
        "obj_opa": 255,
        "obj_r": 0,
        "obj_g": 0,
        "obj_b": 0,
        "obj_opa_2": 255,
        "obj_r_2": 0,
        "obj_g_2": 0,
        "obj_b_2": 0,
        "obj_font_size": 0,
        "obj_font_r": 0,
        "obj_font_g": 0,
        "obj_font_b": 0,
        "obj_range_min": 0,
        "obj_range_max": 100,
        "obj_angle_range": 0,
        "obj_rotation": 0,
        "obj_width": 480,
        "obj_point_x_num": 5,
        "obj_point_y_num": 5,
        "obj_time": 1000,
        "obj_var": [
            0
        ],
        "obj_event": 0,
        "obj_action": 0,
        "obj_data": "cpu.png",
        "obj_reserve": ""
    },
    {
        "obj_id": 9,
        "obj_type": 1,
        "obj_name": "CPU-Num",
        "obj_x": 80,
        "obj_y": 160,
        "obj_w": 80,
        "obj_h": 60,
        "obj_opa": 255,
        "obj_r": 250,
        "obj_g": 0,
        "obj_b": 0,
        "obj_opa_2": 255,
        "obj_r_2": 0,
        "obj_g_2": 0,
        "obj_b_2": 0,
        "obj_font_size": 80,
        "obj_font_r": 0,
        "obj_font_g": 0,
        "obj_font_b": 0,
        "obj_range_min": 0,
        "obj_range_max": 100,
        "obj_angle_range": 0,
        "obj_rotation": 0,
        "obj_width": 80,
        "obj_point_x_num": 5,
        "obj_point_y_num": 5,
        "obj_time": 1000,
        "obj_var": [
            16
        ],
        "obj_event": 0,
        "obj_action": 0,
        "obj_data": "31",
        "obj_reserve": ""
    }
]


let hidHandle = hid.HidHandle();

console.info("handle: ", hidHandle);

// 默认UI元素
// console.info("HID", hidHandle.hmi_create_obj_test_handle())

// 生成UI文件
hidHandle.generate_ui_handle(data, "./new_ui");
// console.info("更新UI：", hidHandle.hid_write_file_handle('./new_ui/hbin/hmi_res.hbin', 'hmi_res.hbin', 9))
