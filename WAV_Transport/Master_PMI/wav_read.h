#ifndef WAV_READ_H
#define WAV_READ_H

#include <stdio.h>

#define PRINT_WAV_HEAD 0
#define WAVE_HEAD_SIZE 44

/*litter endian*/
typedef struct
{
    /*file_size=chunk_size+8=sub_chunk_size+44*/
    /*fmt_chunk_size=0x10,defines format information
    for audio data,0x14~0x23*/
    char file_format[4];   // "RIFF"
    int chunk_size;        // =sub_chunk_size+36
    char form_type[4];     // "WAVE"
    char fmt[4];           // "fmt "
    int fmt_size;          // fmt chunk size=0x10
    short audio_format;    // 01=PCM
    short channels;        // channel number,1/2
    int sample_rate;       // sample rate,8000/16000/44100
    int byte_rate;         // SampleRate * Channels * BitsPerSample / 8
    short byte_per_sample; // BitsPerSample*Channels/8
    short bit_per_sample;  // 8/16/32
    char data[4];          // "data"
    int sub_chunk_size;    // =file_size-44
} WAV_HEAD;

int WAV_AnalyzeHead(FILE *fp_in, WAV_HEAD *h);
int WAV_WriteHead(FILE *fp_in, WAV_HEAD *h);
int WAV_ReadBuffer(FILE *fp_in, short *buf, int n);
short *WAV_ReadAll(FILE *fp_in, int *n);
int WAV_WriteBuffer(FILE *fp_in, const short *buf, int n);
#endif // !WAV_READ_H
