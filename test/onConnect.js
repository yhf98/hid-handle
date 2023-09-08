/*
 * @Author: 姚恒锋 1921934563@qq.com
 * @Date: 2023-07-25 16:58:04
 * @LastEditors: 姚恒锋 1921934563@qq.com
 * @LastEditTime: 2023-09-08 14:32:18
 * @FilePath: \hid-handle\test\wifi.js
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
const hid = require("../main.js");

hid.HidHandle().then(fn => {
  console.info("fn：", fn);
  fn.startDeviceStatusListener((connected) => {
    console.log('Device connected', connected);
  });
})
