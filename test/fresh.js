/*
 * @Author: 姚恒锋 1921934563@qq.com
 * @Date: 2023-07-25 16:58:04
 * @LastEditors: 姚恒锋 1921934563@qq.com
 * @LastEditTime: 2023-08-30 16:23:33
 * @FilePath: \hid-handle\test\wifi.js
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
const hid = require("../main.js");

hid.HidHandle().then(fn => {
  console.info("fn", fn.test_fresh_handle());
  fn.hid_write_file_handle('H:/reminder-vue2/node_modules/electron/dist/hbin/hmi_res.hbin', 'hmi_res.hbin', 9)
  // fn.hmi_send_wifi_info_handle("GalaxySZ", "galaxy123");
})
