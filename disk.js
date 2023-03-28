const nodeDiskInfo = require('node-disk-info');

function dataHandle(disks) {
    let data = []
    for (const disk of disks) {
        data.push({
            filesystem: disk.filesystem,
            blocks: disk.blocks,
            used: disk.used,
            available: disk.available,
            capacity: disk.capacity,
            mounted: disk.mounted
        })
    }
    return data;
}

function deskInfo() {
    const disks = nodeDiskInfo.getDiskInfoSync();

    const list = dataHandle(disks);

    let total = list.map(item => item.blocks).reduce((prev, cur) => prev + cur);

    let available = list.map(item => item.available).reduce((prev, cur) => prev + cur);

    let ret = Number(available / total).toFixed(2) * 100;

    return ret;
}

module.exports = {
    deskInfo
}