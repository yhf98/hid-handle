const hid = require("./hidhandle.js");

// let data = [
//     {
//         "obj_id": 21,
//         "obj_type": 1,
//         "obj_name": "test",
//         "obj_x": 280,
//         "obj_y": 40,
//         "obj_w": 80,
//         "obj_h": 40,
//         "obj_opa": 255,
//         "obj_r": 81,
//         "obj_g": 255,
//         "obj_b": 0,
//         "obj_opa_2": 255,
//         "obj_r_2": 0,
//         "obj_g_2": 0,
//         "obj_b_2": 0,
//         "obj_font_size": 18,
//         "obj_font_r": 255,
//         "obj_font_g": 0,
//         "obj_font_b": 0,
//         "obj_range_min": 0,
//         "obj_range_max": 100,
//         "obj_angle_range": 0,
//         "obj_rotation": 0,
//         "obj_width": 80,
//         "obj_point_x_num": 5,
//         "obj_point_y_num": 5,
//         "obj_time": 1000,
//         "obj_var": [
//             0
//         ],
//         "obj_event": 0,
//         "obj_action": 0,
//         "obj_data": "提醒器",
//         "obj_reserve": ""
//     },
//     {
//         "obj_id": 23,
//         "obj_type": 2,
//         "obj_name": "img2",
//         "obj_x": 740,
//         "obj_y": 0,
//         "obj_w": 220,
//         "obj_h": 200,
//         "obj_opa": 255,
//         "obj_r": 0,
//         "obj_g": 0,
//         "obj_b": 0,
//         "obj_opa_2": 255,
//         "obj_r_2": 0,
//         "obj_g_2": 0,
//         "obj_b_2": 0,
//         "obj_font_size": 0,
//         "obj_font_r": 0,
//         "obj_font_g": 0,
//         "obj_font_b": 0,
//         "obj_range_min": 0,
//         "obj_range_max": 100,
//         "obj_angle_range": 0,
//         "obj_rotation": 0,
//         "obj_width": 220,
//         "obj_point_x_num": 5,
//         "obj_point_y_num": 5,
//         "obj_time": 1000,
//         "obj_var": [
//             0
//         ],
//         "obj_event": 0,
//         "obj_action": 0,
//         "obj_data": "gg.png",
//         "obj_reserve": ""
//     },
//     {
//         "obj_id": 25,
//         "obj_type": 1,
//         "obj_name": "fan",
//         "obj_x": 120,
//         "obj_y": 40,
//         "obj_w": 120,
//         "obj_h": 40,
//         "obj_opa": 255,
//         "obj_r": 0,
//         "obj_g": 0,
//         "obj_b": 0,
//         "obj_opa_2": 255,
//         "obj_r_2": 0,
//         "obj_g_2": 0,
//         "obj_b_2": 0,
//         "obj_font_size": 16,
//         "obj_font_r": 255,
//         "obj_font_g": 0,
//         "obj_font_b": 0,
//         "obj_range_min": 0,
//         "obj_range_max": 100,
//         "obj_angle_range": 0,
//         "obj_rotation": 0,
//         "obj_width": 120,
//         "obj_point_x_num": 5,
//         "obj_point_y_num": 5,
//         "obj_time": 1000,
//         "obj_var": [
//             4807196
//         ],
//         "obj_event": 0,
//         "obj_action": 0,
//         "obj_data": "4807196",
//         "obj_reserve": ""
//     },
//     {
//         "obj_id": 26,
//         "obj_type": 1,
//         "obj_name": "ftxt",
//         "obj_x": 0,
//         "obj_y": 40,
//         "obj_w": 120,
//         "obj_h": 40,
//         "obj_opa": 255,
//         "obj_r": 0,
//         "obj_g": 0,
//         "obj_b": 0,
//         "obj_opa_2": 255,
//         "obj_r_2": 0,
//         "obj_g_2": 0,
//         "obj_b_2": 0,
//         "obj_font_size": 16,
//         "obj_font_r": 255,
//         "obj_font_g": 0,
//         "obj_font_b": 0,
//         "obj_range_min": 0,
//         "obj_range_max": 100,
//         "obj_angle_range": 0,
//         "obj_rotation": 0,
//         "obj_width": 120,
//         "obj_point_x_num": 5,
//         "obj_point_y_num": 5,
//         "obj_time": 1000,
//         "obj_var": [
//             0
//         ],
//         "obj_event": 0,
//         "obj_action": 0,
//         "obj_data": "粉丝数：",
//         "obj_reserve": ""
//     },
//     {
//         "obj_id": 27,
//         "obj_type": 1,
//         "obj_name": "currency",
//         "obj_x": 120,
//         "obj_y": 80,
//         "obj_w": 120,
//         "obj_h": 40,
//         "obj_opa": 255,
//         "obj_r": 0,
//         "obj_g": 0,
//         "obj_b": 0,
//         "obj_opa_2": 255,
//         "obj_r_2": 0,
//         "obj_g_2": 0,
//         "obj_b_2": 0,
//         "obj_font_size": 16,
//         "obj_font_r": 0,
//         "obj_font_g": 255,
//         "obj_font_b": 42,
//         "obj_range_min": 0,
//         "obj_range_max": 100,
//         "obj_angle_range": 0,
//         "obj_rotation": 0,
//         "obj_width": 120,
//         "obj_point_x_num": 5,
//         "obj_point_y_num": 5,
//         "obj_time": 1000,
//         "obj_var": [
//             23.267
//         ],
//         "obj_event": 0,
//         "obj_action": 0,
//         "obj_data": "23.267",
//         "obj_reserve": ""
//     },
//     {
//         "obj_id": 28,
//         "obj_type": 1,
//         "obj_name": "cu",
//         "obj_x": 0,
//         "obj_y": 80,
//         "obj_w": 120,
//         "obj_h": 40,
//         "obj_opa": 255,
//         "obj_r": 0,
//         "obj_g": 0,
//         "obj_b": 0,
//         "obj_opa_2": 255,
//         "obj_r_2": 0,
//         "obj_g_2": 0,
//         "obj_b_2": 0,
//         "obj_font_size": 16,
//         "obj_font_r": 81,
//         "obj_font_g": 255,
//         "obj_font_b": 0,
//         "obj_range_min": 0,
//         "obj_range_max": 100,
//         "obj_angle_range": 0,
//         "obj_rotation": 0,
//         "obj_width": 120,
//         "obj_point_x_num": 5,
//         "obj_point_y_num": 5,
//         "obj_time": 1000,
//         "obj_var": [
//             0
//         ],
//         "obj_event": 0,
//         "obj_action": 0,
//         "obj_data": "比特币价格",
//         "obj_reserve": ""
//     },
//     {
//         "obj_id": 29,
//         "obj_type": 1,
//         "obj_name": "play",
//         "obj_x": 120,
//         "obj_y": 120,
//         "obj_w": 120,
//         "obj_h": 40,
//         "obj_opa": 255,
//         "obj_r": 0,
//         "obj_g": 0,
//         "obj_b": 0,
//         "obj_opa_2": 255,
//         "obj_r_2": 0,
//         "obj_g_2": 0,
//         "obj_b_2": 0,
//         "obj_font_size": 16,
//         "obj_font_r": 255,
//         "obj_font_g": 0,
//         "obj_font_b": 212,
//         "obj_range_min": 0,
//         "obj_range_max": 100,
//         "obj_angle_range": 0,
//         "obj_rotation": 0,
//         "obj_width": 120,
//         "obj_point_x_num": 5,
//         "obj_point_y_num": 5,
//         "obj_time": 1000,
//         "obj_var": [
//             320857
//         ],
//         "obj_event": 0,
//         "obj_action": 0,
//         "obj_data": "320857",
//         "obj_reserve": ""
//     },
//     {
//         "obj_id": 30,
//         "obj_type": 1,
//         "obj_name": "pp",
//         "obj_x": 0,
//         "obj_y": 120,
//         "obj_w": 120,
//         "obj_h": 40,
//         "obj_opa": 255,
//         "obj_r": 0,
//         "obj_g": 0,
//         "obj_b": 0,
//         "obj_opa_2": 255,
//         "obj_r_2": 0,
//         "obj_g_2": 0,
//         "obj_b_2": 0,
//         "obj_font_size": 16,
//         "obj_font_r": 251,
//         "obj_font_g": 0,
//         "obj_font_b": 255,
//         "obj_range_min": 0,
//         "obj_range_max": 100,
//         "obj_angle_range": 0,
//         "obj_rotation": 0,
//         "obj_width": 120,
//         "obj_point_x_num": 5,
//         "obj_point_y_num": 5,
//         "obj_time": 1000,
//         "obj_var": [
//             0
//         ],
//         "obj_event": 0,
//         "obj_action": 0,
//         "obj_data": "Youtbu播放量",
//         "obj_reserve": ""
//     },
//     {
//         "obj_id": 8,
//         "obj_type": 1,
//         "obj_name": "gogo",
//         "obj_x": 120,
//         "obj_y": 160,
//         "obj_w": 120,
//         "obj_h": 30,
//         "obj_opa": 255,
//         "obj_r": 0,
//         "obj_g": 0,
//         "obj_b": 0,
//         "obj_opa_2": 255,
//         "obj_r_2": 0,
//         "obj_g_2": 0,
//         "obj_b_2": 0,
//         "obj_font_size": 18,
//         "obj_font_r": 255,
//         "obj_font_g": 0,
//         "obj_font_b": 0,
//         "obj_range_min": 0,
//         "obj_range_max": 100,
//         "obj_angle_range": 0,
//         "obj_rotation": 0,
//         "obj_width": 120,
//         "obj_point_x_num": 5,
//         "obj_point_y_num": 5,
//         "obj_time": 1000,
//         "obj_var": [
//             5000
//         ],
//         "obj_event": 0,
//         "obj_action": 0,
//         "obj_data": "5000",
//         "obj_reserve": ""
//     },
//     {
//         "obj_id": 9,
//         "obj_type": 1,
//         "obj_name": "cou",
//         "obj_x": 0,
//         "obj_y": 160,
//         "obj_w": 120,
//         "obj_h": 30,
//         "obj_opa": 255,
//         "obj_r": 0,
//         "obj_g": 0,
//         "obj_b": 0,
//         "obj_opa_2": 255,
//         "obj_r_2": 0,
//         "obj_g_2": 0,
//         "obj_b_2": 0,
//         "obj_font_size": 16,
//         "obj_font_r": 0,
//         "obj_font_g": 191,
//         "obj_font_b": 255,
//         "obj_range_min": 0,
//         "obj_range_max": 100,
//         "obj_angle_range": 0,
//         "obj_rotation": 0,
//         "obj_width": 120,
//         "obj_point_x_num": 5,
//         "obj_point_y_num": 5,
//         "obj_time": 1000,
//         "obj_var": [
//             0
//         ],
//         "obj_event": 0,
//         "obj_action": 0,
//         "obj_data": "众筹金额",
//         "obj_reserve": ""
//     }
// ]

