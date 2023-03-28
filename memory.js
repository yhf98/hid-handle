const os = require('os');

// 查看当前 Node 进程内存使用情况

const { rss, heapUsed, heapTotal } = process.memoryUsage();

// 获取系统空闲内存
const systemFree = os.freemem();

// 获取系统总内存
const systemTotal = os.totalmem();

// export default function memoryInfo() {
//       return (Number(1 - systemFree / systemTotal).toFixed(2) * 100) + Math.floor(Math.random()* 10);
// }

module.exports = {
      system: Number(1 - systemFree / systemTotal).toFixed(2) * 100, // 系统内存占用率
      node: Number(rss / systemTotal).toFixed(2) * 100 // 当前 Node 进程内存占用系统内存的比例
}
