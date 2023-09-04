/*
 * @Author: 姚恒锋 1921934563@qq.com
 * @Date: 2023-07-25 16:58:03
 * @LastEditors: 姚恒锋 1921934563@qq.com
 * @LastEditTime: 2023-08-30 17:01:26
 * @FilePath: \hid-handle\test\generate_ui.js
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
const hid = require("../main.js");

let data =[
    {
        "obj_id": 2,
        "obj_type": 11,
        "obj_name": "untitled1",
        "obj_wifi_name": "jieshen",
        "obj_wifi_pass": "Jieshen168",
        "obj_x": 200,
        "obj_y": 120,
        "obj_w": 280,
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
        "obj_width": 280,
        "obj_progress_interval": 9,
        "obj_point_x_num": 5,
        "obj_point_y_num": 5,
        "obj_time": 3000,
        "obj_var": [
            5
        ],
        "obj_event": 4,
        "obj_action": 0,
        "obj_data": "5",
        "obj_reserve": "",
        "obj_url": "",
        "obj_field": "",
        "obj_align_reserve": "",
        "obj_rule_name": [
            "1",
            "2",
            "3",
            "4",
            "5",
            "7",
            "8",
            "9",
            "10"
        ],
        "obj_progress_name": [
            "disk1.png",
            "disk2.png",
            "disk3.png",
            "disk4.png",
            "disk5.png",
            "disk7.png",
            "disk8.png",
            "disk9.png",
            "disk10.png"
        ]
    }
]
hid.HidHandle().then(fn => {
    // 生成UI文件
    console.info("开--始：====", fn);
    console.info("生-成：", fn.generate_ui_handle(data, "H:/reminder-vue2/node_modules/electron/dist", 'GalaxySZ', 'galaxy168'));
    console.info("更新-UI：", fn.hid_write_file_handle('H:/reminder-vue2/node_modules/electron/dist/hbin/hmi_res.hbin', 'hmi_res.hbin', 9));
})