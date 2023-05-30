/*
 * @Author: 姚恒锋 1921934563@qq.com
 * @Date: 2023-05-19 10:58:27
 * @LastEditors: 姚恒锋 1921934563@qq.com
 * @LastEditTime: 2023-05-19 10:59:05
 * @FilePath: \hid-handle\test5-19.js
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
const hid = require("./hidhandle.js");
let hidHandle = hid.HidHandle();

console.info("handle: ", hidHandle);

console.info("============================================================");

console.info(hidHandle.hmi_create_obj_test_handle())
// // 写入文件更新UI
// console.info(
//   hidHandle.hid_write_file_handle("H:/reminder-vue2/product_sabbat_dual.abs", "hmi_res.hbin", 1)
// )