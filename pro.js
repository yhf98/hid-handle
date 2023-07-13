// // const hid = require("./hidhandle.js");
// // let hidHandle = hid.HidHandle();

// const addon = require('./build/Release/hidhandle.node');

// function testHandle() {
//     return new Promise((resolve, reject) => {
//         console.info("addon", addon, addon.testHandle, addon.testHandle())
//         addon.testHandle((error) => {
//             console.info("-1--")
//             if (error) {
//                 console.info("2--")
//                 reject(error);
//             } else {
//                 console.info("-3-")
//                 resolve("Async operation completed");
//             }
//         });
//     });
// }

// console.info("开始：", testHandle());
// testHandle()
// .then(result => {
//     console.log('异步操作成功：', result);
// })
// .catch(error => {
//     console.error('异步操作失败：', error);
// });

// console.info("结束：");

const addon = require('./build/Release/hidhandle.node');

addon.asyncFunction()
  .then(result => {
    console.log(result);
  })
  .catch(error => {
    console.error(error);
  });