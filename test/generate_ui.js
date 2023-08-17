/*
 * @Author: 姚恒锋 1921934563@qq.com
 * @Date: 2023-07-25 16:58:03
 * @LastEditors: 姚恒锋 1921934563@qq.com
 * @LastEditTime: 2023-08-17 10:55:34
 * @FilePath: \hid-handle\test\generate_ui.js
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
const hid = require("../main.js");

let data =[
    {
        "obj_id": 1,
        "obj_type": 11,
        "obj_name": "untitled0",
        "obj_wifi_name": "jieshen",
        "obj_wifi_pass": "Jieshen168",
        "obj_x": 240,
        "obj_y": 140,
        "obj_w": 240,
        "obj_h": 100,
        "obj_opa": 255,
        "obj_r": 0,
        "obj_g": 0,
        "obj_b": 0,
        "obj_opa_2": 255,
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
        "obj_range_max": 100,
        "obj_angle_range": 0,
        "obj_rotation": 0,
        "obj_width": 240,
        "obj_progress_interval": 10,
        "obj_progress_name": [
            "fan1.png",
            "fan2.png",
            "fan3.png",
            "fan4.png",
            "fan5.png",
            "fan6.png",
            "fan7.png",
            "fan8.png",
            "fan9.png",
            "fan10.png"
        ],
        "obj_point_x_num": 5,
        "obj_point_y_num": 5,
        "obj_time": 5000,
        "obj_var": [
            25
        ],
        "obj_event": 0,
        "obj_action": 0,
        "obj_data": "25",
        "obj_reserve": "",
        "obj_url": "",
        "obj_field": "",
        "obj_align_reserve": ""
    }
]

hid.HidHandle().then(fn => {
    // 生成UI文件
    console.info("开--始：====", fn);
    console.info("生-成：", fn.generate_ui_handle(data, "H:/reminder-vue2/node_modules/electron/dist", 'jieshen', 'Jieshen168'));
    console.info("更新-UI：", fn.hid_write_file_handle('H:/reminder-vue2/node_modules/electron/dist/hbin/hmi_res.hbin', 'hmi_res.hbin', 9))
})