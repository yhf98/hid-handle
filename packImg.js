/*
 * @Author: yaohengfeng 1921934563@qq.com
 * @Date: 2023-03-22 14:53:39
 * @LastEditors: yaohengfeng 1921934563@qq.com
 * @LastEditTime: 2023-03-22 15:32:25
 * @FilePath: \hid-handle\packImg.js
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */

const hid = require("./hidhandle.js");
let hidHandle = hid.HidHandle();


let element = [
  {
    obj_id: 0,
    obj_type: 2,
    obj_name: "img0",
    obj_x: 500,
    obj_y: 10,
    obj_w: 320,
    obj_h: 320,
    obj_opa: 0,
    obj_r: 255,
    obj_g: 0,
    obj_b: 0,
    obj_opa_2: 0,
    obj_r_2: 0,
    obj_g_2: 0,
    obj_b_2: 255,
    obj_font_size: 0,
    obj_font_r: 0,
    obj_font_g: 0,
    obj_font_b: 255,
    obj_range_min: 0,
    obj_range_max: 1000000,
    obj_angle_range: 0,
    obj_rotation: 0,
    obj_width: 0,
    obj_point_x_num: 0,
    obj_point_y_num: 0,
    obj_time: 0,
    obj_var: [50, 60, 70],
    obj_event: 0,
    obj_action: 0,
    obj_data: "333.png",
    obj_reserve: "",
  }
];

console.info("============================================================");

// 设置WIFI
// console.info("设置WIFI：")
// console.info(hidHandle.hmi_send_wifi_info_handle("jieshen", "Jieshen168"))

// // 生成UI
console.info(hidHandle.generate_ui_handle(element, 'C:/Users/idmin/reminder'))

// // 写入文件更新UI
console.info(
  hidHandle.hid_write_file_handle("C:/Users/idmin/reminder/hbin/hmi_res.hbin", "hmi_res.hbin", 1)
)

// 更新数据
// console.info("更新数据：");
// console.info(hidHandle.hmi_page_update_elem_var_handle());
