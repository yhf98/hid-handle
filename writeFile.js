/*
 * @Author: yaohengfeng 1921934563@qq.com
 * @Date: 2023-01-13 10:53:22
 * @LastEditors: 姚恒锋 1921934563@qq.com
 * @LastEditTime: 2023-07-05 17:38:27
 * @FilePath: \hid-handle\test.js
 * @Description: test.js
 */

const hid = require("./hidhandle.js");
let hidHandle = hid.HidHandle();

console.info("handle: ", hidHandle);

console.info("============================================================");

// 写入文件更新UI
console.info(
  hidHandle.hid_write_file_handle("./new_ui/hbin/hmi_res.hbin", "hmi_res.hbin", 1)
)

// 更新固件
// console.info(
//   hidHandle.hid_write_file_handle("./product_sabbat_dual.abs", "product_sabbat_dual.abs", 1)
// )