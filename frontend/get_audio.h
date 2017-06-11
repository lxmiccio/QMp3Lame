/*
 *    Get Audio routines include file
 *
 *    Copyright (c) 1999 Albert L Faber
 *                  2010 Robert Hegemann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */


#ifndef LAME_GET_AUDIO_H
#define LAME_GET_AUDIO_H
#include "lame.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum sound_file_format_e {
    sf_unknown,
    sf_raw,
    sf_wave,
    sf_aiff,
    sf_mp1,                  /* MPEG Layer 1, aka mpg */
    sf_mp2,                  /* MPEG Layer 2 */
    sf_mp3,                  /* MPEG Layer 3 */
    sf_mp123,                /* MPEG Layer 1,2 or 3; whatever .mp3, .mp2, .mp1 or .mpg contains */
    sf_ogg
} sound_file_format;

#if 1
/* GLOBAL VARIABLES used by parse.c and main.c.
   instantiated in parce.c.  ugly, ugly */

typedef struct ReaderConfig
{
        sound_file_format input_format;
        int   swapbytes;                /* force byte swapping   default=0 */
        int   swap_channel;             /* 0: no-op, 1: swaps input channels */
        int   input_samplerate;
} ReaderConfig;

typedef struct WriterConfig
{
        int   flush_write;
} WriterConfig;

typedef struct UiConfig
{
        int   silent;                   /* Verbosity */
        int   brhist;
        int   print_clipping_info;      /* print info whether waveform clips */
        float update_interval;          /* to use Frank's time status display */
} UiConfig;

typedef struct DecoderConfig
{
        int   mp3_delay;                /* to adjust the number of samples truncated during decode */
        int   mp3_delay_set;            /* user specified the value of the mp3 encoder delay to assume for decoding */
        int   disable_wav_header;
        mp3data_struct mp3input_data;
} DecoderConfig;

typedef enum ByteOrder { ByteOrderLittleEndian, ByteOrderBigEndian } ByteOrder;

typedef struct RawPCMConfig
{
        int     in_bitwidth;
        int     in_signed;
        ByteOrder in_endian;
} RawPCMConfig;

extern ReaderConfig global_reader;
extern WriterConfig global_writer;
extern UiConfig global_ui_config;
extern DecoderConfig global_decoder;
extern RawPCMConfig global_raw_pcm;
#endif

#if 1 /* Moved from get_audio.c */
#ifdef LIBSNDFILE
#include <sndfile.h>
#else
typedef void SNDFILE;
#endif /* ifdef LIBSNDFILE */
#endif

#if 1 /* Moved from get_audio.c */
struct PcmBuffer {
        void   *ch[2];           /* buffer for each channel */
        int     w;               /* sample width */
        int     n;               /* number samples allocated */
        int     u;               /* number samples used */
        int     skip_start;      /* number samples to ignore at the beginning */
        int     skip_end;        /* number samples to ignore at the end */
};

typedef struct PcmBuffer PcmBuffer;
#endif

#if 1 /* Moved from get_audio.c */
/* global data for get_audio.c. */
typedef struct get_audio_global_data_struct {
        int     count_samples_carefully;
        int     pcmbitwidth;
        int     pcmswapbytes;
        int     pcm_is_unsigned_8bit;
        int     pcm_is_ieee_float;
        unsigned int num_samples_read;
        FILE   *music_in;
        SNDFILE *snd_file;
        hip_t   hip;
        PcmBuffer pcm32;
        PcmBuffer pcm16;
        size_t  in_id3v2_size;
        unsigned char* in_id3v2_tag;
} get_audio_global_data;

typedef get_audio_global_data AudioData;
#endif

int     is_mpeg_file_format( int input_format );

int     init_infile(lame_t gfp, AudioData* audioData, ReaderConfig* readerConfig, DecoderConfig* decoderConfig, RawPCMConfig* rawPcmConfig, char const* inPath);
int     samples_to_skip_at_start(AudioData* audioData);
int     samples_to_skip_at_end(AudioData* audioData);
void    close_infile(AudioData* audioData);
int     get_audio(lame_t gfp, AudioData* audioData, ReaderConfig* readerConfig, DecoderConfig* decoderConfig, RawPCMConfig* rawPcmConfig, int buffer[2][1152]);
int     get_audio16(lame_t gfp, AudioData* audioData, ReaderConfig* readerConfig, DecoderConfig* decoderConfig, RawPCMConfig* rawPcmConfig, short buffer[2][1152]);
int     get_audio_float(lame_t gfp, float buffer[2][1152]);
int     get_audio_double(lame_t gfp, double buffer[2][1152]);
hip_t   get_hip(AudioData* audioData);

FILE   *init_outfile(char const *outPath, int decode);
int     WriteWaveHeader(FILE * const fp, int pcmbytes, int freq, int channels, int bits);
void    put_audio16(ReaderConfig* readerConfig, WriterConfig* writerConfig, DecoderConfig* decoderConfig, FILE* outf, short Buffer[2][1152], int iread, int nch);

/*
struct AudioReader;
typedef struct AudioReader* AudioReader;

AudioReader ar_open(lame_t gfp, char const* inPath);
int     ar_samplesToSkipAtStart(AudioReader ar);
int     ar_samplesToSkipAtEnd(AudioReader ar);
void    ar_close(AudioReader ar);
int     ar_readInt(AudioReader ar, lame_t gfp, int buffer[2][1152]);
int     ar_readShort(AudioReader ar, lame_t gfp, short buffer[2][1152]);
int     ar_readFloat(AudioReader ar, lame_t gfp, float buffer[2][1152]);

struct AudioWriter;
typedef struct AudioWriter* AudioWriter;

AudioWriter aw_open(lame_t gfp, char const* outPath, int pcmbystes, int freq, int channels, int bits);
int     aw_writeWaveHeader(AudioWriter aw);
int     aw_write(AudioWriter aw, short buffer[2][1152], int n);
int     aw_write(AudioWriter aw, float buffer[2][1152], int n);

*/

extern size_t sizeOfOldTag(lame_t gf, AudioData* audioData);
extern unsigned char* getOldTag(lame_t gf, AudioData* audioData);

#ifdef __cplusplus
}
#endif

#endif /* ifndef LAME_GET_AUDIO_H */
