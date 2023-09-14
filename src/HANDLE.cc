/*
 * @Author: 姚恒锋 1921934563@qq.com
 * @Date: 2023-09-13 13:51:25
 * @LastEditors: 姚恒锋 1921934563@qq.com
 * @LastEditTime: 2023-09-14 15:15:52
 * @FilePath: \hid-handle\src\HANDLE.cc
 * @Description: HANDLE
 */
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <napi.h>
#include <hidapi.h>

#include "hmi/hmi_hid.h"
#include "hmi/hmi_core.h"
#include "hmi/hmi_packet.h"

#define READ_BUFF_MAXSIZE 2048

using namespace Napi;
using namespace std;

class HANDLE : public Napi::ObjectWrap<HANDLE>
{
public:
    static void Initialize(Napi::Env &env, Napi::Object &exports);

    void closeHandle();

    HANDLE(const Napi::CallbackInfo &info);
    ~HANDLE() { closeHandle(); }

    hid_device *_hidHandle;

private:
    static Napi::Value devices(const Napi::CallbackInfo &info);
    // From node-hid
    Napi::Value close(const Napi::CallbackInfo &info);
    Napi::Value read(const Napi::CallbackInfo &info);
    Napi::Value write(const Napi::CallbackInfo &info);
    Napi::Value setNonBlocking(const Napi::CallbackInfo &info);
    Napi::Value getFeatureReport(const Napi::CallbackInfo &info);
    Napi::Value sendFeatureReport(const Napi::CallbackInfo &info);
    Napi::Value readSync(const Napi::CallbackInfo &info);
    Napi::Value readTimeout(const Napi::CallbackInfo &info);
    Napi::Value getDeviceInfo(const Napi::CallbackInfo &info);
    // 自定义HANDLE
    Napi::Value writeStr(const Napi::CallbackInfo &info);
    Napi::Value writeHex(const Napi::CallbackInfo &info);
    Napi::Value writeFile(const Napi::CallbackInfo &info);
    Napi::Value writeFileAsync(const Napi::CallbackInfo &info);
    Napi::Value sendWifiInfo(const Napi::CallbackInfo &info);
    Napi::Value sendWifiInfoAsyn(const Napi::CallbackInfo &info);
    Napi::Value generateUI(const Napi::CallbackInfo &info);
    Napi::Value unpacketFile(const Napi::CallbackInfo &info);
    Napi::Value updateScreenData(const Napi::CallbackInfo &info);
    Napi::Value updateScreenDataAsync(const Napi::CallbackInfo &info);
    Napi::Value hidInit(const Napi::CallbackInfo &info);
};

HANDLE::HANDLE(const Napi::CallbackInfo &info)
    : Napi::ObjectWrap<HANDLE>(info)
{
    Napi::Env env = info.Env();

    if (!info.IsConstructCall())
    {
        Napi::TypeError::New(env, "HANDLE function can only be used as a constructor").ThrowAsJavaScriptException();
        return;
    }

    if (info.Length() < 1)
    {
        Napi::TypeError::New(env, "HANDLE constructor requires at least one argument").ThrowAsJavaScriptException();
        return;
    }

    if (info.Length() == 1)
    {
        // open by path
        if (!info[0].IsString())
        {
            Napi::TypeError::New(env, "Device path must be a string").ThrowAsJavaScriptException();
            return;
        }

        std::string path = info[0].As<Napi::String>().Utf8Value();
        _hidHandle = hid_open_path(path.c_str());
        if (!_hidHandle)
        {
            std::ostringstream os;
            os << "cannot open device with path " << path;
            Napi::TypeError::New(env, os.str()).ThrowAsJavaScriptException();
            return;
        }
    }
    else
    {
        int32_t vendorId = info[0].As<Napi::Number>().Int32Value();
        int32_t productId = info[1].As<Napi::Number>().Int32Value();
        wchar_t wserialstr[100]; // FIXME: is there a better way?
        wchar_t *wserialptr = NULL;
        if (info.Length() > 2)
        {
            std::string serialstr = info[2].As<Napi::String>().Utf8Value();
            mbstowcs(wserialstr, serialstr.c_str(), 100);
            wserialptr = wserialstr;
        }

        _hidHandle = hid_open(vendorId, productId, wserialptr);
        if (!_hidHandle)
        {
            std::ostringstream os;
            os << "cannot open device with vendor id 0x" << std::hex << vendorId << " and product id 0x" << productId;
            Napi::TypeError::New(env, os.str()).ThrowAsJavaScriptException();
            return;
        }
    }
}

void HANDLE::closeHandle()
{
    if (_hidHandle)
    {
        hid_close(_hidHandle);
        _hidHandle = 0;
    }
}

class ReadWorker : public Napi::AsyncWorker
{
public:
    ReadWorker(HANDLE *hid, Napi::Function &callback)
        : Napi::AsyncWorker(hid->Value(), callback), _hid(hid) {}

    ~ReadWorker()
    {
        if (buf != nullptr)
        {
            delete[] buf;
        }
    }
    // This code will be executed on the worker thread
    void Execute() override
    {
        int mswait = 50;
        while (len == 0 && _hid->_hidHandle)
        {
            len = hid_read_timeout(_hid->_hidHandle, buf, READ_BUFF_MAXSIZE, mswait);
        }
        if (len <= 0)
        {
            SetError("could not read from HANDLE device");
        }
    }

    void OnOK() override
    {
        auto buffer = Napi::Buffer<unsigned char>::Copy(Env(), buf, len);
        Callback().Call({Env().Null(), buffer});
    }

private:
    HANDLE *_hid;
    unsigned char *buf = new unsigned char[READ_BUFF_MAXSIZE];
    int len = 0;
};

