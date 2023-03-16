#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

#define FRAME_SIZE 512
#define DATA_BLOCK_SIZE (FRAME_SIZE * 2)

#define FRAME_START 1
#define FRAME_DATA 2
#define FRAME_END 3

#define FORMAT_CHAR 1
#define FORMAT_SHORT 2
#define FORMAT_INT 3
#define FORMAT_FLOAT 4

typedef enum
{
    SerialOK,
    SerialError
} UserStatus;

typedef struct
{
    uint8_t content_type;
    uint8_t frame_index;
    uint8_t inv_index;
    uint8_t data_format;
    uint8_t data_size[2];
    uint8_t crc[2];
} FormatTransmit;

typedef struct
{
    uint8_t content_type;
    uint8_t frame_index;
    uint8_t inv_index;
    uint8_t data[DATA_BLOCK_SIZE];
    uint8_t data_format;
    uint8_t data_size[2];
    uint8_t crc[2];
} DataTransmit;

UserStatus RecvStartFrame(void);
UserStatus RecvDataFrame(void);
UserStatus RecvEndFrame(void);
#endif
