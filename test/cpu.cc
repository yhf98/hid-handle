/*
 * @Author: 姚恒锋 1921934563@qq.com
 * @Date: 2023-05-25 13:41:01
 * @LastEditors: 姚恒锋 1921934563@qq.com
 * @LastEditTime: 2023-06-21 13:54:31
 * @FilePath: \hid-handle\test\cpu.cc
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <iostream>

int main() {
    ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
    int numProcessors;
    HANDLE self;

    SYSTEM_INFO sysInfo;
    FILETIME ftSysIdle, ftSysKernel, ftSysUser;

    GetSystemInfo(&sysInfo);
    numProcessors = sysInfo.dwNumberOfProcessors;

    self = GetCurrentProcess();
    GetProcessTimes(self, &ftSysIdle, &ftSysKernel, &ftSysUser);
    lastCPU.QuadPart = ftSysKernel.dwLowDateTime + ftSysUser.dwLowDateTime;
    lastSysCPU.QuadPart = ftSysKernel.dwHighDateTime + ftSysUser.dwHighDateTime;
    lastUserCPU.QuadPart = ftSysUser.dwHighDateTime + ftSysUser.dwLowDateTime;

    while (true) {
        GetProcessTimes(self, &ftSysIdle, &ftSysKernel, &ftSysUser);
        ULARGE_INTEGER cpu;
        cpu.QuadPart = ftSysKernel.dwLowDateTime + ftSysUser.dwLowDateTime;
        ULARGE_INTEGER sysCPU;
        sysCPU.QuadPart = ftSysKernel.dwHighDateTime + ftSysUser.dwHighDateTime;
        ULARGE_INTEGER userCPU;
        userCPU.QuadPart = ftSysUser.dwHighDateTime + ftSysUser.dwLowDateTime;

        double cpuUsage = (double)(cpu.QuadPart - lastCPU.QuadPart) / ((sysCPU.QuadPart - lastSysCPU.QuadPart) + (userCPU.QuadPart - lastUserCPU.QuadPart));
        std::cout << "CPU usage: " << cpuUsage * 100 << "%" << std::endl;

        lastCPU = cpu;
        lastSysCPU = sysCPU;
        lastUserCPU = userCPU;

        Sleep(1000);
    }
    return 0;
}
