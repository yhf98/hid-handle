const hid = require("./hidhandle.js");

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
        "obj_data": "",
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
        "obj_data": "YYYY",
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
