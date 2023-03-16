
#include "ui_setting.h"
#include "com_port.h"
#include "main.h"
#include <ansi_c.h>
#include <cvirte.h>
#include <rs232.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <userint.h>
#include <windows.h>

extern int panelHandle;
int com_port = 0;
int baud_rate = 0;
int com_open_flag = 0;

static int Label2int(char *label)
{
    int val = 0;
    if (label[4] == '\0')
    {
        val = label[3] - '0';
    }
    else if (label[5] == '\0')
    {
        val = (label[3] - '0') * 10 + (label[4] - '0');
    }

    return val;
}

void UI_ScanComPort(void)
{
    char seriallist[10][25] = {0};
    int comnum = 0;
    int ret = 0;

    ret = GetComNum(seriallist, &comnum);
    ClearListCtrl(panelHandle, PANEL_SERIAL_PORT);
    for (int i = 0; i < comnum; i++)
    {
        if (i == 0)
        {
            InsertListItem(panelHandle, PANEL_SERIAL_PORT, 0, seriallist[i], Label2int(seriallist[i]));
        }
        else
        {
            InsertListItem(panelHandle, PANEL_SERIAL_PORT, -1, seriallist[i], Label2int(seriallist[i]));
        }
    }
}

static int OpenSerialPort(void)
{

    int ret_val = 0;

    GetCtrlVal(panelHandle, PANEL_SERIAL_PORT, (int *)&com_port);
    GetCtrlVal(panelHandle, PANEL_BAUD_RATE, (int *)&baud_rate);

    /* 	Open and Configure Com port */
    ret_val = OpenComConfig(com_port, "", baud_rate, 0, 8, 1, 2048, 2048);
    if (ret_val < 0)
    {
        return -1;
    }
    /* 	Turn off Hardware handshaking (loopback test will not function with it on) */
    SetCTSMode(com_port, LWRS_HWHANDSHAKE_OFF);

    /* Sets timeout limit for input/output operations. */
    SetComTime(com_port, 2.0);
    // InstallComCallback(com_port, LWRS_RECEIVE, 2, 0, Event_Char_Func, 0); //�󶨴����¼��ص�����

    /* 	Make sure Serial buffers are empty */
    FlushInQ(com_port);
    FlushOutQ(com_port);

    return 1;
}

void UI_OpenSerialPort(void)
{
    char text[20] = {0};
    GetCtrlAttribute(panelHandle, PANEL_OPEN_PORT, ATTR_LABEL_TEXT, (char *)text);
    if (!strncmp(text, "�򿪴���", 8))
    {
        if (OpenSerialPort() > 0)
        {
            com_open_flag = 1;
            SetCtrlAttribute(panelHandle, PANEL_OPEN_PORT, ATTR_LABEL_TEXT, "�رմ���");
        }
    }
    else if (!strncmp(text, "�رմ���", 8))
    {
        CloseCom(com_port);
        com_open_flag = 0;
        SetCtrlAttribute(panelHandle, PANEL_OPEN_PORT, ATTR_LABEL_TEXT, "�򿪴���");
    }
}

void UI_SelectFile(int control)
{
    char path_name[512] = {0};
    int select_fold = 0;
    GetCtrlVal(panelHandle, PANEL_SELECT_FOLD, &select_fold);

    if (select_fold)
    {
        // DirSelectPopup("", "��ѡ���ļ���", 1, 1, path_name);
        DirSelectPopupEx("", "��ѡ���ļ���", path_name);
    }
    else
    {
        // FileSelectPopup("", "*.txt", "*.*", "��ѡ��Ŀ���ļ�", VAL_SELECT_BUTTON, 0, 0, 1, 1 path_name);
        FileSelectPopupEx("", "*.wav", "*.*", "��ѡ��Ŀ���ļ�", VAL_SELECT_BUTTON, 0, 0, path_name);
    }

    SetCtrlVal(panelHandle, control, path_name);
}

void SendStartSignal(void)
{
    char byte = 'S'; // START
    if (com_open_flag)
    {
        FlushInQ(com_port);
        ComWrt(com_port, &byte, 1);
    }
}

// ��ʾ������ ����
void Schedule(int cur_position, int total_size)
{
#define CHARACTER_SIZE 50
    int totalLines;
    char buffer[CHARACTER_SIZE + 30] = {0};
    char temp[20] = {0};
    float num = ((float)cur_position / (float)total_size) * CHARACTER_SIZE; // num�����������С�>���ĸ�����0~50��

    sprintf(buffer, "running:[");
    for (int i = 0; i < 50; i++)
    {
        if (i < num) // ���num��">"
            strcat(buffer, ">");
        else
            strcat(buffer, " "); // �����ÿո����
    }

    if (num > 49.5) // ��ֹ(cur_position / total_size)���ܱ�����
        num = 50;

    sprintf(temp, "]%% %.2f", num * (100 / CHARACTER_SIZE)); // �����ɽ��ȵİٷֱ�
    strcat(buffer, temp);
    GetNumTextBoxLines(panelHandle, PANEL_TEXTBOX, &totalLines);
    if (totalLines > 0)
        DeleteTextBoxLine(panelHandle, PANEL_TEXTBOX, totalLines - 1);
    SetCtrlVal(panelHandle, PANEL_TEXTBOX, buffer);
}
