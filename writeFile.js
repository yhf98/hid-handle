/*
 * @Author: yaohengfeng 1921934563@qq.com
 * @Date: 2023-01-13 10:53:22
 * @LastEditors: 姚恒锋 1921934563@qq.com
 * @LastEditTime: 2023-07-19 11:18:54
 * @FilePath: \hid-handle\test.js
 * @Description: test.js
 */

const hid = require("./hidhandle.js");

console.info("============================================================");

// 写入文件更新UI
// console.info(
//   hidHandle.hid_write_file_handle("./new_ui/hbin/hmi_res.hbin", "hmi_res.hbin", 1)
// )

// 更新固件
hid.HidHandle().then(res => {
  console.info("res:", res)
  console.info(
    res.hid_write_file_async_handle("C:/Users/idmin/reminder/img/01.mp4", "01.mp4", 1)
  )
})
console.info("END")