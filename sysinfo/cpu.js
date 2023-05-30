/*
 * @Author: 姚恒锋 1921934563@qq.com
 * @Date: 2023-05-25 14:28:34
 * @LastEditors: 姚恒锋 1921934563@qq.com
 * @LastEditTime: 2023-05-25 14:30:08
 * @FilePath: \hid-handle\sysinfo\cpu.js
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
const os = require('os');
const { exec } = require('child_process');

// 获取CPU使用率
const cpuUsage = os.loadavg()[0];

// 获取GPU使用率
exec('nvidia-smi --query-gpu=utilization.gpu --format=csv,noheader', (err, stdout, stderr) => {
  if (err) {
    console.error(err);
    return;
  }
  const gpuUsage = stdout.trim().split('\n').map(parseFloat)[0];
  console.log(`GPU usage: ${gpuUsage}%`);
});

// 获取内存使用率
const totalMem = os.totalmem();
const freeMem = os.freemem();
const usedMem = totalMem - freeMem;
const memUsage = usedMem / totalMem * 100;
console.log(`Memory usage: ${memUsage}%`);
console.log(`cpuUsage usage: ${cpuUsage}%`);
