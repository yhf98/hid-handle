/*
 * @Author: 姚恒锋 1921934563@qq.com
 * @Date: 2023-05-25 14:33:09
 * @LastEditors: 姚恒锋 1921934563@qq.com
 * @LastEditTime: 2023-05-25 14:41:09
 * @FilePath: \hid-handle\sysinfo\nets.js
 * @Description: 网络配置
 */
const si = require('systeminformation');

// 网络
// si.networkStats('以太网')
//   .then(data => console.log(data))
//   .catch(error => console.error(error));

// 系统时间
console.info(si.time(data => console.log(data)));