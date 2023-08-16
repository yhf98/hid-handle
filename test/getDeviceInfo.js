/*
 * @Author: 姚恒锋 1921934563@qq.com
 * @Date: 2023-08-05 12:09:49
 * @LastEditors: 姚恒锋 1921934563@qq.com
 * @LastEditTime: 2023-08-05 12:13:42
 * @FilePath: \hid-handle\test\getDeviceInfo.js
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
const hid = require("../main.js");

hid.HidHandle().then(fn => {
  console.info(fn.get_device_info())
})