// let data = [
//     {
//         "obj_id": 30,
//         "obj_type": 1,
//         "obj_name": "namess",
//         "obj_x": 640,
//         "obj_y": 0,
//         "obj_w": 220,
//         "obj_h": 140,
//         "obj_opa": 255,
//         "obj_r": 0,
//         "obj_g": 0,
//         "obj_b": 0,
//         "obj_opa_2": 255,
//         "obj_r_2": 0,
//         "obj_g_2": 0,
//         "obj_b_2": 0,
//         "obj_font_size": 100,
//         "obj_font_r": 41,
//         "obj_font_g": 245,
//         "obj_font_b": 0,
//         "obj_range_min": 0,
//         "obj_range_max": 100,
//         "obj_angle_range": 0,
//         "obj_rotation": 0,
//         "obj_width": 220,
//         "obj_point_x_num": 5,
//         "obj_point_y_num": 5,
//         "obj_time": 0,
//         "obj_var": [
//             0
//         ],
//         "obj_event": 0,
//         "obj_action": 0,
//         "obj_data": "测试",
//         "obj_reserve": "",
//         "obj_url": "",
//         "obj_field": "",
//         "obj_align_reserve": ""
//     },
//     {
//         "obj_id": 33,
//         "obj_type": 1,
//         "obj_name": "fan",
//         "obj_x": 220,
//         "obj_y": 100,
//         "obj_w": 140,
//         "obj_h": 40,
//         "obj_opa": 255,
//         "obj_r": 0,
//         "obj_g": 0,
//         "obj_b": 0,
//         "obj_opa_2": 255,
//         "obj_r_2": 0,
//         "obj_g_2": 0,
//         "obj_b_2": 0,
//         "obj_font_size": 30,
//         "obj_font_r": 43,
//         "obj_font_g": 255,
//         "obj_font_b": 0,
//         "obj_range_min": 0,
//         "obj_range_max": 100,
//         "obj_angle_range": 0,
//         "obj_rotation": 0,
//         "obj_width": 140,
//         "obj_point_x_num": 5,
//         "obj_point_y_num": 5,
//         "obj_time": 1000,
//         "obj_var": [
//             1000
//         ],
//         "obj_event": 0,
//         "obj_action": 0,
//         "obj_data": "1000",
//         "obj_reserve": "",
//         "obj_url": "https://bilibili-user-reminder-server-urdotpiowj.cn-hangzhou.fcapp.run?525438321",
//         "obj_field": "follower",
//         "obj_align_reserve": ""
//     },
//     {
//         "obj_id": 33,
//         "obj_type": 1,
//         "obj_name": "fant",
//         "obj_x": 120,
//         "obj_y": 100,
//         "obj_w": 100,
//         "obj_h": 40,
//         "obj_opa": 255,
//         "obj_r": 0,
//         "obj_g": 0,
//         "obj_b": 0,
//         "obj_opa_2": 255,
//         "obj_r_2": 0,
//         "obj_g_2": 0,
//         "obj_b_2": 0,
//         "obj_font_size": 30,
//         "obj_font_r": 0,
//         "obj_font_g": 255,
//         "obj_font_b": 89,
//         "obj_range_min": 0,
//         "obj_range_max": 100,
//         "obj_angle_range": 0,
//         "obj_rotation": 0,
//         "obj_width": 100,
//         "obj_point_x_num": 5,
//         "obj_point_y_num": 5,
//         "obj_time": 0,
//         "obj_var": [
//             0
//         ],
//         "obj_event": 0,
//         "obj_action": 0,
//         "obj_data": "粉丝：",
//         "obj_reserve": "",
//         "obj_url": "",
//         "obj_field": "",
//         "obj_align_reserve": ""
//     }
// ]

