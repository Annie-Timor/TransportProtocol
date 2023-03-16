#include <stdio.h>
#include <windows.h>
#include <winreg.h>

#include "com_port.h"
//==========================================================================
/*
 * 功 能: 通过访问注册表获得串口号。
 * 参 数：二维数组用来存放返回数组
 * 返回值：返回-1失败，返回0成功
 */
int GetComNum(char strSerialList[][25], int *comNum)
{
    char Name[25];
    int i = 0;
    unsigned char szPortName[25];
    long Status = 0.0;
    int dwIndex = 0;
    long unsigned int dwName;
    long unsigned int dwSizeofPortName;
    long unsigned int Type;
    HKEY hKey;                                                                                                      // 返回注册表项的句柄
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DEVICEMAP\\SERIALCOMM\\", 0, KEY_READ, &hKey) != ERROR_SUCCESS) // 打开注册表
        return -1;
    dwName = sizeof(Name);
    dwSizeofPortName = sizeof(szPortName);
    while ((Status == ERROR_SUCCESS) || (Status == ERROR_MORE_DATA))
    {
        Status = RegEnumValue(hKey, dwIndex++, Name, &dwName, NULL, &Type, szPortName, &dwSizeofPortName); // 列举注册表的值
        if ((Status == ERROR_SUCCESS) || (Status == ERROR_MORE_DATA))
        {
            strcpy(strSerialList[i], (const char *)szPortName); // 串口字符串保存
            i++;
        }
        dwName = sizeof(Name);
        dwSizeofPortName = sizeof(szPortName);
    }
    *comNum = i;
    RegCloseKey(hKey); // 关闭注册表
    return 0;
}
