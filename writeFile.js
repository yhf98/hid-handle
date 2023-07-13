/*
 * @Author: yaohengfeng 1921934563@qq.com
 * @Date: 2023-01-13 10:53:22
 * @LastEditors: 姚恒锋 1921934563@qq.com
 * @LastEditTime: 2023-07-13 16:26:43
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
    res.hid_write_file_async_handle("H:/reminder-vue2/node_modules/hid-handle/product_sabbat_dual.abs", "product_sabbat_dual.abs", 5)
  )
})
console.info("END")