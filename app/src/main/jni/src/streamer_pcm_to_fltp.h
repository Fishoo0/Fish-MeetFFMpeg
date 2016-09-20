//
// Created by Volador on 16/5/13.
//

#include <stdio.h>

#ifndef DEMOFFMPEG_STREAMER_PCM_TO_FLTP_H
#define DEMOFFMPEG_STREAMER_PCM_TO_FLTP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "libavutil/opt.h"
#include "libavutil/channel_layout.h"
#include "libavutil/samplefmt.h"
#include "libswresample/swresample.h"

//#include "../FFMpeg/include/libavutil/opt.h"
//#include "../FFMpeg/include/libavutil/channel_layout.h"
//#include "../FFMpeg/include/libavutil/samplefmt.h"
//#include "../FFMpeg/include/libswresample/swresample.h"


/**
 * Initial params
 */
int streamer_pcm_to_fltp_init(enum AVSampleFormat srcSampleFormat,
                              enum AVSampleFormat dstSampleFormat,
                              int64_t srcChLayout, int srcSampeRate ,int srcNbSamples);

/**
 * Converting data
 */
uint8_t *streamer_pcm_to_fltp_convert(uint8_t *data, int srcNbSample, uint8_t *dst, int dstNbSample);


/*
 * Releasing data
 */
int streamer_pcm_to_fltp_release();

#ifdef __cplusplus
}
#endif
#endif //DEMOFFMPEG_STREAMER_PCM_TO_FLTP_H