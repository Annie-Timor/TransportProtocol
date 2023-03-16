#include "main.h"
#include "com_port.h"
#include "protocol.h"
#include "ui_setting.h"
#include "wav_read.h"
#include <ansi_c.h>
#include <cvirte.h>
#include <rs232.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <userint.h>
#include <windows.h>

int panelHandle;

int main(int argc, char *argv[])
{
    if (InitCVIRTE(0, argv, 0) == 0)
        return -1; /* out of memory */
    if ((panelHandle = LoadPanel(0, "main.uir", PANEL)) < 0)
        return -1;
    DisplayPanel(panelHandle);
    /*the boot operation*/
    UI_ScanComPort();
    /*end the boot operation*/
    RunUserInterface();
    DiscardPanel(panelHandle);
    return 0;
}

int CVICALLBACK QUIT(int panel, int event, void *callbackData,
                     int eventData1, int eventData2)
{
    switch (event)
    {
    case EVENT_GOT_FOCUS:

        break;
    case EVENT_LOST_FOCUS:

        break;
    case EVENT_CLOSE:
        /*if UART is openning,close*/
        if (com_open_flag)
        {
            CloseCom(com_port);
        }
        QuitUserInterface(0);
        break;
    }
    return 0;
}

int CVICALLBACK SCAN_PORT_CALLBACK(int panel, int control, int event,
                                   void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
    case EVENT_COMMIT:
        UI_ScanComPort();
        break;
    }
    return 0;
}

int CVICALLBACK OPEN_PORT_CALLBACK(int panel, int control, int event,
                                   void *callbackData, int eventData1, int eventData2)
{

    switch (event)
    {
    case EVENT_COMMIT:
        UI_OpenSerialPort();
        break;
    }
    return 0;
}

int CVICALLBACK START_RUN_CALLBACK(int panel, int control, int event,
                                   void *callbackData, int eventData1, int eventData2)
{
    int select_fold = 0;
    char in_file[100] = {0};
    char out_file[100] = {0};
    switch (event)
    {
    case EVENT_COMMIT:
        GetCtrlVal(panelHandle, PANEL_SELECT_FOLD, &select_fold);
        GetCtrlVal(panelHandle, PANEL_IN_FILE, in_file);
        GetCtrlVal(panelHandle, PANEL_OUT_FILE, out_file);
        if (select_fold)
        {
            FolderProcess(in_file, out_file);
        }
        else
        {
            SendStartSignal();
            UserProtocol(in_file, out_file);
        }
        break;
    }
    return 0;
}

int CVICALLBACK SELECT_IN_FILE_CALLBACK(int panel, int control, int event,
                                        void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
    case EVENT_COMMIT:
        UI_SelectFile(PANEL_IN_FILE);
        break;
    }
    return 0;
}

int CVICALLBACK SELECT_OUT_FILE_CALLBACK(int panel, int control, int event,
                                         void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
    case EVENT_COMMIT:
        UI_SelectFile(PANEL_OUT_FILE);
        break;
    }
    return 0;
}

int CVICALLBACK CLEAR_TEXT_CALLBACK(int panel, int control, int event,
                                    void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
    case EVENT_COMMIT:
        DeleteTextBoxLines(panelHandle, PANEL_TEXTBOX, 0, -1);
        break;
    }
    return 0;
}
