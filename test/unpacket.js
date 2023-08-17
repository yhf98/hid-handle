const hid = require("../main.js");

hid.HidHandle().then(fn => {
  fn.hmi_unpacket_file_handle("H:/reminder-vue2/node_modules/electron/dist/hbin/hmi_res.hbin", "./output");
})
