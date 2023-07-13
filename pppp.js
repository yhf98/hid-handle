const addon = require('./build/Release/hidhandle.node');

// console.info(addon);
// console.info(addon.TestHandleAsync);
// console.info(addon.TestHandleAsync());
// addon.TestHandleAsync()
// .then(result => {
//   console.log(result);
// })
// .catch(error => {
//   console.error(error);
// });

// 调用 C++ 函数并返回 Promise 对象
// const promise = addon.TestHandleAsync(() => {
//   console.log('Callback function called');
// });

// // 在 Promise 对象上添加完成处理函数和错误处理函数
// promise.then((result) => {
//   console.log('Promise resolved:', result);
// }).catch((error) => {
//   console.error('Promise rejected:', error);
// });
// console.info("addon.testAsync", addon, addon.testAsync, addon.testAsync())

const hid = require('./hidhandle')

console.info("GGG");
hid.HidHandle().then(fn => {
  fn.testAsync()
    .then(result => {
      console.log("====", result);
    })
    .catch(error => {
      console.error("----", error);
    });
})

console.info("VVV");
