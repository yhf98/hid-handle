/*
 * @Author: yaohengfeng 1921934563@qq.com
 * @Date: 2023-01-13 10:58:19
 * @LastEditors: 姚恒锋 1921934563@qq.com
 * @LastEditTime: 2023-07-25 18:39:58
 * @FilePath: \hid-handle\src\export.cc
 * @Description: 对外导出接口
 */
#include <napi.h>
#include <node_api.h>
#include <tuple>
#include <string.h>
#include <vector>

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

    char *cstr = new char[arg2.size() + 1];
    std::copy(arg2.begin(), arg2.end(), cstr);
    cstr[arg2.size()] = '\0';

    const auto res = hid_io_control_handle(arg0, arg1.c_str(), cstr, arg3);
    Number result = Number::New(env, res);

    delete[] cstr;

    return result;
}

Value hmiSendWifiInfoHandleJs(const CallbackInfo &info)
{
    auto env = info.Env();

    string wifiName = info[0].As<String>();
    string wifiPassword = info[1].As<String>();

    const auto res = hmi_send_wifi_info_handle(wifiName.c_str(), wifiPassword.c_str());
    Number result = Number::New(env, res);

    return result;
}

Value generateUIHandleJs(const CallbackInfo &info)
{
    auto env = info.Env();

    if (info.Length() < 1 || !info[0].IsArray())
    {
        TypeError::New(env, "Array expected").ThrowAsJavaScriptException();
        return env.Null();
    }

    Array objAttrTs = info[0].As<Array>();

    string pkgPath = info[1].As<String>();

    vector<obj_attr_t> vec_obj_attr_t;

    for (uint32_t i = 0; i < objAttrTs.Length(); i++)
    {
        Value value = objAttrTs.Get(i);
        Object objAttrT = value.As<Object>();

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
            Value val = value_obj_arr[i];
            // obj_var[i] = val.As<Number>().Int32Value();
            para.obj_var[i] = val.As<Number>().Int32Value();
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

        vec_obj_attr_t.push_back(para);
    }

    const auto res = generate_ui_handle(vec_obj_attr_t, pkgPath.c_str());
    Number result = Number::New(env, res);

    return result;
}

Value hmiPageUpdateElemVarHandleJs(const CallbackInfo &info)
{
    auto env = info.Env();

    unsigned int id = 0;
    obj_attr_t para;
    memset(&para, 0, sizeof(obj_attr_t));
    para.obj_id = 4;
    strcpy_s(para.obj_name, "label15");
    // para.obj_name = "label15";
    para.obj_type = 6;

    para.obj_x = 0;
    para.obj_y = 270;
    para.obj_w = 192;
    para.obj_h = 20;

    para.obj_opa = 255;
    para.obj_r = 255;
    para.obj_g = 0;
    para.obj_b = 0;

    para.obj_opa_2 = 255;
    para.obj_r_2 = 0;
    para.obj_g_2 = 0;
    para.obj_b_2 = 255;

    para.obj_font_size = 0;
    para.obj_font_r = 0;
    para.obj_font_g = 255;
    para.obj_font_b = 0;

    para.obj_range_min = 0;
    para.obj_range_max = 100;
    para.obj_angle_range = 360;
    para.obj_rotation = 90;
    para.obj_width = 20;

    para.obj_point_x_num = 0;
    para.obj_point_y_num = 0;

    para.obj_time = 1000;

    para.obj_var[0] = 10;

    para.obj_event = 0;
    para.obj_action = 0;
    strcpy_s(para.obj_data, "磁盘");
    // para.obj_data = "磁盘";
    // para.obj_reserve = "";
    strcpy_s(para.obj_reserve, "");

    const auto res = hmi_page_update_elem_var_handle(id, para);

    Number result = Number::New(env, res);

    return result;
}

Value hmiUnpacketFileHandleJs(const CallbackInfo &info)
{
    auto env = info.Env();

    string filepath = info[0].As<String>();
    string out_path = info[1].As<String>();

    const auto res = hmi_unpacket_file_handle(filepath.c_str(), out_path.c_str());

    Number result = Number::New(env, res);

    return result;
}

Value hmiUpdateObjVarHandleJs(const CallbackInfo &info)
{
    auto env = info.Env();

    const auto res = hmi_update_obj_var_handle();

    Number result = Number::New(env, res);

    return result;
}

Value hmiUpdateScreenDataJs(const CallbackInfo &info)
{
    auto env = info.Env();

    unsigned elem_id = info[0].As<Number>().Int32Value();
    string data = info[1].As<String>();
    unsigned data_len = info[2].As<Number>().Int32Value();

    const auto res = hmi_update_screen_data(elem_id, data.c_str());

    Number result = Number::New(env, res);

    return result;
}

