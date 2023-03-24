/*
 * @Author: yaohengfeng 1921934563@qq.com
 * @Date: 2023-01-13 10:53:22
 * @LastEditors: yaohengfeng 1921934563@qq.com
 * @LastEditTime: 2023-03-24 11:02:02
 * @FilePath: \hid-handle\test.js
 * @Description: test.js
 */

const hid = require("./hidhandle.js");
let hidHandle = hid.HidHandle();

console.info("handle: ", hidHandle);

console.info("============================================================");

// hmi_page_update_elem_var_handle

console.info(
  hidHandle.hmi_page_update_elem_var_handle()
)