Napi::Value HANDLE::read(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    if (info.Length() != 1 || !info[0].IsFunction())
    {
        Napi::TypeError::New(env, "need one callback function argument in read").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto callback = info[0].As<Napi::Function>();
    auto job = new ReadWorker(this, callback);
    job->Queue();

    return env.Null();
}

Napi::Value HANDLE::readSync(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    if (info.Length() != 0)
    {
        Napi::TypeError::New(env, "readSync needs zero length parameter").ThrowAsJavaScriptException();
        return env.Null();
    }

    unsigned char buff_read[READ_BUFF_MAXSIZE];
    int returnedLength = hid_read(_hidHandle, buff_read, sizeof buff_read);
    if (returnedLength == -1)
    {
        Napi::TypeError::New(env, "could not read data from device").ThrowAsJavaScriptException();
        return env.Null();
    }

    Napi::Array retval = Napi::Array::New(env, returnedLength);
    for (int i = 0; i < returnedLength; i++)
    {
        retval.Set(i, Napi::Number::New(env, buff_read[i]));
    }
    return retval;
}

Napi::Value HANDLE::readTimeout(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    if (info.Length() != 1 || !info[0].IsNumber())
    {
        Napi::TypeError::New(env, "readTimeout needs time out parameter").ThrowAsJavaScriptException();
        return env.Null();
    }

    const int timeout = info[0].As<Napi::Number>().Uint32Value();
    unsigned char buff_read[READ_BUFF_MAXSIZE];
    int returnedLength = hid_read_timeout(_hidHandle, buff_read, sizeof buff_read, timeout);
    if (returnedLength == -1)
    {
        Napi::TypeError::New(env, "could not read data from device").ThrowAsJavaScriptException();
        return env.Null();
    }

    Napi::Array retval = Napi::Array::New(env, returnedLength);
    for (int i = 0; i < returnedLength; i++)
    {
        retval.Set(i, Napi::Number::New(env, buff_read[i]));
    }
    return retval;
}

Napi::Value HANDLE::getFeatureReport(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    if (info.Length() != 2 || !info[0].IsNumber() || !info[1].IsNumber())
    {
        Napi::TypeError::New(env, "need report ID and length parameters in getFeatureReport").ThrowAsJavaScriptException();
        return env.Null();
    }

    const uint8_t reportId = info[0].As<Napi::Number>().Uint32Value();
    const int bufSize = info[1].As<Napi::Number>().Uint32Value();
    if (bufSize == 0)
    {
        Napi::TypeError::New(env, "Length parameter cannot be zero in getFeatureReport").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::vector<unsigned char> buf(bufSize);
    buf[0] = reportId;

    int returnedLength = hid_get_feature_report(_hidHandle, buf.data(), bufSize);
    if (returnedLength == -1)
    {
        Napi::TypeError::New(env, "could not get feature report from device").ThrowAsJavaScriptException();
        return env.Null();
    }

    Napi::Array retval = Napi::Array::New(env, returnedLength);
    for (int i = 0; i < returnedLength; i++)
    {
        retval.Set(i, Napi::Number::New(env, buf[i]));
    }
    return retval;
}

Napi::Value HANDLE::sendFeatureReport(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    if (info.Length() != 1)
    {
        Napi::TypeError::New(env, "need report (including id in first byte) only in sendFeatureReport").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::vector<unsigned char> message;
    if (info[0].IsBuffer())
    {
        Napi::Buffer<unsigned char> buffer = info[0].As<Napi::Buffer<unsigned char>>();
        uint32_t len = buffer.Length();
        unsigned char *data = buffer.Data();
        message.assign(data, data + len);
    }
    else if (info[0].IsArray())
    {
        Napi::Array messageArray = info[0].As<Napi::Array>();
        message.reserve(messageArray.Length());

        for (unsigned i = 0; i < messageArray.Length(); i++)
        {
            Napi::Value v = messageArray.Get(i);
            if (!v.IsNumber())
            {
                Napi::TypeError::New(env, "unexpected array element in array to send, expecting only integers").ThrowAsJavaScriptException();
                return env.Null();
            }
            uint32_t b = v.As<Napi::Number>().Uint32Value();
            message.push_back((unsigned char)b);
        }
    }
    else
    {
        Napi::TypeError::New(env, "unexpected data to send, expecting an array or buffer").ThrowAsJavaScriptException();
        return env.Null();
    }

    int returnedLength = hid_send_feature_report(_hidHandle, message.data(), message.size());
    if (returnedLength == -1)
    { // Not sure if there would ever be a valid return value of 0.
        Napi::TypeError::New(env, "could not send feature report to device").ThrowAsJavaScriptException();
        return env.Null();
    }

    return Napi::Number::New(env, returnedLength);
}

Napi::Value HANDLE::close(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    this->closeHandle();
    return env.Null();
}

Napi::Value HANDLE::setNonBlocking(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    if (info.Length() != 1)
    {
        Napi::TypeError::New(env, "Expecting a 1 to enable, 0 to disable as the first argument.").ThrowAsJavaScriptException();
        return env.Null();
    }

    int blockStatus = info[0].As<Napi::Number>().Int32Value();
    int res = hid_set_nonblocking(_hidHandle, blockStatus);
    if (res < 0)
    {
        Napi::TypeError::New(env, "Error setting non-blocking mode.").ThrowAsJavaScriptException();
        return env.Null();
    }

    return env.Null();
}

Napi::Value HANDLE::write(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    if (info.Length() != 1)
    {
        Napi::TypeError::New(env, "HANDLE write requires one argument").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::vector<unsigned char> message;
    if (info[0].IsBuffer())
    {
        Napi::Buffer<unsigned char> buffer = info[0].As<Napi::Buffer<unsigned char>>();
        uint32_t len = buffer.Length();
        unsigned char *data = buffer.Data();
        message.assign(data, data + len);
    }
    else if (info[0].IsArray())
    {
        Napi::Array messageArray = info[0].As<Napi::Array>();
        message.reserve(messageArray.Length());

        for (unsigned i = 0; i < messageArray.Length(); i++)
        {
            Napi::Value v = messageArray.Get(i);
            if (!v.IsNumber())
            {
                Napi::TypeError::New(env, "unexpected array element in array to send, expecting only integers").ThrowAsJavaScriptException();
                return env.Null();
            }
            uint32_t b = v.As<Napi::Number>().Uint32Value();
            message.push_back((unsigned char)b);
        }
    }
    else
    {
        Napi::TypeError::New(env, "unexpected data to send, expecting an array or buffer").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!_hidHandle)
    {
        Napi::TypeError::New(env, "Cannot write to closed device").ThrowAsJavaScriptException();
        return env.Null();
    }

    int returnedLength = hid_write(_hidHandle, message.data(), message.size());
    if (returnedLength < 0)
    {
        Napi::TypeError::New(env, "Cannot write to hid device").ThrowAsJavaScriptException();
        return env.Null();
    }

    return Napi::Number::New(env, returnedLength);
}

static std::string narrow(wchar_t *wide)
{
    std::wstring ws(wide);
    std::ostringstream os;
    for (size_t i = 0; i < ws.size(); i++)
    {
        os << os.narrow(ws[i], '?');
    }
    return os.str();
}

Napi::Value HANDLE::getDeviceInfo(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    const int maxlen = 256;
    wchar_t wstr[maxlen]; // FIXME: use new & delete

    Napi::Object deviceInfo = Napi::Object::New(env);

    hid_get_manufacturer_string(_hidHandle, wstr, maxlen);
    deviceInfo.Set("manufacturer", Napi::String::New(env, narrow(wstr)));

    hid_get_product_string(_hidHandle, wstr, maxlen);
    deviceInfo.Set("product", Napi::String::New(env, narrow(wstr)));

    hid_get_serial_number_string(_hidHandle, wstr, maxlen);
    deviceInfo.Set("serialNumber", Napi::String::New(env, narrow(wstr)));

    return deviceInfo;
}

Napi::Value HANDLE::devices(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    int vendorId = 0;
    int productId = 0;

    switch (info.Length())
    {
    case 0:
        break;
    case 2:
        vendorId = info[0].As<Napi::Number>().Int32Value();
        productId = info[1].As<Napi::Number>().Int32Value();
        break;
    default:
        Napi::TypeError::New(env, "unexpected number of arguments to HANDLE.devices() call, expecting either no arguments or vendor and product ID").ThrowAsJavaScriptException();
        return env.Null();
    }

    hid_device_info *devs = hid_enumerate(vendorId, productId);
    Napi::Array retval = Napi::Array::New(env);
    int count = 0;
    for (hid_device_info *dev = devs; dev; dev = dev->next)
    {
        Napi::Object deviceInfo = Napi::Object::New(env);
        deviceInfo.Set("vendorId", Napi::Number::New(env, dev->vendor_id));
        deviceInfo.Set("productId", Napi::Number::New(env, dev->product_id));
        if (dev->path)
        {
            deviceInfo.Set("path", Napi::String::New(env, dev->path));
        }
        if (dev->serial_number)
        {
            deviceInfo.Set("serialNumber", Napi::String::New(env, narrow(dev->serial_number)));
        }
        if (dev->manufacturer_string)
        {
            deviceInfo.Set("manufacturer", Napi::String::New(env, narrow(dev->manufacturer_string)));
        }
        if (dev->product_string)
        {
            deviceInfo.Set("product", Napi::String::New(env, narrow(dev->product_string)));
        }
        deviceInfo.Set("release", Napi::Number::New(env, dev->release_number));
        deviceInfo.Set("interface", Napi::Number::New(env, dev->interface_number));
        if (dev->usage_page)
        {
            deviceInfo.Set("usagePage", Napi::Number::New(env, dev->usage_page));
        }
        if (dev->usage)
        {
            deviceInfo.Set("usage", Napi::Number::New(env, dev->usage));
        }
        retval.Set(count++, deviceInfo);
    }
    hid_free_enumeration(devs);
    return retval;
}

Napi::Value HANDLE::writeFile(const Napi::CallbackInfo &info)
{
    auto env = info.Env();

    string arg0 = info[0].As<String>();
    string arg1 = info[1].As<String>();
    int arg2 = info[2].As<Napi::Number>().Int32Value();

    const auto res = hid_write_file(_hidHandle, arg0.c_str(), arg1.c_str(), arg2);

    Napi::Number result = Number::New(env, res);

    return result;
}

Napi::Value HANDLE::writeStr(const Napi::CallbackInfo &info)
{
    auto env = info.Env();

    string str = info[0].As<String>();
    if (str.empty())
    {
        TypeError::New(env, "String cannot be empty!").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    unsigned char writebuff[65];
    sprintf((char *const)writebuff, " %s", str.c_str());

    printf("Revices: %s\n", writebuff);
    const auto res = hid_write(_hidHandle, writebuff, sizeof(writebuff));

    Napi::Number result = Number::New(env, res);

    return result;
}

Napi::Value HANDLE::writeHex(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    if (info.Length() != 1)
    {
        Napi::TypeError::New(env, "HANDLE write requires one argument").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::vector<unsigned char> message;
    if (info[0].IsBuffer())
    {
        printf("==========Buffer======");
        Napi::Buffer<unsigned short> buffer = info[0].As<Napi::Buffer<unsigned short>>();
        uint32_t len = buffer.Length();
        unsigned short *data = buffer.Data();
        message.assign(data, data + len);
    }
    else if (info[0].IsArray())
    {
        printf("==========Array======");
        Napi::Array messageArray = info[0].As<Napi::Array>();
        message.reserve(messageArray.Length());

        for (unsigned i = 0; i < messageArray.Length(); i++)
        {
            Napi::Value v = messageArray.Get(i);
            if (!v.IsNumber())
            {
                Napi::TypeError::New(env, "unexpected array element in array to send, expecting only integers").ThrowAsJavaScriptException();
                return env.Null();
            }
            uint32_t b = v.As<Napi::Number>().Uint32Value();
            message.push_back((unsigned short)b);
        }
    }
    else
    {
        Napi::TypeError::New(env, "unexpected data to send, expecting an array or buffer").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!_hidHandle)
    {
        Napi::TypeError::New(env, "Cannot write to closed device").ThrowAsJavaScriptException();
        return env.Null();
    }

    int returnedLength = hid_write(_hidHandle, message.data(), message.size());
    if (returnedLength < 0)
    {
        Napi::TypeError::New(env, "Cannot write to hid device").ThrowAsJavaScriptException();
        return env.Null();
    }

    return Napi::Number::New(env, returnedLength);
}

// FIXME: 打包出错：Napi::Promise::Deferred”: 没有合适的默认构造函数可用
Napi::Value HANDLE::writeFileAsync(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    // 创建 Promise 的执行器函数
    auto executor = [&](Napi::Promise::Deferred deferred)
    {
        string arg0 = info[0].As<String>();
        string arg1 = info[1].As<String>();
        int arg2 = info[2].As<Number>().Int32Value();
        // 异步操作完成后，根据结果决定是解决还是拒绝 Promise
        int success = hid_write_file(_hidHandle, arg0.c_str(), arg1.c_str(), arg2);
        if (success == 0)
        {
            deferred.Resolve(Number::New(env, success));
        }
        else
        {
            Error::New(env, "Write file error!").ThrowAsJavaScriptException();
            deferred.Reject(env.Null()); // 使用 Null 作为拒绝的原因
        }
    };

    // 创建 Promise 对象并返回
    Napi::Promise::Deferred deferred = Napi::Promise::Deferred::New(env);
    Napi::Promise promise = deferred.Promise();

    // 调用执行器函数，开始异步操作
    executor(deferred);

    // 返回 Promise 对象给 JavaScript
    return promise;
}

Napi::Value HANDLE::sendWifiInfo(const Napi::CallbackInfo &info)
{
    auto env = info.Env();

    string wifiName = info[0].As<String>();
    string wifiPassword = info[1].As<String>();

    const auto res = hmi_send_wifi_info(_hidHandle, wifiName.c_str(), wifiPassword.c_str());
    Number result = Number::New(env, res);

    return result;
}

Napi::Value HANDLE::generateUI(const Napi::CallbackInfo &info)
{
    auto env = info.Env();

    if (info.Length() < 1 || !info[0].IsArray())
    {
        TypeError::New(env, "Array expected").ThrowAsJavaScriptException();
        return env.Null();
    }

    Array objAttrTs = info[0].As<Array>();

    string pkgPath = info[1].As<String>();
    string wifiName = info[2].As<String>();
    string wifiPwd = info[3].As<String>();

    printf("\n\n--11--\n\n");

    vector<obj_attr_t> vec_obj_attr_t;

    for (uint32_t i = 0; i < objAttrTs.Length(); i++)
    {
        Napi::Value _value = objAttrTs.Get(i);
        Object objAttrT = _value.As<Object>();

        if (!objAttrT.IsObject())
        {
            TypeError::New(env, "Object expected").ThrowAsJavaScriptException();
            return env.Null();
        }

        obj_attr_t para;
        memset(&para, 0, sizeof(obj_attr_t));

        // 对象id,要唯一
        para.obj_id = objAttrT.Get("obj_id").As<Number>().Int32Value();
        // UI类型
        para.obj_type = objAttrT.Get("obj_type").As<Number>().Int32Value();
        // 对象名字,唯一
        std::string obj_name = objAttrT.Get("obj_name").As<String>();
        strcpy_s(para.obj_name, obj_name.c_str());

        // Wifi 名称
        std::string obj_wifi_name = objAttrT.Get("obj_wifi_name").As<String>();
        strcpy_s(para.obj_wifi_name, obj_wifi_name.c_str());
        // Wifi 密码
        std::string obj_wifi_pass = objAttrT.Get("obj_wifi_pass").As<String>();
        strcpy_s(para.obj_wifi_pass, obj_wifi_pass.c_str());

        // 横向起点坐标
        para.obj_x = objAttrT.Get("obj_x").As<Number>().Int32Value();
        // 纵向起点坐标
        para.obj_y = objAttrT.Get("obj_y").As<Number>().Int32Value();
        // 宽度
        para.obj_w = objAttrT.Get("obj_w").As<Number>().Int32Value();
        // 高度
        para.obj_h = objAttrT.Get("obj_h").As<Number>().Int32Value();
        // 主透明度/颜色
        para.obj_opa = objAttrT.Get("obj_opa").As<Number>().Int32Value();
        para.obj_r = objAttrT.Get("obj_r").As<Number>().Int32Value();
        para.obj_g = objAttrT.Get("obj_g").As<Number>().Int32Value();
        para.obj_b = objAttrT.Get("obj_b").As<Number>().Int32Value();
        // 次透明度/颜色
        para.obj_opa_2 = objAttrT.Get("obj_opa_2").As<Number>().Int32Value();
        para.obj_r_2 = objAttrT.Get("obj_r_2").As<Number>().Int32Value();
        para.obj_g_2 = objAttrT.Get("obj_g_2").As<Number>().Int32Value();
        para.obj_b_2 = objAttrT.Get("obj_b_2").As<Number>().Int32Value();
        // 次透明度/颜色
        para.obj_opa_3 = objAttrT.Get("obj_opa_3").As<Number>().Int32Value();
        para.obj_r_3 = objAttrT.Get("obj_r_3").As<Number>().Int32Value();
        para.obj_g_3 = objAttrT.Get("obj_g_3").As<Number>().Int32Value();
        para.obj_b_3 = objAttrT.Get("obj_b_3").As<Number>().Int32Value();
        // 字体名字
        string obj_font_name = objAttrT.Get("obj_font_name").As<String>();
        strcpy_s(para.obj_font_name, obj_font_name.c_str());
        // 字体大小
        para.obj_font_size = objAttrT.Get("obj_font_size").As<Number>().Int32Value();
        // 字体颜色
        para.obj_font_r = objAttrT.Get("obj_font_r").As<Number>().Int32Value();
        para.obj_font_g = objAttrT.Get("obj_font_g").As<Number>().Int32Value();
        para.obj_font_b = objAttrT.Get("obj_font_b").As<Number>().Int32Value();
        // 数值标签
        para.obj_val_flag = objAttrT.Get("obj_val_flag").As<Number>().Int32Value();
        // 上涨颜色
        para.obj_rise_r = objAttrT.Get("obj_rise_r").As<Number>().Int32Value();
        para.obj_rise_g = objAttrT.Get("obj_rise_g").As<Number>().Int32Value();
        para.obj_rise_b = objAttrT.Get("obj_rise_b").As<Number>().Int32Value();
        // 下跌颜色
        para.obj_fall_r = objAttrT.Get("obj_fall_r").As<Number>().Int32Value();
        para.obj_fall_g = objAttrT.Get("obj_fall_g").As<Number>().Int32Value();
        para.obj_fall_b = objAttrT.Get("obj_fall_b").As<Number>().Int32Value();
        // 范围最小值
        para.obj_range_min = objAttrT.Get("obj_range_min").As<Number>().Int32Value();
        // 范围最大值
        para.obj_range_max = objAttrT.Get("obj_range_max").As<Number>().Int32Value();
        // 角度
        para.obj_angle_range = objAttrT.Get("obj_angle_range").As<Number>().Int32Value();
        // 方向
        para.obj_rotation = objAttrT.Get("obj_rotation").As<Number>().Int32Value();
        // 宽度
        para.obj_width = objAttrT.Get("obj_width").As<Number>().Int32Value();
        // 进度间隔
        para.obj_progress_interval = objAttrT.Get("obj_progress_interval").As<Number>().Int32Value();

        // 进度刷图数组obj_progress_name[MAX_OBJ_PROGRESS_LEN][MAX_OBJ_PROGRESS_NAME_LEN]
        // -------------------------------------------------------------------------
        Array progressNameArray = objAttrT.Get("obj_progress_name").As<Array>();

        for (int i = 0; i < MAX_OBJ_PROGRESS_LEN && i < progressNameArray.Length(); i++)
        {
            String progressName = progressNameArray.Get(i).As<String>();
            string progressNameStr = progressName.Utf8Value();

            strncpy(para.obj_progress_name[i], progressNameStr.c_str(), MAX_OBJ_PROGRESS_NAME_LEN - 1);
            para.obj_progress_name[i][MAX_OBJ_PROGRESS_NAME_LEN - 1] = '\0';

            printf("%s \t", para.obj_progress_name[i]);
        }
        // ===============================================

        printf("\n===============================================\n");

        // 进度刷图规则obj_rule_name[MAX_OBJ_RULES_LEN][MAX_OBJ_RULES_NAME_LEN]
        // -------------------------------------------------------------------------
        Array ruleNameArray = objAttrT.Get("obj_rule_name").As<Array>();

        for (int i = 0; i < MAX_OBJ_RULES_LEN && i < ruleNameArray.Length(); i++)
        {
            String ruleName = ruleNameArray.Get(i).As<String>();
            string ruleNameStr = ruleName.Utf8Value();

            strncpy(para.obj_rule_name[i], ruleNameStr.c_str(), MAX_OBJ_RULES_NAME_LEN - 1);
            para.obj_rule_name[i][MAX_OBJ_RULES_NAME_LEN - 1] = '\0';

            printf("%s \t", para.obj_rule_name[i]);
        }
        // ===============================================

        // 折线图横向点数
        para.obj_point_x_num = objAttrT.Get("obj_point_x_num").As<Number>().Int32Value();
        // 折线图纵向点数
        para.obj_point_y_num = objAttrT.Get("obj_point_y_num").As<Number>().Int32Value();
        // 定时时间,0则不定时
        para.obj_time = objAttrT.Get("obj_time").As<Number>().Int32Value();
        // 对象内置的一些数据更新接口 obj_var[MAX_OBJ_VAR_LEN];
        Array value_obj_arr = objAttrT.Get("obj_var").As<Array>();
        for (int i = 0; i < MAX_OBJ_VAR_LEN && i < value_obj_arr.Length(); i++)
        {
            Napi::Value _val = value_obj_arr[i];
            para.obj_var[i] = _val.As<Napi::Number>().Int32Value();
        }
        // 对象事件
        para.obj_event = objAttrT.Get("obj_event").As<Number>().Int32Value();
        // 事件对应的操作
        para.obj_action = objAttrT.Get("obj_action").As<Number>().Int32Value();
        // obj_data[MAX_OBJ_DATA_LEN]; 对象的buff区,可通过此更新txt内容
        string obj_data = objAttrT.Get("obj_data").As<String>();
        strcpy_s(para.obj_data, obj_data.c_str());

        printf("$$$$==para.obj_data=%s ==$$$$$\n", para.obj_data);

        // obj_reserve[MAX_RESERVE_LEN]; 保留字
        string obj_reserve = objAttrT.Get("obj_reserve").As<String>();
        strcpy_s(para.obj_reserve, obj_reserve.c_str());

        // obj_url[MAX_OBJ_URL_LEN] 获取数据API地址
        string obj_url = objAttrT.Get("obj_url").As<String>();
        strcpy_s(para.obj_url, obj_url.c_str());

        // obj_field[MAX_OBJ_FIELD_LEN] 数据字段
        string obj_field = objAttrT.Get("obj_field").As<String>();
        strcpy_s(para.obj_field, obj_field.c_str());

        // obj_align_reserve[8] 保留字段
        string obj_align_reserve = objAttrT.Get("obj_align_reserve").As<String>();
        strcpy_s(para.obj_align_reserve, obj_align_reserve.c_str());

        // 是否是股票类型元素 obj_is_stock
        para.obj_is_stock = objAttrT.Get("obj_is_stock").As<Number>().Int32Value();

        vec_obj_attr_t.push_back(para);
    }

    // const auto res = generate_ui_handle(vec_obj_attr_t, pkgPath.c_str(), wifiName.c_str(), wifiPwd.c_str());

    hmi_init();
    hmi_config_wifi_info(wifiName.c_str(), wifiPwd.c_str());

    hmi_page_t *page = hmi_page_get_default(0);
    for (const auto &obj : vec_obj_attr_t)
    {
        hmi_add_obj(page, obj);
        printf("\n#obj.obj_id=%d\t obj.obj_data=%s###: %d\n", obj.obj_id, obj.obj_data);
    }
    const auto res = hmi_packet_file(page, pkgPath.c_str());

    Number result = Number::New(env, res);

    return result;
}

Napi::Value HANDLE::unpacketFile(const Napi::CallbackInfo &info)
{
    auto env = info.Env();

    string filepath = info[0].As<String>();
    string out_path = info[1].As<String>();

    const auto res = hmi_unpacket_file(filepath.c_str(), out_path.c_str());

    Number result = Number::New(env, res);

    return result;
}

Napi::Value HANDLE::updateScreenData(const Napi::CallbackInfo &info)
{
    auto env = info.Env();

    if (info.Length() < 1 || !info[0].IsArray())
    {
        TypeError::New(env, "Array expected").ThrowAsJavaScriptException();
        return env.Null();
    }

    Array objAttrTs = info[0].As<Array>();
    vector<obj_attr_t> vec_obj_attr_t;

    for (uint32_t i = 0; i < objAttrTs.Length(); i++)
    {
        Napi::Value value = objAttrTs.Get(i);
        Napi::Object objAttrT = value.As<Napi::Object>();

        if (!objAttrT.IsObject())
        {
            TypeError::New(env, "Object expected").ThrowAsJavaScriptException();
            return env.Null();
        }

        obj_attr_t para;
        memset(&para, 0, sizeof(obj_attr_t));

        // 对象id,要唯一
        para.obj_id = objAttrT.Get("obj_id").As<Number>().Int32Value();
        // UI类型
        para.obj_type = objAttrT.Get("obj_type").As<Number>().Int32Value();
        // 对象名字,唯一
        std::string obj_name = objAttrT.Get("obj_name").As<String>();
        strcpy_s(para.obj_name, obj_name.c_str());

        // 横向起点坐标
        para.obj_x = objAttrT.Get("obj_x").As<Number>().Int32Value();
        // 纵向起点坐标
        para.obj_y = objAttrT.Get("obj_y").As<Number>().Int32Value();
        // 宽度
        para.obj_w = objAttrT.Get("obj_w").As<Number>().Int32Value();
        // 高度
        para.obj_h = objAttrT.Get("obj_h").As<Number>().Int32Value();
        // 主透明度
        // std::string obj_opa = objAttrT.Get("obj_opa").As<String>();
        para.obj_opa = objAttrT.Get("obj_opa").As<Number>().Int32Value();
        // 颜色
        para.obj_r = objAttrT.Get("obj_r").As<Number>().Int32Value();
        para.obj_g = objAttrT.Get("obj_g").As<Number>().Int32Value();
        para.obj_b = objAttrT.Get("obj_b").As<Number>().Int32Value();
        // 次透明度
        para.obj_opa_2 = objAttrT.Get("obj_opa_2").As<Number>().Int32Value();
        para.obj_g_2 = objAttrT.Get("obj_g_2").As<Number>().Int32Value();
        para.obj_b_2 = objAttrT.Get("obj_b_2").As<Number>().Int32Value();
        // 字体大小
        para.obj_font_size = objAttrT.Get("obj_font_size").As<Number>().Int32Value();
        // 字体名字
        string obj_font_name = objAttrT.Get("obj_font_name").As<String>();
        strcpy_s(para.obj_font_name, obj_font_name.c_str());
        // 字体颜色
        para.obj_font_r = objAttrT.Get("obj_font_r").As<Number>().Int32Value();
        para.obj_font_g = objAttrT.Get("obj_font_g").As<Number>().Int32Value();
        para.obj_font_b = objAttrT.Get("obj_font_b").As<Number>().Int32Value();
        // 范围最小值
        para.obj_range_min = objAttrT.Get("obj_range_min").As<Number>().Int32Value();
        // 范围最大值
        para.obj_range_max = objAttrT.Get("obj_range_max").As<Number>().Int32Value();
        // 角度
        para.obj_angle_range = objAttrT.Get("obj_angle_range").As<Number>().Int32Value();
        // 方向
        para.obj_rotation = objAttrT.Get("obj_rotation").As<Number>().Int32Value();
        // 宽度
        para.obj_width = objAttrT.Get("obj_width").As<Number>().Int32Value();
        // 折线图横向点数
        para.obj_point_x_num = objAttrT.Get("obj_point_x_num").As<Number>().Int32Value();
        // 折线图纵向点数
        para.obj_point_y_num = objAttrT.Get("obj_point_y_num").As<Number>().Int32Value();
        // 定时时间,0则不定时
        para.obj_time = objAttrT.Get("obj_time").As<Number>().Int32Value();
        // 对象内置的一些数据更新接口 obj_var[MAX_OBJ_VAR_LEN];
        // para.obj_var = objAttrT.Get("obj_var").As<Number>().Int32Value();

        Array value_obj_arr = objAttrT.Get("obj_var").As<Array>();
        // unsigned int obj_var[MAX_OBJ_VAR_LEN];
        for (int i = 0; i < MAX_OBJ_VAR_LEN && i < value_obj_arr.Length(); i++)
        {
            Napi::Value val = value_obj_arr[i];
            // obj_var[i] = val.As<Number>().Int32Value();
            para.obj_var[i] = val.As<Napi::Number>().Int32Value();
        }
        // para.obj_var = obj_var;

        // 对象事件
        para.obj_event = objAttrT.Get("obj_event").As<Number>().Int32Value();
        // 事件对应的操作
        para.obj_action = objAttrT.Get("obj_action").As<Number>().Int32Value();
        // obj_data[MAX_OBJ_DATA_LEN]; 对象的buff区,可通过此更新txt内容
        string obj_data = objAttrT.Get("obj_data").As<String>();
        strcpy_s(para.obj_data, obj_data.c_str());

        // obj_reserve[MAX_RESERVE_LEN]; 保留字
        string obj_reserve = objAttrT.Get("obj_reserve").As<String>();
        strcpy_s(para.obj_reserve, obj_reserve.c_str());

        // obj_url[MAX_OBJ_URL_LEN] 获取数据API地址
        string obj_url = objAttrT.Get("obj_url").As<String>();
        strcpy_s(para.obj_url, obj_url.c_str());

        // obj_field[MAX_OBJ_FIELD_LEN] 数据字段
        string obj_field = objAttrT.Get("obj_field").As<String>();
        strcpy_s(para.obj_field, obj_field.c_str());

        // obj_align_reserve[8] 保留字段
        string obj_align_reserve = objAttrT.Get("obj_align_reserve").As<String>();
        strcpy_s(para.obj_align_reserve, obj_align_reserve.c_str());

        // unsigned int* obj_font;

        vec_obj_attr_t.push_back(para);
    }

    // const auto res = hmi_batch_update_screen_data(vec_obj_attr_t);

    unsigned int ret = 0;
    unsigned int page_id = 0;
    unsigned int elem_id = 0;
    unsigned int data_type = 0;
    unsigned int elem_data_len = 0;

    char control_buff[512];
    memset(control_buff, 0, sizeof(control_buff));
    // 更新obj_data
    for (const auto &obj : vec_obj_attr_t)
    {

        page_id = 0;
        elem_id = obj.obj_id;

        data_type = HMI_OBJ_DATA_BUFF;

        elem_data_len = 3;

        control_buff[0] = ((page_id >> 0) & 0xff);
        control_buff[1] = ((page_id >> 8) & 0xff);

        control_buff[2] = ((elem_id >> 0) & 0xff);
        control_buff[3] = ((elem_id >> 8) & 0xff);

        control_buff[4] = ((data_type >> 0) & 0xff);
        control_buff[5] = ((data_type >> 8) & 0xff);

        control_buff[6] = ((elem_data_len >> 0) & 0xff);
        control_buff[7] = ((elem_data_len >> 8) & 0xff);

        sprintf(control_buff + 8, "%s", obj.obj_data);

        ret = hid_io_control(_hidHandle, CMD_ELEM_UPDATE, " ", control_buff, sizeof(control_buff));
    }

    // 更新obj_var
    for (const auto &obj : vec_obj_attr_t)
    {

        page_id = 0;
        elem_id = obj.obj_id;

        data_type = HMI_OBJ_DATA_DEFAULT;

        elem_data_len = 3;

        control_buff[0] = ((page_id >> 0) & 0xff);
        control_buff[1] = ((page_id >> 8) & 0xff);

        control_buff[2] = ((elem_id >> 0) & 0xff);
        control_buff[3] = ((elem_id >> 8) & 0xff);

        control_buff[4] = ((data_type >> 0) & 0xff);
        control_buff[5] = ((data_type >> 8) & 0xff);

        control_buff[6] = ((elem_data_len >> 0) & 0xff);
        control_buff[7] = ((elem_data_len >> 8) & 0xff);

        control_buff[8] = ((obj.obj_var[0] >> 0) & 0xff);
        control_buff[9] = ((obj.obj_var[0] >> 7) & 0xff);

        ret = hid_io_control(_hidHandle, CMD_ELEM_UPDATE, " ", control_buff, sizeof(control_buff));
    }

    Number result = Number::New(env, ret);

    return result;
}

Napi::Value HANDLE::updateScreenDataAsync(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    // 创建 Promise 的执行器函数
    auto executor = [&](Promise::Deferred deferred)
    {
        if (info.Length() < 1 || !info[0].IsArray())
        {
            TypeError::New(env, "Array expected").ThrowAsJavaScriptException();
            return env.Null();
        }

        Array objAttrTs = info[0].As<Array>();
        vector<obj_attr_t> vec_obj_attr_t;

        for (uint32_t i = 0; i < objAttrTs.Length(); i++)
        {
            Napi::Value value = objAttrTs.Get(i);
            Napi::Object objAttrT = value.As<Napi::Object>();

            if (!objAttrT.IsObject())
            {
                TypeError::New(env, "Object expected").ThrowAsJavaScriptException();
                return env.Null();
            }

            obj_attr_t para;
            memset(&para, 0, sizeof(obj_attr_t));

            // 对象id,要唯一
            para.obj_id = objAttrT.Get("obj_id").As<Number>().Int32Value();
            // UI类型
            para.obj_type = objAttrT.Get("obj_type").As<Number>().Int32Value();
            // 对象名字,唯一
            std::string obj_name = objAttrT.Get("obj_name").As<String>();
            strcpy_s(para.obj_name, obj_name.c_str());

            // 横向起点坐标
            para.obj_x = objAttrT.Get("obj_x").As<Number>().Int32Value();
            // 纵向起点坐标
            para.obj_y = objAttrT.Get("obj_y").As<Number>().Int32Value();
            // 宽度
            para.obj_w = objAttrT.Get("obj_w").As<Number>().Int32Value();
            // 高度
            para.obj_h = objAttrT.Get("obj_h").As<Number>().Int32Value();
            // 主透明度
            // std::string obj_opa = objAttrT.Get("obj_opa").As<String>();
            para.obj_opa = objAttrT.Get("obj_opa").As<Number>().Int32Value();
            // 颜色
            para.obj_r = objAttrT.Get("obj_r").As<Number>().Int32Value();
            para.obj_g = objAttrT.Get("obj_g").As<Number>().Int32Value();
            para.obj_b = objAttrT.Get("obj_b").As<Number>().Int32Value();
            // 次透明度
            para.obj_opa_2 = objAttrT.Get("obj_opa_2").As<Number>().Int32Value();
            para.obj_g_2 = objAttrT.Get("obj_g_2").As<Number>().Int32Value();
            para.obj_b_2 = objAttrT.Get("obj_b_2").As<Number>().Int32Value();
            // 字体大小
            para.obj_font_size = objAttrT.Get("obj_font_size").As<Number>().Int32Value();
            // 字体名字
            string obj_font_name = objAttrT.Get("obj_font_name").As<String>();
            strcpy_s(para.obj_font_name, obj_font_name.c_str());
            // 字体颜色
            para.obj_font_r = objAttrT.Get("obj_font_r").As<Number>().Int32Value();
            para.obj_font_g = objAttrT.Get("obj_font_g").As<Number>().Int32Value();
            para.obj_font_b = objAttrT.Get("obj_font_b").As<Number>().Int32Value();
            // 范围最小值
            para.obj_range_min = objAttrT.Get("obj_range_min").As<Number>().Int32Value();
            // 范围最大值
            para.obj_range_max = objAttrT.Get("obj_range_max").As<Number>().Int32Value();
            // 角度
            para.obj_angle_range = objAttrT.Get("obj_angle_range").As<Number>().Int32Value();
            // 方向
            para.obj_rotation = objAttrT.Get("obj_rotation").As<Number>().Int32Value();
            // 宽度
            para.obj_width = objAttrT.Get("obj_width").As<Number>().Int32Value();
            // 折线图横向点数
            para.obj_point_x_num = objAttrT.Get("obj_point_x_num").As<Number>().Int32Value();
            // 折线图纵向点数
            para.obj_point_y_num = objAttrT.Get("obj_point_y_num").As<Number>().Int32Value();
            // 定时时间,0则不定时
            para.obj_time = objAttrT.Get("obj_time").As<Number>().Int32Value();
            // 对象内置的一些数据更新接口 obj_var[MAX_OBJ_VAR_LEN];
            // para.obj_var = objAttrT.Get("obj_var").As<Number>().Int32Value();

            Array value_obj_arr = objAttrT.Get("obj_var").As<Array>();
            // unsigned int obj_var[MAX_OBJ_VAR_LEN];
            for (int i = 0; i < MAX_OBJ_VAR_LEN && i < value_obj_arr.Length(); i++)
            {
                Napi::Value val = value_obj_arr[i];
                // obj_var[i] = val.As<Number>().Int32Value();
                para.obj_var[i] = val.As<Napi::Number>().Int32Value();
            }
            // para.obj_var = obj_var;

            // 对象事件
            para.obj_event = objAttrT.Get("obj_event").As<Number>().Int32Value();
            // 事件对应的操作
            para.obj_action = objAttrT.Get("obj_action").As<Number>().Int32Value();
            // obj_data[MAX_OBJ_DATA_LEN]; 对象的buff区,可通过此更新txt内容
            string obj_data = objAttrT.Get("obj_data").As<String>();
            strcpy_s(para.obj_data, obj_data.c_str());

            // obj_reserve[MAX_RESERVE_LEN]; 保留字
            string obj_reserve = objAttrT.Get("obj_reserve").As<String>();
            strcpy_s(para.obj_reserve, obj_reserve.c_str());

            // obj_url[MAX_OBJ_URL_LEN] 获取数据API地址
            string obj_url = objAttrT.Get("obj_url").As<String>();
            strcpy_s(para.obj_url, obj_url.c_str());

            // obj_field[MAX_OBJ_FIELD_LEN] 数据字段
            string obj_field = objAttrT.Get("obj_field").As<String>();
            strcpy_s(para.obj_field, obj_field.c_str());

            // obj_align_reserve[8] 保留字段
            string obj_align_reserve = objAttrT.Get("obj_align_reserve").As<String>();
            strcpy_s(para.obj_align_reserve, obj_align_reserve.c_str());

            // unsigned int* obj_font;

            vec_obj_attr_t.push_back(para);
        }

        // int success = hmi_batch_update_screen_data(vec_obj_attr_t);
        // 更新数据start
        unsigned int ret = 0;
        unsigned int page_id = 0;
        unsigned int elem_id = 0;
        unsigned int data_type = 0;
        unsigned int elem_data_len = 0;

        char control_buff[512];
        memset(control_buff, 0, sizeof(control_buff));
        // 更新obj_data
        for (const auto &obj : vec_obj_attr_t)
        {

            page_id = 0;
            elem_id = obj.obj_id;

            data_type = HMI_OBJ_DATA_BUFF;

            elem_data_len = 3;

            control_buff[0] = ((page_id >> 0) & 0xff);
            control_buff[1] = ((page_id >> 8) & 0xff);

            control_buff[2] = ((elem_id >> 0) & 0xff);
            control_buff[3] = ((elem_id >> 8) & 0xff);

            control_buff[4] = ((data_type >> 0) & 0xff);
            control_buff[5] = ((data_type >> 8) & 0xff);

            control_buff[6] = ((elem_data_len >> 0) & 0xff);
            control_buff[7] = ((elem_data_len >> 8) & 0xff);

            sprintf(control_buff + 8, "%s", obj.obj_data);

            ret = hid_io_control(_hidHandle, CMD_ELEM_UPDATE, " ", control_buff, sizeof(control_buff));
        }

        // 更新obj_var
        for (const auto &obj : vec_obj_attr_t)
        {

            page_id = 0;
            elem_id = obj.obj_id;

            data_type = HMI_OBJ_DATA_DEFAULT;

            elem_data_len = 3;

            control_buff[0] = ((page_id >> 0) & 0xff);
            control_buff[1] = ((page_id >> 8) & 0xff);

            control_buff[2] = ((elem_id >> 0) & 0xff);
            control_buff[3] = ((elem_id >> 8) & 0xff);

            control_buff[4] = ((data_type >> 0) & 0xff);
            control_buff[5] = ((data_type >> 8) & 0xff);

            control_buff[6] = ((elem_data_len >> 0) & 0xff);
            control_buff[7] = ((elem_data_len >> 8) & 0xff);

            control_buff[8] = ((obj.obj_var[0] >> 0) & 0xff);
            control_buff[9] = ((obj.obj_var[0] >> 7) & 0xff);

            ret = hid_io_control(_hidHandle, CMD_ELEM_UPDATE, " ", control_buff, sizeof(control_buff));
        }

        // 更新数据 end
        if (ret == 0)
        {
            deferred.Resolve(Number::New(env, ret));
        }
        else
        {
            Error::New(env, "Data update error!").ThrowAsJavaScriptException();
            deferred.Reject(env.Null()); // 使用 Null 作为拒绝的原因
        }
    };

    // 创建 Promise 对象并返回
    Promise::Deferred deferred = Promise::Deferred::New(env);
    Promise promise = deferred.Promise();

    // 调用执行器函数，开始异步操作
    executor(deferred);

    // 返回 Promise 对象给 JavaScript
    return promise;
}

static void
deinitialize(void *)
{
    if (hid_exit())
    {
        // Process is exiting, no need to log? TODO
        // Napi::TypeError::New(env, "cannot uninitialize hidapi (hid_exit failed)").ThrowAsJavaScriptException();
        return;
    }
}
void HANDLE::Initialize(Napi::Env &env, Napi::Object &exports)
{
    if (hid_init())
    {
        Napi::TypeError::New(env, "cannot initialize hidapi (hid_init failed)").ThrowAsJavaScriptException();
        return;
    }

    napi_add_env_cleanup_hook(env, deinitialize, nullptr);

    Napi::Function ctor = DefineClass(env, "HANDLE",
                                      {
                                          InstanceMethod("close", &HANDLE::close),
                                          InstanceMethod("read", &HANDLE::read),
                                          InstanceMethod("write", &HANDLE::write, napi_enumerable),
                                          InstanceMethod("getFeatureReport", &HANDLE::getFeatureReport, napi_enumerable),
                                          InstanceMethod("sendFeatureReport", &HANDLE::sendFeatureReport, napi_enumerable),
                                          InstanceMethod("setNonBlocking", &HANDLE::setNonBlocking, napi_enumerable),
                                          InstanceMethod("readSync", &HANDLE::readSync, napi_enumerable),
                                          InstanceMethod("readTimeout", &HANDLE::readTimeout, napi_enumerable),
                                          InstanceMethod("getDeviceInfo", &HANDLE::getDeviceInfo, napi_enumerable),
                                          InstanceMethod("writeStr", &HANDLE::writeStr, napi_enumerable),
                                          InstanceMethod("writeHex", &HANDLE::writeHex, napi_enumerable),
                                          InstanceMethod("writeFile", &HANDLE::writeFile, napi_enumerable),
                                          InstanceMethod("writeFileAsync", &HANDLE::writeFileAsync, napi_enumerable),
                                          InstanceMethod("sendWifiInfo", &HANDLE::sendWifiInfo, napi_enumerable),
                                          InstanceMethod("generateUI", &HANDLE::generateUI, napi_enumerable),
                                          InstanceMethod("unpacketFile", &HANDLE::unpacketFile, napi_enumerable),
                                          InstanceMethod("updateScreenData", &HANDLE::updateScreenData, napi_enumerable),
                                          InstanceMethod("updateScreenDataAsync", &HANDLE::updateScreenDataAsync, napi_enumerable),
                                      });

    exports.Set("HANDLE", ctor);
    exports.Set("devices", Napi::Function::New(env, &HANDLE::devices));
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    HANDLE::Initialize(env, exports);

    return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)