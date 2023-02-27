
var os = require('os')

var driverType = null;
function setDriverType(type) {
    driverType = type;
}

// lazy load the C++ binding
var binding = null;
function loadBinding() {
    if (os.platform() === 'linux') {
        // Linux defaults to hidraw
        if (!driverType || driverType === 'hidraw') {
            return binding = require('bindings')('hidhandle_raw.node');
        } else {
            return binding = require('bindings')('hidhandle.node');
        }
    }
    else {
        return binding = require('bindings')('hidhandle.node');
    }
}

//This class is a wrapper for `binding.HidHandle` class
function HidHandle() {
    return loadBinding();
}

//Expose API
exports.HidHandle = HidHandle;
exports.setDriverType = setDriverType;