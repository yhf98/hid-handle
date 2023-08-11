/*
 * @Author: 姚恒锋 1921934563@qq.com
 * @Date: 2023-07-25 16:58:03
 * @LastEditors: 姚恒锋 1921934563@qq.com
 * @LastEditTime: 2023-08-10 18:13:20
 * @FilePath: \hid-handle\test\generate_ui.js
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
const hid = require("../main.js");

let data = [
    {
        "obj_id": 0,
        "obj_type": 1,
        "obj_name": "untitled0",
        "obj_x": 300,
        "obj_y": 60,
        "obj_w": 100,
        "obj_h": 40,
        "obj_opa": 255,
        "obj_r": 0,
        "obj_g": 0,
        "obj_b": 0,
        "obj_opa_2": 100,
        "obj_r_2": 159,
        "obj_g_2": 28,
        "obj_b_2": 28,
        "obj_font_size": 20,
        "obj_font_r": 247,
        "obj_font_g": 4,
        "obj_font_b": 4,
        "obj_range_min": 0,
        "obj_range_max": 0,
        "obj_angle_range": 0,
        "obj_rotation": 0,
        "obj_width": 100,
        "obj_point_x_num": 5,
        "obj_point_y_num": 5,
        "obj_time": 5000,
        "obj_var": [
            0
        ],
        "obj_event": 4,
        "obj_action": 0,
        "obj_data": "+0.630",
        "obj_reserve": "",
        "obj_url": "https://stock-f-reminr-server-aybofoxbug.us-west-1.fcapp.run?BABA",
        "obj_field": "increase",
        "obj_align_reserve": "",
        "obj_font_name": "ya.ttf",
        "obj_rise_r": 0,
        "obj_rise_g": 255,
        "obj_rise_b": 0,
        "obj_fall_r": 255,
        "obj_fall_g": 0,
        "obj_fall_b": 0
    },
    {
        "obj_id": 1,
        "obj_type": 1,
        "obj_name": "untitled1",
        "obj_x": 60,
        "obj_y": 80,
        "obj_w": 120,
        "obj_h": 60,
        "obj_opa": 255,
        "obj_r": 0,
        "obj_g": 0,
        "obj_b": 0,
        "obj_opa_2": 100,
        "obj_r_2": 248,
        "obj_g_2": 0,
        "obj_b_2": 0,
        "obj_font_size": 30,
        "obj_font_r": 14,
        "obj_font_g": 90,
        "obj_font_b": 255,
        "obj_range_min": 0,
        "obj_range_max": 0,
        "obj_angle_range": 0,
        "obj_rotation": 0,
        "obj_width": 120,
        "obj_point_x_num": 5,
        "obj_point_y_num": 5,
        "obj_time": 0,
        "obj_var": [
            0
        ],
        "obj_event": 0,
        "obj_action": 0,
        "obj_data": "哈哈哈",
        "obj_reserve": "",
        "obj_url": "",
        "obj_field": "",
        "obj_align_reserve": "",
        "obj_font_name": "ya.ttf",
        "obj_rise_r": 0,
        "obj_rise_g": 0,
        "obj_rise_b": 0,
        "obj_fall_r": 0,
        "obj_fall_g": 0,
        "obj_fall_b": 0
    }
]

hid.HidHandle().then(fn => {
    // 生成UI文件
    console.info("开始：====", fn);
    console.info("生成：", fn.generate_ui_handle(data, "./ui", 'jieshen', 'Jieshen168'));
    console.info("更新UI：", fn.hid_write_file_handle('./ui/hbin/hmi_res.hbin', 'hmi_res.hbin', 9))
})