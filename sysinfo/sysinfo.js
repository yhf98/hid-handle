/*
 * @Author: 姚恒锋 1921934563@qq.com
 * @Date: 2023-05-25 14:45:37
 * @LastEditors: 姚恒锋 1921934563@qq.com
 * @LastEditTime: 2023-05-26 11:06:38
 * @FilePath: \hid-handle\sysinfo\sysinfo.js
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/*
 * @Author: 姚恒锋 1921934563@qq.com
 * @Date: 2023-05-25 14:45:37
 * @LastEditors: 姚恒锋 1921934563@qq.com
 * @LastEditTime: 2023-05-25 16:12:56
 * @FilePath: \hid-handle\sysinfo\sysinfo.js
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
const si = require('systeminformation');

// 网络
// si.networkStats('以太网')
//   .then(data => console.log(data))
//   .catch(error => console.error(error));

// 系统时间
// console.info(si.time(data => console.log(data)));

// 系统信息     
// si.osInfo(res => {
//     console.info("系统信息：", res)

// })

// 版本信息
// si.versions(res => {
//     console.info("版本信息：", res);
// })

// shell
// si.shell().then(res => {
//     console.info("shell：", res);
// })

// UUID
// si.uuid().then(res => {
//     console.info("uuid：", res);
// })

// 磁盘
// si.mem().then(res => {
//     console.info('mem', res);
// })

// const fs = require('fs');

// si.getDynamicData().then(res => {
//     console.info("Dynamic", fs.writeFileSync('./dy.json', JSON.stringify(res)));
// })
