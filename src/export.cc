/*
 * @Author: yaohengfeng 1921934563@qq.com
 * @Date: 2023-01-13 10:58:19
 * @LastEditors: yaohengfeng 1921934563@qq.com
 * @LastEditTime: 2023-03-10 15:34:59
 * @FilePath: \hid-handle\src\export.cc
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
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

    char* cstr = new char[arg2.size() + 1];
    std::copy(arg2.begin(), arg2.end(), cstr);
    cstr[arg2.size()] = '\0';

    const auto res = hid_io_control_handle(arg0, arg1.c_str(), cstr, arg3);
    Number result = Number::New(env, res);

    delete[] cstr;

    return result;
}

Value hmiCreateLabelHandleJs(const CallbackInfo &info)
{
    auto env = info.Env();

    const auto res = hmi_create_label_handle();
    Number result = Number::New(env, res);

    return result;
}

Object Init(Env env, Object exports)
{
    exports.Set("hid_write_file_handle", Function::New(env, HidWriteFileHandleJs));
    exports.Set("hid_write_buff_handle", Function::New(env, HidWriteBuffHandleJs));
    exports.Set("hid_io_control_handle", Function::New(env, hidIOControlHandleJs));
    exports.Set("hmi_create_label_handle", Function::New(env, hmiCreateLabelHandleJs));

    return exports;
}
NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)