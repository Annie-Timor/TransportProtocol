#include <stdio.h>
#include <windows.h>
#include <winreg.h>

#include "com_port.h"
//==========================================================================
/*
 * �� ��: ͨ������ע����ô��ںš�
 * �� ������ά����������ŷ�������
 * ����ֵ������-1ʧ�ܣ�����0�ɹ�
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
    HKEY hKey;                                                                                                      // ����ע�����ľ��
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DEVICEMAP\\SERIALCOMM\\", 0, KEY_READ, &hKey) != ERROR_SUCCESS) // ��ע���
        return -1;
    dwName = sizeof(Name);
    dwSizeofPortName = sizeof(szPortName);
    while ((Status == ERROR_SUCCESS) || (Status == ERROR_MORE_DATA))
    {
        Status = RegEnumValue(hKey, dwIndex++, Name, &dwName, NULL, &Type, szPortName, &dwSizeofPortName); // �о�ע����ֵ
        if ((Status == ERROR_SUCCESS) || (Status == ERROR_MORE_DATA))
        {
            strcpy(strSerialList[i], (const char *)szPortName); // �����ַ�������
            i++;
        }
        dwName = sizeof(Name);
        dwSizeofPortName = sizeof(szPortName);
    }
    *comNum = i;
    RegCloseKey(hKey); // �ر�ע���
    return 0;
}
