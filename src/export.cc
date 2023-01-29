#include <napi.h>
#include <tuple>
#include <string.h>
#include "hidhandle.h"

using namespace Napi;
using namespace std;

Value HidWriteFileHandleJs(const CallbackInfo &info)
{
    auto env = info.Env();

    string arg0 = info[0].As<String>();
    string arg1 = info[1].As<String>();
    int arg2 = info[2].As<Number>().Int32Value();

    const auto res = hid_write_file_handle(arg0.c_str(), arg1.c_str(), arg2);
    Number result = Number::New(env, res);

    return result;
}

Value HidWriteBuffHandleJs(const CallbackInfo &info)
{
    auto env = info.Env();

    string arg0 = info[0].As<String>();
    const unsigned int arg1 = info[1].As<Number>().Int32Value();
    unsigned int arg2 = info[2].As<Number>().Int32Value();

    const auto res = hid_write_buff_handle((unsigned char *)arg0.c_str(), arg1, arg2);
    Number result = Number::New(env, res);

    return result;
}

Value hidIOControlHandleJs(const CallbackInfo &info)
{
    auto env = info.Env();

    unsigned int arg0 = info[0].As<Number>().Int32Value();
    string arg1 = info[1].As<String>();
    string arg2 = info[2].As<String>();
    unsigned int arg3 = info[3].As<Number>().Int32Value();

    const auto res = hid_io_control_handle(arg0, arg1.c_str(), (unsigned char *)arg2.c_str(), arg3);
    Number result = Number::New(env, res);

    return result;
}

Object Init(Env env, Object exports)
{
    exports.Set("hid_write_file_handle", Function::New(env, HidWriteFileHandleJs));
    exports.Set("hid_write_buff_handle", Function::New(env, HidWriteBuffHandleJs));
    exports.Set("hid_io_control_handle", Function::New(env, hidIOControlHandleJs));

    return exports;
}
NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)