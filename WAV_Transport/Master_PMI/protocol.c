#include "protocol.h"
#include "crcLib.h"
#include "main.h"
#include "ui_setting.h"
#include "wav_read.h"
#include <ansi_c.h>
#include <cvirte.h>
#include <rs232.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <userint.h>
#include <windows.h>

extern int panelHandle;

/**
 * @brief �����ļ���С
 *
 * @param fp �ļ�ָ��
 * @return long �ļ���С
 */
long FileSize(FILE *fp)
{
    long size = 0;
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    return size;
}

/**
 * @brief �ж�����֡��CRC�Ƿ���ȷ
 *
 * @param data ����֡
 * @return int 1-��ȷ��0-����
 */
int JudgeCRC(DataTransmit *data)
{
    uint8_t *in = NULL;
    uint16_t crc = 0;
    uint8_t crc_h, crc_l;
    in = (uint8_t *)data;
    crc = crc16_maxim(in, sizeof(DataTransmit) - 2);
    crc_h = (uint8_t)(crc >> 8);
    crc_l = (uint8_t)(crc & 0xff);
    if ((crc_h == data->crc[0]) && (crc_l == data->crc[1]))
    {
        return 1;
    }
    return 0;
}

FormatTransmit format = {0}; // ��ʽ֡����
DataTransmit data = {0};     // ����֡����

/**
 * @brief �û�ͨ��Э��
 *
 * @param in_file �����ļ�
 * @param out_file ����ļ�
 * @return int
 */
int UserProtocol(char *in_file, char *out_file)
{
    FILE *fp_in = NULL;       // �ļ�����ָ��
    FILE *fp_out = NULL;      // �ļ����ָ��
    char msg[64] = {0};       // ��Ϣ
    long file_size = 0;       // �ļ���С
    int send_step = 0;        // ���Ͳ���
    int send_new = 0;         // ������һ֡
    char byte_value = 0;      // �ֽ�����
    int file_read_status = 0; // �ļ���ȡ״̬
    // ��������ݸ���ʵ���޸�����
    short *pdata = (short *)data.data;
    uint16_t crc = 0;       // crcУ��ֵ
    int data_cnt = 1;       // �������ݼ���
    int wait_cnt = 0;       // �ȴ�����
    WAV_HEAD header = {0};  // wav�ļ�ͷ
    int frame_cnt = 0;      // �ļ�֡��
    clock_t start_time = 0; // ��ʼʱ��
    clock_t end_time = 0;   // ����ʱ��

    fp_in = fopen(in_file, "rb");
    fp_out = fopen(out_file, "wb");
    if (!fp_in || !fp_out)
    {
        sprintf(msg, "Error opening %s or %s\n", fp_in, fp_out);
        SetCtrlVal(panelHandle, PANEL_TEXTBOX, msg);
        return 1;
    }
    memset(msg, 0, sizeof(msg));
    file_size = FileSize(fp_in);
    sprintf(msg, "File Size is %d\n", file_size);
    SetCtrlVal(panelHandle, PANEL_TEXTBOX, msg);

    WAV_AnalyzeHead(fp_in, &header);
    /*skip the 44 head*/
    fseek(fp_in, WAVE_HEAD_SIZE, SEEK_SET);
    fseek(fp_out, WAVE_HEAD_SIZE, SEEK_SET);

    while (1)
    {
        switch (send_step)
        {
        case 0: // �ȴ����ն˿�ʼ��������
        {
            wait_cnt++;
            SetCtrlVal(panelHandle, PANEL_TEXTBOX, "�ȴ����ն˿�ʼ����\n");
            // FlushInQ(com_port);
            ComRd(com_port, &byte_value, 1);
            if ((byte_value == 'c') || (byte_value == 'C'))
            {
                wait_cnt = 0;
                send_step = 1;
            }
            if (wait_cnt > 10)
            {
                SetCtrlVal(panelHandle, PANEL_TEXTBOX, "��ʱδ��Ӧ\n");
                return (-2);
            }
            Sleep(50);
            break;
        }
        case 1: // ������ʼ֡
        {
            SetCtrlVal(panelHandle, PANEL_TEXTBOX, "������ʼ֡\n");
            format.content_type = SIGNAL_START;
            format.frame_index = 0x00;
            format.inv_index = 0xff;
            format.data_format = FORMAT_FLOAT;
            format.data_size[0] == 0x00;
            format.data_size[1] == 0x00;
            crc = crc16_maxim((uint8_t *)&format, sizeof(format) - 2);
            format.crc[0] = (uint8_t)(crc >> 8);
            format.crc[1] = (uint8_t)(crc & 0xff);
            while (GetOutQLen(com_port) > 10)
            {
                Sleep(50);
            }
            FlushInQ(com_port);
            ComWrt(com_port, (char *)&format, sizeof(FormatTransmit));
            SetCtrlVal(panelHandle, PANEL_TEXTBOX, "�ȴ�Ӧ����ʼ֡\n");
            while (1)
            {
                wait_cnt++;
                ComRd(com_port, &byte_value, 1);
                if ((byte_value == 'a') || (byte_value == 'A')) // ACK
                {
                    send_step = 255;
                    break;
                }
                else if ((byte_value == 'r') || (byte_value == 'R')) // RESEND
                {
                    break;
                }
                else
                {
                    if (wait_cnt > 10)
                    {
                        return (-3);
                    }
                }
            }
            if (send_step == 255)
            {
                SetCtrlVal(panelHandle, PANEL_TEXTBOX, "��ʼ���ݷ���\n");
                send_new = 1;
                send_step = 2;
                start_time = clock();
            }

            break;
        }
        case 2: // ��������֡
        {

            if (send_new) // ��ȡ�ļ�
            {
                file_read_status = WAV_ReadBuffer(fp_in, pdata, FRAME_SIZE);

                long current_position = ftell(fp_in);
                Schedule(current_position, file_size);
            }

            if (file_read_status)
            {
                frame_cnt++;
                data.content_type = SIGNAL_DATA;
                data.data_size[0] = FRAME_SIZE / 256;
                data.data_size[1] = FRAME_SIZE % 256;
            }
            else
            {
                data.content_type = SIGNAL_END;
                data.data_size[0] = 0x00;
                data.data_size[1] = 0x01;
            }
            data.frame_index = data_cnt & 0xff;
            data.inv_index = (data_cnt ^ 0xff) & 0xff;
            // memcpy(data.data, pdata, 1024);
            data.data_format = FORMAT_FLOAT;

            crc = crc16_maxim((uint8_t *)&data, sizeof(data) - 2);
            data.crc[0] = (uint8_t)(crc >> 8);
            data.crc[1] = (uint8_t)(crc & 0xff);

            while (GetOutQLen(com_port) > 10)
            {
                Sleep(10);
            }
            ComWrt(com_port, (char *)&data, sizeof(data));
            ComRd(com_port, &byte_value, 1);
            if ((byte_value == 'a') || (byte_value == 'A'))
            {
                if (file_read_status)
                {
                    if (sizeof(data) == ComRd(com_port, (char *)&data, sizeof(data)))
                    {
                        if (1 == JudgeCRC(&data))
                        {
                            FlushInQ(com_port);
                            byte_value = 'a';
                            ComWrt(com_port, &byte_value, 1);
                            WAV_WriteBuffer(fp_out, pdata, FRAME_SIZE);
                            send_new = 1;
                            data_cnt++;
                        }
                        else
                        {
                            FlushInQ(com_port);
                            byte_value = 'r';
                            ComWrt(com_port, &byte_value, 1);
                        }
                    }
                }
                else
                {
                    send_step = 3;
                }
            }
            else if ((byte_value == 'r') || (byte_value == 'R'))
            {
                send_new = 0;
            }

            break;
        }
        case 3: /*��3�������ͽ���֡*/
        {
            memset(&format, 0, sizeof(format));
            format.content_type = SIGNAL_END;
            format.frame_index = 0xff;
            format.inv_index = 0x00;
            format.data_format = FORMAT_FLOAT;
            crc = crc16_maxim((uint8_t *)&format, sizeof(format) - 2);
            format.crc[0] = (uint8_t)(crc >> 8);
            format.crc[1] = (uint8_t)(crc & 0xff);
            while (GetOutQLen(com_port) > 10)
            {
                Sleep(50);
            }
            FlushInQ(com_port);
            ComWrt(com_port, (char *)&format, sizeof(FormatTransmit));
            header.sub_chunk_size = FRAME_SIZE * 2 * frame_cnt;
            header.chunk_size = header.sub_chunk_size + 36;
            WAV_WriteHead(fp_out, &header);
            SetCtrlVal(panelHandle, PANEL_TEXTBOX, "\nȫ���������\n");
            end_time = clock();
            memset(msg, 0, sizeof(msg));
            sprintf(msg, "����ʱ��Ϊ :%.2f��\n", (float)(end_time - start_time) / CLOCKS_PER_SEC);
            SetCtrlVal(panelHandle, PANEL_TEXTBOX, msg);
            send_step = 100;
            break;
        }

        default:
            break;
        }

        if (send_step > 20) /*ȫ��������ɣ�����*/
        {
            break;
        }
    }
    return 0;
}

