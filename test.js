const hid = require('./hidhandle.js')

// console.info(native);

// let res = native.hid_write_file_handle("safasf","asfas",1);

// console.info(res)

// const hid = require('H:\\hid-handle\\build\\Release\\hidhandle.node')
let hidHandle = hid.HidHandle()

console.info(hidHandle)

console.info(hidHandle.hid_write_file_handle("H:/hid-handle/test.js", "test.js", 0))

