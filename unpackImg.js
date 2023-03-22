/*
 * @Author: yaohengfeng 1921934563@qq.com
 * @Date: 2023-03-22 14:53:39
 * @LastEditors: yaohengfeng 1921934563@qq.com
 * @LastEditTime: 2023-03-22 15:17:23
 * @FilePath: \hid-handle\packImg.js
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */

const hid = require("./hidhandle.js");
let hidHandle = hid.HidHandle();

// 解压文件
console.info(
  hidHandle.hmi_unpacket_file_handle("C:/Users/idmin/reminder/hbin/hmi_res.hbin", "C:/Users/idmin/reminder/output")
)

