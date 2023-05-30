/*
 * @Author: 姚恒锋 1921934563@qq.com
 * @Date: 2023-05-25 14:08:47
 * @LastEditors: 姚恒锋 1921934563@qq.com
 * @LastEditTime: 2023-05-25 14:14:26
 * @FilePath: \hid-handle\red.js
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
const regedit = require('regedit');
 
regedit.list('QUERY HKEY_CURRENT_USER\\Software\\WanXiang\\v Name', function(err, result) {
    if (err) {
        console.log(err);
        return;
    }
    console.log(JSON.stringify(result));
});