#define MAX_NUM 100  /*dir file number*/
#define MAX_CHAR 100 /*max file name length */

char file_in[MAX_NUM][MAX_CHAR] = {0};  /*The specified file within the folder*/
char file_out[MAX_NUM][MAX_CHAR] = {0}; /*The write file within the folder*/
int file_cnt = 0;                       /*quantity of file*/

/**
 * @brief ��ȡ�ļ���������ָ���ļ�
 *
 * @param folder_in �����ļ���
 * @param folder_out ����ļ���
 * @return int
 */
int CVI_TraverseFolder(char *folder_in, char *folder_out)
{
    WIN32_FIND_DATA fileData;
    HANDLE hFind;
    char path[MAX_PATH] = {0};
    strcpy(path, folder_in);
    strcat(path, "\\*.wav");

    hFind = FindFirstFile(path, &fileData);
    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (!(fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                sprintf(file_in[file_cnt], "%s\\%s", folder_in, fileData.cFileName);
                sprintf(file_out[file_cnt], "%s\\%s", folder_out, fileData.cFileName);
                file_cnt++;
            }
        } while (FindNextFile(hFind, &fileData) != 0);
        FindClose(hFind);
    }
    return 0;
}

/**
 * @brief ���ļ�����
 *
 * @param folder_in �����ļ���
 * @param folder_out ����ļ���
 */
void FolderProcess(char *folder_in, char *folder_out)
{
    char msg[100] = {0};

    sprintf(msg, "�����ļ���Ϊ: %s\n", folder_in);
    SetCtrlVal(panelHandle, PANEL_TEXTBOX, msg);
    memset(msg, 0, sizeof(msg));
    sprintf(msg, "����ļ���Ϊ: %s\n", folder_out);
    SetCtrlVal(panelHandle, PANEL_TEXTBOX, msg);
    CVI_TraverseFolder(folder_in, folder_out);
    for (int i = 0; i < file_cnt; i++)
    {
        memset(msg, 0, sizeof(msg));
        sprintf(msg, "%s\n%s\n", file_in[i], file_out[i]);
        SetCtrlVal(panelHandle, PANEL_TEXTBOX, msg);
        SendStartSignal();
        UserProtocol(file_in[i], file_out[i]);
        Sleep(100);
    }
}
