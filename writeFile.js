/*
 * @Author: yaohengfeng 1921934563@qq.com
 * @Date: 2023-01-13 10:53:22
 * @LastEditors: yaohengfeng 1921934563@qq.com
 * @LastEditTime: 2023-03-21 15:12:06
 * @FilePath: \hid-handle\test.js
 * @Description: test.js
 */

const hid = require("./hidhandle.js");
let hidHandle = hid.HidHandle();

console.info("handle: ", hidHandle);

console.info("============================================================");

// 写入文件更新UI
console.info(
  hidHandle.hid_write_file_handle("C:/Users/idmin/reminder/hbin/hmi_res.hbin", "hmi_res.hbin", 1)
)

