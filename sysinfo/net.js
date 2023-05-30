/*
 * @Author: 姚恒锋 1921934563@qq.com
 * @Date: 2023-05-25 14:19:55
 * @LastEditors: 姚恒锋 1921934563@qq.com
 * @LastEditTime: 2023-05-25 14:24:11
 * @FilePath: \hid-handle\sysinfo\net.js
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
const speedTest = require('speedtest-net');

(async () => {
  try {
    console.log("网络", await speedTest());
  } catch (err) {
    console.log("EEEE--RRR", err.message);
  } finally {
    process.exit(0);
  }
})();