// let data = [{ "obj_id": 30, "obj_type": 1, "obj_name": "namess", "obj_x": 640, "obj_y": 0, "obj_w": 220, "obj_h": 140, "obj_opa": 255, "obj_r": 0, "obj_g": 0, "obj_b": 0, "obj_opa_2": 255, "obj_r_2": 0, "obj_g_2": 0, "obj_b_2": 0, "obj_font_size": 100, "obj_font_r": 41, "obj_font_g": 245, "obj_font_b": 0, "obj_range_min": 0, "obj_range_max": 100, "obj_angle_range": 0, "obj_rotation": 0, "obj_width": 220, "obj_point_x_num": 5, "obj_point_y_num": 5, "obj_time": 0, "obj_var": [0], "obj_event": 0, "obj_action": 0, "obj_data": "测试", "obj_reserve": "", "obj_url": "", "obj_field": "", "obj_align_reserve": "" }, { "obj_id": 33, "obj_type": 1, "obj_name": "fan", "obj_x": 220, "obj_y": 100, "obj_w": 140, "obj_h": 40, "obj_opa": 255, "obj_r": 0, "obj_g": 0, "obj_b": 0, "obj_opa_2": 255, "obj_r_2": 0, "obj_g_2": 0, "obj_b_2": 0, "obj_font_size": 30, "obj_font_r": 43, "obj_font_g": 255, "obj_font_b": 0, "obj_range_min": 0, "obj_range_max": 100, "obj_angle_range": 0, "obj_rotation": 0, "obj_width": 140, "obj_point_x_num": 5, "obj_point_y_num": 5, "obj_time": 5000, "obj_var": [400], "obj_event": 0, "obj_action": 0, "obj_data": "400", "obj_reserve": "", "obj_url": "https://bilibili-user-reminder-server-urdotpiowj.cn-hangzhou.fcapp.run?525438321", "obj_field": "follower", "obj_align_reserve": "" }, { "obj_id": 33, "obj_type": 1, "obj_name": "fant", "obj_x": 120, "obj_y": 100, "obj_w": 100, "obj_h": 40, "obj_opa": 255, "obj_r": 0, "obj_g": 0, "obj_b": 0, "obj_opa_2": 255, "obj_r_2": 0, "obj_g_2": 0, "obj_b_2": 0, "obj_font_size": 30, "obj_font_r": 0, "obj_font_g": 255, "obj_font_b": 89, "obj_range_min": 0, "obj_range_max": 100, "obj_angle_range": 0, "obj_rotation": 0, "obj_width": 100, "obj_point_x_num": 5, "obj_point_y_num": 5, "obj_time": 0, "obj_var": [0], "obj_event": 0, "obj_action": 0, "obj_data": "粉丝：", "obj_reserve": "", "obj_url": "", "obj_field": "", "obj_align_reserve": "" }, { "obj_id": 34, "obj_type": 1, "obj_name": "likes", "obj_x": 220, "obj_y": 160, "obj_w": 140, "obj_h": 50, "obj_opa": 255, "obj_r": 0, "obj_g": 0, "obj_b": 0, "obj_opa_2": 255, "obj_r_2": 0, "obj_g_2": 0, "obj_b_2": 0, "obj_font_size": 30, "obj_font_r": 249, "obj_font_g": 1, "obj_font_b": 1, "obj_range_min": 0, "obj_range_max": 100, "obj_angle_range": 0, "obj_rotation": 0, "obj_width": 140, "obj_point_x_num": 5, "obj_point_y_num": 5, "obj_time": 5000, "obj_var": [200], "obj_event": 0, "obj_action": 0, "obj_data": "200", "obj_reserve": "", "obj_url": "https://bilibili-video-reminder-server-wksyumnnvk.cn-hangzhou.fcapp.run?BV15u411x7nj", "obj_field": "like", "obj_align_reserve": "" }, { "obj_id": 35, "obj_type": 1, "obj_name": "slike", "obj_x": 120, "obj_y": 160, "obj_w": 100, "obj_h": 50, "obj_opa": 255, "obj_r": 0, "obj_g": 0, "obj_b": 0, "obj_opa_2": 255, "obj_r_2": 0, "obj_g_2": 0, "obj_b_2": 0, "obj_font_size": 30, "obj_font_r": 255, "obj_font_g": 0, "obj_font_b": 0, "obj_range_min": 0, "obj_range_max": 100, "obj_angle_range": 0, "obj_rotation": 0, "obj_width": 100, "obj_point_x_num": 5, "obj_point_y_num": 5, "obj_time": 0, "obj_var": [0], "obj_event": 0, "obj_action": 0, "obj_data": "点赞：", "obj_reserve": "", "obj_url": "", "obj_field": "", "obj_align_reserve": "" }]

