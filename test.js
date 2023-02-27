// const native = require('./build/Release/hidhandle.node')

// console.info(native);

// let res = native.hid_write_file_handle("safasf","asfas",1);

// console.info(res)

const hid = require('H:\\hid-handle\\build\\Release\\hidhandle.node')
// let hidHandle = hid.HidHandle()

// console.info(hid)

console.info(hid.hid_write_file_handle("C:\\Users\\idmin\\.gitconfig", ".gitconfig", 0))

