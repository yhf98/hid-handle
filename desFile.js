/*
 * @Author: 姚恒锋 1921934563@qq.com
 * @Date: 2023-05-29 15:06:27
 * @LastEditors: 姚恒锋 1921934563@qq.com
 * @LastEditTime: 2023-05-29 15:09:19
 * @FilePath: \hid-handle\desFile.js
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
const hid = require("./hidhandle.js");
let hidHandle = hid.HidHandle();

// 解压文件
console.info('解压：文件')

console.info(hidHandle.hmi_unpacket_file_handle('./new_ui/hbin/hmi_res.hbin', './new_ui/out'))