// let data = [{"obj_id":30,"obj_type":1,"obj_name":"namess","obj_x":640,"obj_y":0,"obj_w":220,"obj_h":140,"obj_opa":255,"obj_r":0,"obj_g":0,"obj_b":0,"obj_opa_2":255,"obj_r_2":0,"obj_g_2":0,"obj_b_2":0,"obj_font_size":100,"obj_font_r":41,"obj_font_g":245,"obj_font_b":0,"obj_range_min":0,"obj_range_max":100,"obj_angle_range":0,"obj_rotation":0,"obj_width":220,"obj_point_x_num":5,"obj_point_y_num":5,"obj_time":0,"obj_var":[0],"obj_event":0,"obj_action":0,"obj_data":"测试","obj_reserve":"","obj_url":"","obj_field":"","obj_align_reserve":""},{"obj_id":33,"obj_type":1,"obj_name":"fan","obj_x":180,"obj_y":40,"obj_w":140,"obj_h":40,"obj_opa":255,"obj_r":0,"obj_g":0,"obj_b":0,"obj_opa_2":255,"obj_r_2":0,"obj_g_2":0,"obj_b_2":0,"obj_font_size":30,"obj_font_r":43,"obj_font_g":255,"obj_font_b":0,"obj_range_min":0,"obj_range_max":100,"obj_angle_range":0,"obj_rotation":0,"obj_width":140,"obj_point_x_num":5,"obj_point_y_num":5,"obj_time":5000,"obj_var":[400],"obj_event":0,"obj_action":0,"obj_data":"400","obj_reserve":"","obj_url":"https://bilibili-user-reminder-server-urdotpiowj.cn-hangzhou.fcapp.run?525438321","obj_field":"follower","obj_align_reserve":""},{"obj_id":33,"obj_type":1,"obj_name":"fant","obj_x":60,"obj_y":40,"obj_w":100,"obj_h":40,"obj_opa":255,"obj_r":0,"obj_g":0,"obj_b":0,"obj_opa_2":255,"obj_r_2":0,"obj_g_2":0,"obj_b_2":0,"obj_font_size":30,"obj_font_r":0,"obj_font_g":255,"obj_font_b":89,"obj_range_min":0,"obj_range_max":100,"obj_angle_range":0,"obj_rotation":0,"obj_width":100,"obj_point_x_num":5,"obj_point_y_num":5,"obj_time":0,"obj_var":[0],"obj_event":0,"obj_action":0,"obj_data":"粉丝：","obj_reserve":"","obj_url":"","obj_field":"","obj_align_reserve":""},{"obj_id":34,"obj_type":1,"obj_name":"likes","obj_x":160,"obj_y":160,"obj_w":140,"obj_h":50,"obj_opa":255,"obj_r":0,"obj_g":0,"obj_b":0,"obj_opa_2":255,"obj_r_2":0,"obj_g_2":0,"obj_b_2":0,"obj_font_size":30,"obj_font_r":249,"obj_font_g":1,"obj_font_b":1,"obj_range_min":0,"obj_range_max":100,"obj_angle_range":0,"obj_rotation":0,"obj_width":140,"obj_point_x_num":5,"obj_point_y_num":5,"obj_time":5000,"obj_var":[200],"obj_event":0,"obj_action":0,"obj_data":"200","obj_reserve":"","obj_url":"https://bilibili-video-reminder-server-wksyumnnvk.cn-hangzhou.fcapp.run?BV15u411x7nj","obj_field":"like","obj_align_reserve":""},{"obj_id":35,"obj_type":1,"obj_name":"slike","obj_x":60,"obj_y":160,"obj_w":100,"obj_h":50,"obj_opa":255,"obj_r":0,"obj_g":0,"obj_b":0,"obj_opa_2":255,"obj_r_2":0,"obj_g_2":0,"obj_b_2":0,"obj_font_size":30,"obj_font_r":255,"obj_font_g":0,"obj_font_b":0,"obj_range_min":0,"obj_range_max":100,"obj_angle_range":0,"obj_rotation":0,"obj_width":100,"obj_point_x_num":5,"obj_point_y_num":5,"obj_time":0,"obj_var":[0],"obj_event":0,"obj_action":0,"obj_data":"点赞：","obj_reserve":"","obj_url":"","obj_field":"","obj_align_reserve":""},{"obj_id":36,"obj_type":3,"obj_name":"lineee","obj_x":360,"obj_y":60,"obj_w":160,"obj_h":220,"obj_opa":255,"obj_r":0,"obj_g":0,"obj_b":0,"obj_opa_2":255,"obj_r_2":0,"obj_g_2":0,"obj_b_2":0,"obj_font_size":0,"obj_font_r":0,"obj_font_g":0,"obj_font_b":0,"obj_range_min":0,"obj_range_max":100,"obj_angle_range":0,"obj_rotation":0,"obj_width":160,"obj_point_x_num":5,"obj_point_y_num":5,"obj_time":5000,"obj_var":[0],"obj_event":0,"obj_action":0,"obj_data":"like","obj_reserve":"","obj_url":"https://bilibili-video-reminder-server-wksyumnnvk.cn-hangzhou.fcapp.run?BV15u411x7nj","obj_field":"like","obj_align_reserve":""}]

