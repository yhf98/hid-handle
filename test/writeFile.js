/*
 * @Author: yaohengfeng 1921934563@qq.com
 * @Date: 2023-01-13 10:53:22
 * @LastEditors: 姚恒锋 1921934563@qq.com
 * @LastEditTime: 2023-07-25 17:58:22
 * @FilePath: \hid-handle\test\writeFile.js
 * @Description: writeFile.js
 */

const hid = require("../main.js");

console.info("============================================================");


hid.HidHandle().then(fn => {
  // 写入文件更新UI
  // console.info(
  //   fn.hid_write_file_handle("./new_ui/hbin/hmi_res.hbin", "hmi_res.hbin", 1)
  // )

  // 更新固件
  console.info(
    fn.hid_write_file_handle("./product_sabbat_dual.abs", "product_sabbat_dual.abs", 1)
  )

})