Value hmiBatchUpdateScreenDataJs(const CallbackInfo &info)
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
        Value value = objAttrTs.Get(i);
        Object objAttrT = value.As<Object>();

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
            Value val = value_obj_arr[i];
            // obj_var[i] = val.As<Number>().Int32Value();
            para.obj_var[i] = val.As<Number>().Int32Value();
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

        vec_obj_attr_t.push_back(para);
    }

    const auto res = hmi_batch_update_screen_data(vec_obj_attr_t);

    Number result = Number::New(env, res);

    return result;
}

Value hmiCreateObjTestJs(const CallbackInfo &info)
{
    auto env = info.Env();

    const auto res = hmi_create_obj_test_handle();

    Number result = Number::New(env, res);

    return result;
}

Value hmiUnpacketHandleJs(const CallbackInfo &info)
{
    auto env = info.Env();

    const auto res = hmi_update_obj_var_handle();

    Number result = Number::New(env, res);

    return result;
}

Value testHandleJs(const CallbackInfo &info)
{
    auto env = info.Env();

    const auto res = test_handle();

    Number result = Number::New(env, res);

    return result;
}

Promise TestAsync(const CallbackInfo &info)
{
    Env env = info.Env();

    // 创建 Promise 的执行器函数
    auto executor = [&](Promise::Deferred deferred)
    {
        // 异步操作完成后，根据结果决定是解决还是拒绝 Promise
        bool success = async_test_handle();
        if (success == 0)
        {
            printf("\nSuccess\n");
            deferred.Resolve(String::New(env, "Async operation completed"));
        }
        else
        {
            Error::New(env, "Async operation failed").ThrowAsJavaScriptException();
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

Promise HidWriteFileHandleAsync(const CallbackInfo &info)
{
    Env env = info.Env();

    // 创建 Promise 的执行器函数
    auto executor = [&](Promise::Deferred deferred)
    {
        string arg0 = info[0].As<String>();
        string arg1 = info[1].As<String>();
        int arg2 = info[2].As<Number>().Int32Value();
        // 异步操作完成后，根据结果决定是解决还是拒绝 Promise
        int success = hid_write_file_handle(arg0.c_str(), arg1.c_str(), arg2);
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
    Promise::Deferred deferred = Promise::Deferred::New(env);
    Promise promise = deferred.Promise();

    // 调用执行器函数，开始异步操作
    executor(deferred);

    // 返回 Promise 对象给 JavaScript
    return promise;
}

Promise hmiSendWifiInfoHandleAsync(const CallbackInfo &info)
{
    Env env = info.Env();

    // 创建 Promise 的执行器函数
    auto executor = [&](Promise::Deferred deferred)
    {
        string wifiName = info[0].As<String>();
        string wifiPassword = info[1].As<String>();
        // 异步操作完成后，根据结果决定是解决还是拒绝 Promise
        int success = hmi_send_wifi_info_handle(wifiName.c_str(), wifiPassword.c_str());
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
    Promise::Deferred deferred = Promise::Deferred::New(env);
    Promise promise = deferred.Promise();

    // 调用执行器函数，开始异步操作
    executor(deferred);

    // 返回 Promise 对象给 JavaScript
    return promise;
}


Object Init(Env env, Object exports)
{
    exports.Set("hid_write_file_handle", Function::New(env, HidWriteFileHandleJs));
    exports.Set("hid_write_buff_handle", Function::New(env, HidWriteBuffHandleJs));
    exports.Set("hid_io_control_handle", Function::New(env, hidIOControlHandleJs));
    exports.Set("hmi_send_wifi_info_handle", Function::New(env, hmiSendWifiInfoHandleJs));
    exports.Set("hmi_send_wifi_info_async_handle", Function::New(env, hmiSendWifiInfoHandleAsync));
    exports.Set("generate_ui_handle", Function::New(env, generateUIHandleJs));
    exports.Set("hmi_page_update_elem_var_handle", Function::New(env, hmiPageUpdateElemVarHandleJs));
    exports.Set("hmi_unpacket_file_handle", Function::New(env, hmiUnpacketFileHandleJs));
    exports.Set("hmi_update_obj_var_handle", Function::New(env, hmiUpdateObjVarHandleJs));
    exports.Set("hmi_update_screen_data", Function::New(env, hmiUpdateScreenDataJs));
    exports.Set("hmi_batch_update_screen_data", Function::New(env, hmiBatchUpdateScreenDataJs));
    exports.Set("hmi_create_obj_test_handle", Function::New(env, hmiCreateObjTestJs));
    exports.Set("test_handle", Function::New(env, testHandleJs));
    exports.Set("testAsync",Function::New(env, TestAsync));
    exports.Set("hid_write_file_async_handle", Function::New(env, HidWriteFileHandleAsync));

    return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)