// 测试
// let data = [
//     {
//         "obj_id": 4,
//         "obj_type": '3',
//         "obj_name": "untitled",
//         "obj_x": 420,
//         "obj_y": 60,
//         "obj_w": 180,
//         "obj_h": 160,
//         "obj_opa": 255,
//         "obj_r": 0,
//         "obj_g": 0,
//         "obj_b": 0,
//         "obj_opa_2": 255,
//         "obj_r_2": 0,
//         "obj_g_2": 0,
//         "obj_b_2": 0,
//         "obj_font_size": 0,
//         "obj_font_r": 0,
//         "obj_font_g": 0,
//         "obj_font_b": 0,
//         "obj_range_min": 0,
//         "obj_range_max": 1000000,
//         "obj_angle_range": 0,
//         "obj_rotation": 0,
//         "obj_width": 180,
//         "obj_point_x_num": 5,
//         "obj_point_y_num": 5,
//         "obj_time": 0,
//         "obj_var": [
//             0
//         ],
//         "obj_event": 4,
//         "obj_action": 0,
//         "obj_data": "mid",
//         "obj_reserve": "",
//         "obj_url": "https://bilibili-user-reminder-server-urdotpiowj.cn-hangzhou.fcapp.run?525438321",
//         "obj_field": "mid",
//         "obj_align_reserve": ""
//     },
//     {
//         "obj_id": 5,
//         "obj_type": 5,
//         "obj_name": "untitled",
//         "obj_x": 720,
//         "obj_y": 180,
//         "obj_w": 200,
//         "obj_h": 220,
//         "obj_opa": 255,
//         "obj_r": 0,
//         "obj_g": 0,
//         "obj_b": 0,
//         "obj_opa_2": 255,
//         "obj_r_2": 0,
//         "obj_g_2": 0,
//         "obj_b_2": 0,
//         "obj_font_size": 0,
//         "obj_font_r": 0,
//         "obj_font_g": 0,
//         "obj_font_b": 0,
//         "obj_range_min": 0,
//         "obj_range_max": 1000000,
//         "obj_angle_range": 0,
//         "obj_rotation": 0,
//         "obj_width": 200,
//         "obj_point_x_num": 5,
//         "obj_point_y_num": 5,
//         "obj_time": 0,
//         "obj_var": [
//             0
//         ],
//         "obj_event": 4,
//         "obj_action": 0,
//         "obj_data": "follower",
//         "obj_reserve": "",
//         "obj_url": "https://bilibili-user-reminder-server-urdotpiowj.cn-hangzhou.fcapp.run?525438321",
//         "obj_field": "follower",
//         "obj_align_reserve": ""
//     }
// ]

