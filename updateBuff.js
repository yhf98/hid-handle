/*
 * @Author: yaohengfeng 1921934563@qq.com
 * @Date: 2023-01-13 10:53:22
 * @LastEditors: yaohengfeng 1921934563@qq.com
 * @LastEditTime: 2023-03-27 11:18:02
 * @FilePath: \hid-handle\test.js
 * @Description: test.js
 */

const hid = require("./hidhandle.js");
let hidHandle = hid.HidHandle();

console.info("handle: ", hidHandle);

console.info("============================================================");

// 更新buff
let buff = "123456789012345678901234567890-123456789012345678901234567890"

console.info(
  hidHandle.hmi_update_screen_data(0, buff, buff.length)
)

