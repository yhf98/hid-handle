/*
 * @Author: 姚恒锋 1921934563@qq.com
 * @Date: 2023-08-25 11:41:07
 * @LastEditors: 姚恒锋 1921934563@qq.com
 * @LastEditTime: 2023-09-06 10:55:53
 * @FilePath: \hid-handle\test\unpacket.js
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
const hid = require("../main.js");

hid.HidHandle().then(fn => {
  fn.hmi_unpacket_file_handle("H:/reminder-vue2/node_modules/electron/dist/hbin/hmi_res.hbin", "./output");
})