let data = [
    {
        "obj_id": 0,
        "obj_type": 5,
        "obj_name": "name",
        "obj_x": 147,
        "obj_y": 9,
        "obj_w": 500,
        "obj_h": 280,
        "obj_opa": 255,
        "obj_r": 0,
        "obj_g": 0,
        "obj_b": 0,
        "obj_opa_2": 255,
        "obj_r_2": 0,
        "obj_g_2": 0,
        "obj_b_2": 0,
        "obj_font_size": 0,
        "obj_font_r": 115,
        "obj_font_g": 255,
        "obj_font_b": 0,
        "obj_range_min": 0,
        "obj_range_max": 100,
        "obj_angle_range": 0,
        "obj_rotation": 0,
        "obj_width": 500,
        "obj_point_x_num": 5,
        "obj_point_y_num": 5,
        "obj_time": 500,
        "obj_var": [
            0
        ],
        "obj_event": 0,
        "obj_action": 0,
        "obj_data": "0.04",
        "obj_reserve": "",
        "obj_url": "",
        "obj_field": "",
        "obj_align_reserve": ""
    }
]

let hidHandle = hid.HidHandle();

hid.HidHandle().then(res => {
    console.info("res: ", res);
    // 生成UI文件
    res.generate_ui_handle(data, "./new_ui");
    // 写入数据
    console.info("更新UI：", res.hid_write_file_handle('./new_ui/hbin/hmi_res.hbin', 'hmi_res.hbin', 9))
})

