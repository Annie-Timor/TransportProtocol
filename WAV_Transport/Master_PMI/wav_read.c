#include "wav_read.h"
#include "main.h"
#include <ansi_c.h>
#include <cvirte.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <userint.h>

extern int panelHandle;

/**
 * @brief Analyze header file contents
 *
 * @param fp_in :file point
 * @param w : wav head struct point or NULL
 * @return int -1:fp_in error,1:correct
 */
int WAV_AnalyzeHead(FILE *fp_in, WAV_HEAD *h)
{
    char str[100] = {0};
    char *s = NULL;
    /*the 2 struct point is:
    Distinguish function input from self allocate*/
    WAV_HEAD *head = NULL; /*print wav head information*/
    WAV_HEAD *wav = NULL;  /**/

    if (fp_in == NULL)
    {
        return -1;
    }

    if (h != NULL)
    {
        /*the struct point to char * */
        s = (char *)h;
        wav = h;
    }
    else
    {
        head = (WAV_HEAD *)malloc(sizeof(WAV_HEAD));
        s = (char *)head;
        wav = head;
    }

    fseek(fp_in, 0, 0);
    /*both the little endian~*/
    fread(s, sizeof(char), WAVE_HEAD_SIZE, fp_in);

#if PRINT_WAV_HEAD
    SetCtrlVal(panelHandle, PANEL_TEXTBOX, "---------------------------------------------\n");
    SetCtrlVal(panelHandle, PANEL_TEXTBOX, "----------------Analyze start----------------\n");

    sprintf(str, "1-File fomat:%c%c%c%c\n", wav->file_format[0], wav->file_format[1], wav->file_format[2], wav->file_format[3]);
    SetCtrlVal(panelHandle, PANEL_TEXTBOX, str);
    memset(str, 0, sizeof(str));
    sprintf(str, "2-chunk size:%d bytes\n", wav->chunk_size);
    SetCtrlVal(panelHandle, PANEL_TEXTBOX, str);
    memset(str, 0, sizeof(str));
    sprintf(str, "3-Form Type:%c%c%c%c\n", wav->form_type[0], wav->form_type[1], wav->form_type[2], wav->form_type[3]);
    SetCtrlVal(panelHandle, PANEL_TEXTBOX, str);
    memset(str, 0, sizeof(str));
    sprintf(str, "4-%c%c%c%c size:", wav->fmt[0], wav->fmt[1], wav->fmt[2], wav->fmt[3]);
    SetCtrlVal(panelHandle, PANEL_TEXTBOX, str);
    memset(str, 0, sizeof(str));
    sprintf(str, "%d bytes\n", wav->fmt_size);
    SetCtrlVal(panelHandle, PANEL_TEXTBOX, str);
    memset(str, 0, sizeof(str));
    sprintf(str, "5-Audio Format:%s\n", wav->audio_format == 1 ? "PCM" : "other");
    SetCtrlVal(panelHandle, PANEL_TEXTBOX, str);
    memset(str, 0, sizeof(str));
    sprintf(str, "6-Channels:%d\n", wav->channels);
    SetCtrlVal(panelHandle, PANEL_TEXTBOX, str);
    memset(str, 0, sizeof(str));
    sprintf(str, "7-Sample Rate:%d\n", wav->sample_rate);
    SetCtrlVal(panelHandle, PANEL_TEXTBOX, str);
    memset(str, 0, sizeof(str));
    sprintf(str, "8-Byte Rate(SampleRate * Channels * BitsPerSample / 8):%d\n", wav->byte_rate);
    SetCtrlVal(panelHandle, PANEL_TEXTBOX, str);
    memset(str, 0, sizeof(str));
    sprintf(str, "9-Bytes Per Sample:%d\n", wav->byte_per_sample);
    SetCtrlVal(panelHandle, PANEL_TEXTBOX, str);
    memset(str, 0, sizeof(str));
    sprintf(str, "10-Bits Per Sample:%d\n", wav->bit_per_sample);
    SetCtrlVal(panelHandle, PANEL_TEXTBOX, str);
    memset(str, 0, sizeof(str));
    sprintf(str, "11-%c%c%c%c:", wav->data[0], wav->data[1], wav->data[2], wav->data[3]);
    SetCtrlVal(panelHandle, PANEL_TEXTBOX, str);
    memset(str, 0, sizeof(str));
    sprintf(str, "%d bytes\n", wav->sub_chunk_size);
    SetCtrlVal(panelHandle, PANEL_TEXTBOX, str);
    memset(str, 0, sizeof(str));
    SetCtrlVal(panelHandle, PANEL_TEXTBOX, "-----------------Analyze end-----------------\n");
    SetCtrlVal(panelHandle, PANEL_TEXTBOX, "---------------------------------------------\n");
#endif

    /*free user allocate momery*/
    if (head)
    {
        free(head);
    }

    return 1;
}

/**
 * @brief write the header to the input WAV file
 *
 * @param fp_in :the input WAV file ponit
 * @param h :the input WAV head
 * @return int -1:fp_in or head is NULL,0:write error,1:correct
 */
int WAV_WriteHead(FILE *fp_in, WAV_HEAD *h)
{
    char *s = NULL;
    int ret = 0;
    if ((fp_in == NULL) || (h == NULL))
    {
        return -1;
    }
    s = (char *)h;
    fseek(fp_in, 0, 0);
    if (WAVE_HEAD_SIZE == fwrite(s, sizeof(char), WAVE_HEAD_SIZE, fp_in))
    {
        ret = 1;
    }

    return ret;
}

/**
 * @brief read N buffers from the input WAV file
 *
 * @param fp_in :the input WAV file ponit
 * @param buf :the buffer to read
 * @param n :N
 * @return int 0:read error,1:correct
 */
int WAV_ReadBuffer(FILE *fp_in, short *buf, int n)
{
    short s = 0;
    int i = 0;
    for (i = 0; i < n; i++)
    {
        if (fread(&s, sizeof(short), 1, fp_in) != 1)
        {
            return 0;
        }
        buf[i] = s;
    }
    return 1;
}

/**
 * @brief read all buffers from the input WAV file
 *
 * @param fp_in :the input WAV file ponit
 * @param n :the input WAV file buffers number
 * @return short* return the data
 */
short *WAV_ReadAll(FILE *fp_in, int *n)
{
    unsigned char head[4] = {0};
    unsigned int size = 0;
    unsigned char *audio_read = NULL;
    fseek(fp_in, 40, 0);
    fread(head, sizeof(char), 4, fp_in);
    size = head[0] + head[1] * 256 + head[2] * 256 * 256 + head[3] * 256 * 256 * 256;
    audio_read = (unsigned char *)malloc(size);
    memset(audio_read, 0, size);
    fseek(fp_in, 44, 0);
    if (fread(audio_read, sizeof(char), size, fp_in) != size)
    {
        free(audio_read);
        return NULL;
    }
    *n = size / 2;
    return ((short *)audio_read);
}

/**
 * @brief write data to the input WAV file
 *
 * @param fp_in :the input WAV file ponit
 * @param buf :the wav data
 * @param n :data number
 * @return int 1
 */
int WAV_WriteBuffer(FILE *fp_in, const short *buf, int n)
{
    fwrite(buf, sizeof(short), n, fp_in);
    return 1;
}