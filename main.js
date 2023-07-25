var os = require('os')

var driverType = null;
function setDriverType(type) {
    driverType = type;
}

// lazy load the C++ binding
var binding = null;
async function loadBinding() {
    return binding = await require('./build/Release/hidhandle.node');
}

//This class is a wrapper for `binding.HidHandle` class
function HidHandle() {
    return loadBinding();
}

//Expose API
exports.HidHandle = HidHandle;
exports.setDriverType = setDriverType;