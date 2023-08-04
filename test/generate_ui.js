/*
 * @Author: 姚恒锋 1921934563@qq.com
 * @Date: 2023-07-25 16:58:03
 * @LastEditors: 姚恒锋 1921934563@qq.com
 * @LastEditTime: 2023-08-01 14:27:04
 * @FilePath: \hid-handle\test\generate_ui.js
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
const hid = require("../main.js");

let data = [
    {
        "obj_id": 0,
        "obj_type": 1,
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
        "obj_time": 0,
        "obj_var": [
            0
        ],
        "obj_event": 0,
        "obj_action": 0,
        "obj_data": "测试",
        "obj_reserve": "",
        "obj_url": "",
        "obj_field": "",
        "obj_align_reserve": ""
    },
]

hid.HidHandle().then(fn => {
    // 生成UI文件
    fn.generate_ui_handle(data, "./ui")
    console.info("更新UI：", fn.hid_write_file_handle('./ui/hbin/hmi_res.hbin', 'hmi_res.hbin', 9))
})
