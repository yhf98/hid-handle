const hid = require("./hidhandle.js");
let hidHandle = hid.HidHandle();

// 解压文件
console.info(
  hidHandle.hmi_send_wifi_info_handle("jieshen", "Jieshen168")
)