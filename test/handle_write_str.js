/*
 * @Author: 姚恒锋 1921934563@qq.com
 * @Date: 2023-09-13 17:20:04
 * @LastEditors: 姚恒锋 1921934563@qq.com
 * @LastEditTime: 2023-09-14 14:57:00
 * @FilePath: \hid-handle\test\handle_write_file.js
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
const HANDLE = require("../handle.js");

// var device = new HANDLE.HANDLE(0x264a, 0x232a);
var device = new HANDLE.HANDLE(0x483, 0x5750);
console.info("device: ", device)
console.info("getDeviceInfo: ", device.getDeviceInfo())

const str = ``
device.writeStr(item)
const sliceLength = 64;
// 切片后的数组
let slicedArray = [];

// 遍历数组并切片
for (let i = 0; i < str.length; i += sliceLength) {
  let slice = str.slice(i, i + sliceLength);
  slicedArray.push(slice);
}
// console.info(slicedArray)

slicedArray.forEach(item => {
    console.info(item)
    device.writeStr(item)
})




