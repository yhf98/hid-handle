// const native = require('./build/Release/hidhandle.node')

// console.info(native);

// let res = native.hid_write_file_handle("safasf","asfas",1);

// console.info(res)

const hid = require('./hidhandle')
let hidHandle = hid.HidHandle()

console.info(hidHandle.hid_write_file_handle("C:/Users/idmin/Pictures/cat.png", "aa",1))