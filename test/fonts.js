/*
 * @Author: 姚恒锋 1921934563@qq.com
 * @Date: 2023-07-25 16:58:03
 * @LastEditors: 姚恒锋 1921934563@qq.com
 * @LastEditTime: 2023-09-11 10:43:02
 * @FilePath: \hid-handle\test\generate_ui.js
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
const hid = require("../main.js");

hid.HidHandle().then(fn => {
    console.info("更新-UI：", fn.hid_write_file_handle('I:/0000000000-08-09/2023-09-04/huang/Bilibili/simhei.ttf', 'simhei.ttf', 9));
})