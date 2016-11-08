//
// Created by Volador on 16/5/13.
// This copy is Learned form resampling_audio.c
//

#include "streamer_pcm_to_fltp.h"

#include "my_log.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "libavutil/opt.h"
#include "libavutil/channel_layout.h"
#include "libavutil/samplefmt.h"
#include "libswresample/swresample.h"


#ifdef __cplusplus
}
#endif

#include "../FFMpeg/include/libavutil/opt.h"
#include "../FFMpeg/include/libavutil/channel_layout.h"
#include "../FFMpeg/include/libavutil/samplefmt.h"
#include "../FFMpeg/include/libswresample/swresample.h"


uint8_t **src_data = NULL;
uint8_t **dst_data = NULL;

int64_t src_ch_layout, dst_ch_layout;
int src_rate, dst_rate;
int src_nb_channels = 0, dst_nb_channels = 0;
enum AVSampleFormat src_sample_fmt, dst_sample_fmt;


int src_linesize, dst_linesize;

int max_dst_nb_samples;


SwrContext *swr_ctx;


/**
 * Initial params
 */
int streamer_pcm_to_fltp_init(enum AVSampleFormat srcSampleFormat,
                              enum AVSampleFormat dstSampleFormat,
                              int64_t srcChLayout, int srcSampeRate, int srcNbSamples) {

    LOGV("init");

    src_ch_layout = dst_ch_layout = srcChLayout;
    src_rate = dst_rate = srcSampeRate;
    src_sample_fmt = srcSampleFormat;
    dst_sample_fmt = dstSampleFormat;

    swr_ctx = swr_alloc();
    if (!swr_ctx) {
        LOGE("Can not creating SwrContext for AudioFormat converting !!!");
        return -1;
    }

    LOGV("setting params ...");

    av_opt_set_int(swr_ctx, "in_channel_layout", src_ch_layout, 0);
    av_opt_set_int(swr_ctx, "in_sample_rate", src_rate, 0);
    av_opt_set_sample_fmt(swr_ctx, "in_sample_fmt", src_sample_fmt, 0);

    av_opt_set_int(swr_ctx, "out_channel_layout", dst_ch_layout, 0);
    av_opt_set_int(swr_ctx, "out_sample_rate", dst_rate, 0);
    av_opt_set_sample_fmt(swr_ctx, "out_sample_fmt", dst_sample_fmt, 0);

    int ret = swr_init(swr_ctx);
    if (ret < 0) {
        LOGE("Can not init SwrContext !!! Invalid params ?!!!");
        return -1;
    }


    LOGV("init src params ...");
    LOGV("caculating nb_channels ...");
    src_nb_channels = av_get_channel_layout_nb_channels(src_ch_layout);


    LOGV("allocating src memory ...");  // ??? how ?
    ret = av_samples_alloc_array_and_samples(&src_data, &src_linesize, src_nb_channels,
                                             srcNbSamples, src_sample_fmt, 0);

    if (ret < 0) {
        LOGE("Failed creating allocating src memory ...");
        return -1;
    }

    LOGV("init dst params ...");
    LOGV("caculating nb_channels ...");
    dst_nb_channels = av_get_channel_layout_nb_channels(dst_ch_layout);

//    LOGV("allocating dst memory ...");
//    /* compute the number of converted samples: buffering is avoided
//     * ensuring that the output buffer will contain at least all the
//     * converted input samples */
//    max_dst_nb_samples = av_rescale_rnd(srcNbSamples, dst_rate, src_rate,
//                                        AV_ROUND_UP);
//
//    ret = av_samples_alloc_array_and_samples(&dst_data, &dst_linesize, dst_nb_channels,
//                                             max_dst_nb_samples, dst_sample_fmt, 0);

//    if (ret < 0) {
//        LOGE("Failed allocating dst memory ...");
//        return -1;
//    }

    LOGV("init converter sucess. ");
    return 0;
}

/**
 * Converting data
 */
uint8_t *streamer_pcm_to_fltp_convert(uint8_t *srcData, int srcNbSample, uint8_t *dst,
                                      int dstNbSample) {
    LOGV("streamer_pcm_to_fltp_convert");

    int ret;

    LOGV("recaculating dst_nb_samples ...");
    int dstNbSamples = av_rescale_rnd(swr_get_delay(swr_ctx, src_rate) + srcNbSample, dst_rate,
                                      src_rate, AV_ROUND_UP);
//
//    if (dstNbSamples > max_dst_nb_samples) {
//        LOGV("dst_nb_samples is bigger than max_dst_nb_samples, reallocating dst memory ...");
//        av_freep(&dst_data[0]);
//        ret = av_samples_alloc(dst_data, &dst_linesize, dst_nb_channels, dstNbSamples,
//                               dst_sample_fmt, 1);
//        if (ret < 0) {
//            LOGE("reallocating failed !!!");
//            return -1;
//        }
//        max_dst_nb_samples = dstNbSamples;
//        LOGV("reallocating success .");
//    }

//    if (dst_data != NULL && dst_data[0] != NULL) {
//        av_freep(&dst_data[0]);
//    }


    ret = av_samples_alloc_array_and_samples(&dst_data, &dst_linesize, dst_nb_channels,
                                             dstNbSamples, dst_sample_fmt, 0);
    if (ret < 0) {
        LOGE("Allocating dst memory failed !!!");
        return -1;
    }

    src_data[0] = srcData;

    LOGV("try converting format ...");
    ret = swr_convert(swr_ctx, dst_data, dstNbSample, src_data,srcNbSample);
    if (ret < 0) {
        LOGE("Error when converting audio !!!");
        return ret;
    } else {
        LOGV("result -> %d", ret);
    }

    int resampled_data_size = av_samples_get_buffer_size(&dst_linesize, dst_nb_channels,
                                                         ret, dst_sample_fmt, 1);
    if (resampled_data_size <= 0) {
        LOGV("av_samples_get_buffer_size error \n");
        return -1;
    } else {
        LOGV("resampled data size -> %d", resampled_data_size);
    }

    if (dst_data == NULL || dst_data[0] == NULL) {
        LOGE("Unknown error !!!");
        return -1;
    }

    //将值返回去
//    memcpy(dst, dst_data[0], resampled_data_size);

    LOGV("converting success .");
    return dst_data[0];
}


/*
 * Releasing data
 */
int streamer_pcm_to_fltp_release() {
    swr_free(&swr_ctx);
}