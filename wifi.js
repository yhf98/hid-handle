/*
 * @Author: 姚恒锋 1921934563@qq.com
 * @Date: 2023-03-23 10:55:36
 * @LastEditors: 姚恒锋 1921934563@qq.com
 * @LastEditTime: 2023-07-21 16:18:38
 * @FilePath: \hid-handle\wifi.js
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
const hid = require("./hidhandle.js");

hid.HidHandle().then(res => {
  console.info("res:", res)
  console.info(
    // res.hmi_send_wifi_info_handle("权猫咖啡", "Quanmao168")
    res.hmi_send_wifi_info_handle("GalaxySZ", "galaxy123")
  )
})
