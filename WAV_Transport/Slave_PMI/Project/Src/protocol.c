#include "protocol.h"
#include "crcLib.h"
#include "usart.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

FormatTransmit format = {0};
DataTransmit dSend = {0};
DataTransmit dRecv = {0};

extern int16_t receiveStep;

void RecvDataProcess(DataTransmit *recv, DataTransmit *send);

void CalculateCRC(DataTransmit *data)
{
    uint8_t *in = NULL;
    uint16_t crc = 0;
    in = (uint8_t *)data;
    crc = crc16_maxim(in, sizeof(DataTransmit) - 2);
    data->crc[0] = (uint8_t)(crc >> 8);
    data->crc[1] = (uint8_t)(crc & 0xff);
}

/**
 * @brief 判断接收的数据帧是否正确
 *
 * @param data
 * @return UserStatus
 */
UserStatus JudgeDataCRC(DataTransmit *data)
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
        return SerialOK;
    }
    return SerialError;
}

UserStatus JudgeFormatCRC(FormatTransmit *data)
{
    uint8_t *in = NULL;
    uint16_t crc = 0;
    uint8_t crc_h, crc_l;
    in = (uint8_t *)data;
    crc = crc16_maxim(in, sizeof(FormatTransmit) - 2);
    crc_h = (uint8_t)(crc >> 8);
    crc_l = (uint8_t)(crc & 0xff);
    if ((crc_h == data->crc[0]) && (crc_l == data->crc[1]))
    {
        return SerialOK;
    }
    return SerialError;
}

UserStatus RecvStartFrame(void)
{
    int recvCnt = 0;
    //__HAL_UART_FLUSH_DRREGISTER(&huart1);
    while (1)
    {
        HAL_UART_Receive(&huart1, (uint8_t *)&format, 8, 1000);
        if ((JudgeFormatCRC(&format) == SerialOK) && (format.content_type == FRAME_START))
        {
            HAL_UART_Transmit(&huart1, "a", 1, 0xff);
            return SerialOK;
        }
        else
        {
            HAL_UART_Transmit(&huart1, "r", 1, 0xff);
        }

        if (recvCnt++ == 5)
        {
            return SerialError;
        }
    }
}

UserStatus SendDataFrame(void)
{
    uint8_t byte = 0;
    HAL_UART_Transmit(&huart1, (uint8_t *)&dSend, sizeof(DataTransmit), 1000);
    /*等待ACK响应*/

    HAL_UART_Receive(&huart1, (uint8_t *)&byte, 1, 1000);
    if (byte == 'a')
    {
        return SerialOK;
    }
    else if (byte == 'r')
    {
        /*清除串口接收*/
        __HAL_UART_FLUSH_DRREGISTER(&huart1);
        return SerialError;
    }
    return SerialError;
}

UserStatus RecvDataFrame(void)
{
    uint16_t recvCnt = 0;
    uint8_t byte = 0;

    while (1)
    {
        if (recvCnt++ == 5)
        {
            return SerialError;
        }
        memset(&dRecv, 0, sizeof(DataTransmit));
        HAL_UART_Receive(&huart1, (uint8_t *)&dRecv, sizeof(DataTransmit), 1000);

        if (SerialOK == JudgeDataCRC(&dRecv))
        {
            if (dRecv.content_type == FRAME_DATA)
            {
                HAL_UART_Transmit(&huart1, "a", 1, 0xff);
                /*接收到的数据处理*/
                // memcpy(&dSend, &dRecv, sizeof(DataTransmit));
                RecvDataProcess(&dRecv, &dSend);
                CalculateCRC(&dSend);

                /*处理结束*/

                HAL_UART_Transmit(&huart1, (uint8_t *)&dSend, sizeof(DataTransmit), 1000);
                /*等待ACK响应*/
                // memset(&dRecv, 0, sizeof(DataTransmit));
                HAL_UART_Receive(&huart1, &byte, 1, 1000);

                if (byte == 'a')
                {
                    __HAL_UART_FLUSH_DRREGISTER(&huart1);
                    return SerialOK;
                }
                else if (byte == 'r')
                {
                    /*清除串口接收*/
                    __HAL_UART_FLUSH_DRREGISTER(&huart1);
                    if (SerialError == SendDataFrame())
                    {
                        return SerialError;
                    }
                }
            }
            else if (dRecv.content_type == FRAME_END)
            {
                /*数据传输结束*/
                HAL_UART_Transmit(&huart1, "a", 1, 0xff);
                receiveStep = 3;
                return SerialOK;
            }
        }
        else
        {
            HAL_UART_Transmit(&huart1, "r", 1, 0xff);
            __HAL_UART_FLUSH_DRREGISTER(&huart1);
        }
    }
}

UserStatus RecvEndFrame(void)
{
    HAL_UART_Receive(&huart1, (uint8_t *)&format, sizeof(format), 1000);
    if ((JudgeFormatCRC(&format) == SerialOK) && (format.content_type == FRAME_END))
    {
        // HAL_UART_Transmit(&huart1, "a", 1, 0xff);

        return SerialOK;
    }
    return SerialError;
}

void RecvDataProcess(DataTransmit *recv, DataTransmit *send)
{
    int data_len = 0;

    short *in_data = (short *)recv->data;
    short *out_data = (short *)send->data;

    data_len = recv->data_size[0] * 256 + recv->data_size[1];

    for (short i = 0; i < data_len; i++)
    {
        out_data[i] = in_data[i] / 2;
    }
}
