const cpu = require("./cpu");
const mem = require("./memory");
const disk = require("./disk");

module.exports = { 
    cpuInfo: cpu.cpuInfo, 
    memoryInfo: mem.system, 
    deskInfo: disk.deskInfo,
 };
