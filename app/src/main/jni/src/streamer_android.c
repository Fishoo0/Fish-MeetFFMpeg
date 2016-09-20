/**
 * Created by Fish
 *
 * Version1
 *
 * Simple Streamer for Android
 *
 * Destination: Push bytes from Android to RTMP by FFMpeg.
 * Steps: bytes[] -> enCoded -> RTMPServer
 *
 */


#include "volador_demoffmpeg_FFMpegStreamer.h"


#define BYTETOBINARYPATTERN "%d%d%d%d%d%d%d%d"
#define BYTETOBINARY(byte)  \
  (byte & 0x80 ? 1 : 0), \
  (byte & 0x40 ? 1 : 0), \
  (byte & 0x20 ? 1 : 0), \
  (byte & 0x10 ? 1 : 0), \
  (byte & 0x08 ? 1 : 0), \
  (byte & 0x04 ? 1 : 0), \
  (byte & 0x02 ? 1 : 0), \
  (byte & 0x01 ? 1 : 0)

#include "my_log.h"

#include "streamer_pcm_to_fltp.h"

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>

#include "libavutil/opt.h"
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/frame.h"
#include "libavutil/time.h"
#include "libavutil/imgutils.h"
#include "libavutil/channel_layout.h"
#include "libavutil/common.h"
#include "libavutil/samplefmt.h"
#include "libswresample/swresample.h"



// for coding
#include "../FFMpeg/include/libavutil/opt.h"
#include "../FFMpeg/include/libavcodec/avcodec.h"
#include "../FFMpeg/include/libavformat/avformat.h"
#include "../FFMpeg/include/libavutil/frame.h"
#include "../FFMpeg/include/libavutil/time.h"
#include "../FFMpeg/include/libavutil/imgutils.h"
#include "../FFMpeg/include/libavutil/channel_layout.h"
#include "../FFMpeg/include/libavutil/common.h"
#include "../FFMpeg/include/libavutil/mathematics.h"
#include "../FFMpeg/include/libavutil/samplefmt.h"
#include "../FFMpeg/include/libswresample/swresample.h"

const enum EStreamStatus {
    IDLE, INITIALED, PREPAIRED, STREAMING, PAUSED, ERROR
};


enum EStreamStatus status = IDLE;


/**
 * Output Url
 */
unsigned char *out_url;

/**
 * Image Width
 */
int picture_width;

/**
 * Image Height
 */
int picture_height;

/**
 * FrameRate for Video
 */
int video_frame_rate;

int audio_sample_rate;

int audio_channel_layout;

char *log_path;


/**
 * Output AVFormatContext
 */
AVFormatContext *out_format_ctx = NULL;


/**
 * Output Video/Audio AVStream
 */
AVStream *out_video_avstream, *out_audio_avstream;

/**
 * Output Video/Audio AVCodecContext , for Muxer
 */
AVCodecContext *out_video_codec_ctx, *out_audio_codec_ctx;

/**
 * Output Video/Audio AVCodec
 */
AVCodec *out_video_codec, *out_audio_codec;


/**
 * Video/Audio AVFrame for decoding
 */
AVFrame *t_video_avframe, *t_audio_frame;


/**
 * The buffer for picture/audio
 */
uint8_t *picture_buf, *audio_buf;


/**
 * The size of pixel for picture
 */
int picture_pixels_length = 0;

/**
 * The UV pixel size for UV plane
 */
int picture_uv_pixels_length = 0;

/**
 * Video frame counter
 */
int video_frame_counter = 0;

/**
 * Audio frame counter
 */
int audio_frame_counter = 0;

/**
 * Start time of encoding
 */
int64_t encode_start_time = -1;


/* check that a given sample format is supported by the encoder */
static int check_sample_fmt(AVCodec *codec, enum AVSampleFormat sample_fmt) {
    LOGV("check_sample_fmt");
    const enum AVSampleFormat *p = codec->sample_fmts;

    while (*p != AV_SAMPLE_FMT_NONE) {
        LOGV("check %s", av_get_sample_fmt_name(*p));
        if (*p == sample_fmt)
            return 1;
        p++;
    }
    return 0;
}

/* just pick the highest supported samplerate */
static int select_sample_rate(AVCodec *codec) {
    const int *p;
    int best_samplerate = 0;

    if (!codec->supported_samplerates)
        return 44100;

    p = codec->supported_samplerates;
    while (*p) {
        best_samplerate = FFMAX(*p, best_samplerate);
        p++;
    }
    return best_samplerate;
}

/* select layout with the highest channel count */
static int select_channel_layout(AVCodec *codec) {
    const uint64_t *p;
    uint64_t best_ch_layout = 0;
    int best_nb_channels = 0;

    if (!codec->channel_layouts)
        return AV_CH_LAYOUT_STEREO;

    p = codec->channel_layouts;
    while (*p) {
        int nb_channels = av_get_channel_layout_nb_channels(*p);

        if (nb_channels > best_nb_channels) {
            best_ch_layout = *p;
            best_nb_channels = nb_channels;
        }
        p++;
    }
    return best_ch_layout;
}


void custom_log(void *ptr, int level, const char *fmt, va_list vl) {

    if (log_path) {
        //To TXT file
        FILE *fp = fopen(log_path, "a+");
        if (fp) {
            vfprintf(fp, fmt, vl);
            fflush(fp);
            fclose(fp);
        }
    }

    //To Logcat
//    LOGV(fmt, vl);
}


int on_error(int res) {
    LOGV("on_error -> %d", res);
    status = ERROR;
    return res;
}


/**
 *
 * NO NEED in Current Streamer !!!
 *
 *
 * Init input params
 */
int init_input() {
    LOGV("init_input");

    LOGV("do nothing ...");
    return 0;
}


/**
 * Init output params
 *
 */
int init_output() {
    LOGV("init_output");

    LOGV("allocating output AVFormatContext with RTMP url -> %s with format flv ...", out_url);
    avformat_alloc_output_context2(&out_format_ctx, NULL, "flv", out_url);
    if (!out_format_ctx) {
        LOGE("\t error ,can not alloc output AVFormatContext !!!");
        return -1;
    }

    LOGV("getting outputFormat -> %s", out_format_ctx->oformat->long_name);

    LOGV("trying open output ...");
    if (!(out_format_ctx->oformat->flags & AVFMT_NOFILE)) {
        int res = avio_open(&out_format_ctx->pb, out_url, AVIO_FLAG_WRITE);
        if (res < 0) {
            LOGE("avio_pen fro output_format_context failed ,error code -> %d", res);
            LOGE("Could not open output URL '%s'", out_url);
            return -1;
        }
        LOGV("\t URL %s is opened", out_url);
    } else {
        LOGE("\t skiped url opening !!! This should not happen !!!");
    }

    LOGV("creating video AVStream ...");
    out_video_avstream = avformat_new_stream(out_format_ctx, NULL);
    if (!out_video_avstream) {
        LOGE("failed creating video outpout AVStream !!!");
        return -1;
    }

    LOGV("creating audio AVStream ...");
    out_audio_avstream = avformat_new_stream(out_format_ctx, NULL);
    if (!out_audio_avstream) {
        LOGE("failed creating audio outpout AVStream !!!");
        return -1;
    }

    LOGV(" :) finish init_output .");

    return 0;
}


int init_video_encoder() {
    LOGV("init_video_encoder");

    LOGV("finding video encoder ...");
    out_video_codec = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (!out_video_codec) {
        LOGE("can not find video encoder !!! ");
        return -1;
    }

    LOGV("setting key params to video AVCodecContext ...");

    //Param that must set
    out_video_codec_ctx = avcodec_alloc_context3(out_video_codec);

//    out_video_codec_ctx->codec_id = out_format_ctx->oformat->video_codec;

    out_video_codec_ctx->codec_type = AVMEDIA_TYPE_VIDEO;
    out_video_codec_ctx->pix_fmt = AV_PIX_FMT_YUV420P;
    out_video_codec_ctx->width = picture_width;
    out_video_codec_ctx->height = picture_height;

    if (out_format_ctx->oformat->flags & AVFMT_GLOBALHEADER) {
        out_video_codec_ctx->flags |= CODEC_FLAG_GLOBAL_HEADER;
    }

//    setting the time_base ,ie frame/sencond
    out_video_codec_ctx->time_base.num = 1;
    out_video_codec_ctx->time_base.den = video_frame_rate;

    //H264
    //out_video_codec_ctx->me_range = 16;
    //out_video_codec_ctx->max_qdiff = 4;
    //out_video_codec_ctx->qcompress = 0.6;

    // bit_rate & gop work together to get the quality & delay
    out_video_codec_ctx->bit_rate = 1000000;

    // GOP : Group Of Picture
    // If alive , set it to 1 to min the delay
    out_video_codec_ctx->gop_size = 1;

    out_video_codec_ctx->qmin = 10;
    out_video_codec_ctx->qmax = 51;

    //Optional Param
    out_video_codec_ctx->max_b_frames = 3;

    // Set Option
    AVDictionary *param = 0;
    LOGV("setting AVDictionary");

    //H.264
    av_dict_set(&param, "preset", "slow", 0);
    av_dict_set(&param, "tune", "zerolatency", 0);
    //av_dict_set(&param, "profile", "main", 0);


    av_dump_format(out_format_ctx, 0, out_url, 1);

    out_video_avstream->codec = out_video_codec_ctx;
    out_video_avstream->time_base.den = out_video_codec_ctx->time_base.den;
    out_video_avstream->time_base.num = out_video_codec_ctx->time_base.num;

    LOGV("opening video encoder ...");
    int res = avcodec_open2(out_video_codec_ctx, out_video_codec, &param);
    if (res < 0) {
        LOGE("can not open video encoder !!! code -> %d", res);
        return res;
    }

    LOGV("video encoder has been initialized & opened successfully .");

    return 0;
}


int init_audio_encoder() {
    LOGV("init_audio_encoder");

    LOGV("finding audio encoder ...");
    out_audio_codec = avcodec_find_encoder(AV_CODEC_ID_AAC);
    if (!out_audio_codec) {
        LOGE("can not find audio encoder !!! ");
        return -1;
    }

    LOGV("setting key params to audio AVCodecContext ...");

    //Param that must set
    out_audio_codec_ctx = avcodec_alloc_context3(out_audio_codec);


    out_audio_codec_ctx->codec_id = AV_CODEC_ID_AAC;
    out_audio_codec_ctx->codec_type = AVMEDIA_TYPE_AUDIO;

    /**
     * The stupid aac encoder only surport
     */
    out_audio_codec_ctx->sample_fmt = AV_SAMPLE_FMT_FLTP;

    if (!check_sample_fmt(out_audio_codec, out_audio_codec_ctx->sample_fmt)) {
        LOGE("Encoder does not support sample format %s",
             av_get_sample_fmt_name(out_audio_codec_ctx->sample_fmt));
        return -1;
    }

    /* select other audio parameters supported by the encoder */
//    out_audio_codec_ctx->sample_rate = select_sample_rate(out_audio_codec);
//    out_audio_codec_ctx->channel_layout = select_channel_layout(out_audio_codec);

    out_audio_codec_ctx->sample_rate = audio_sample_rate;
    out_audio_codec_ctx->channel_layout = AV_CH_LAYOUT_STEREO;

    /** Allow the use of the experimental AAC encoder */
    out_audio_codec_ctx->strict_std_compliance = FF_COMPLIANCE_EXPERIMENTAL;

    out_audio_codec_ctx->channels = av_get_channel_layout_nb_channels(
            out_audio_codec_ctx->channel_layout);
    out_audio_codec_ctx->bit_rate = 32000;

    /**
     * Some container formats (like MP4) require global headers to be present
     * Mark the encoder so that it behaves accordingly.
     */
    if (out_format_ctx->oformat->flags & AVFMT_GLOBALHEADER) {
        out_audio_codec_ctx->flags |= CODEC_FLAG_GLOBAL_HEADER;
    }

    av_dump_format(out_format_ctx, 0, out_url, 1);

    out_audio_avstream->codec = out_audio_codec_ctx;
    out_audio_avstream->time_base.num = 1;
    out_audio_avstream->time_base.den = out_audio_codec_ctx->sample_rate;

    LOGV("opening audio encoder ...");
    int res = avcodec_open2(out_audio_codec_ctx, out_audio_codec, NULL);
    if (res < 0) {
        LOGE("can not open audio encoder !!! code -> %d", res);
        return res;
    }

    LOGV("audio encoder has been initialized & opened successfully .");
    return 0;

}

/**
 *
 * Init encoder
 *
 */
int init_encoder() {
    LOGV("init_encoder");

    int res;
    res = init_video_encoder();
    if (res) {
        return res;
    }

    res = init_audio_encoder();
    if (res) {
        return res;
    }

    LOGV("all encoders has been initialized & opened successfully .");

    return 0;
}


int init_video_encode_key_params() {
    LOGV("init_video_encode_key_params");

    picture_pixels_length = out_video_codec_ctx->width * out_video_codec_ctx->height;
    picture_uv_pixels_length = picture_pixels_length / 4;

    LOGV("picture_pixels_length -> %d", picture_pixels_length);
    LOGV("picture_uv_pixels_length -> %d", picture_uv_pixels_length);


    LOGV("alloc AVFrame for video raw data ...");
    t_video_avframe = av_frame_alloc();

    if (!t_video_avframe) {
        LOGE("creating video AVFrame failed !!!");
        return -1;
    }

    t_video_avframe->width = picture_width;
    t_video_avframe->height = picture_height;
    t_video_avframe->format = out_video_codec_ctx->pix_fmt;

    int res = -1;

    res = av_image_alloc(t_video_avframe->data, t_video_avframe->linesize, t_video_avframe->width,
                         t_video_avframe->height,
                         t_video_avframe->format, 32);

    if (res < 0) {
        LOGE("failed in creating buffer for video AVFrame !!! self handling");
        int picture_size = avpicture_get_size(out_video_codec_ctx->pix_fmt,
                                              out_video_codec_ctx->width,
                                              out_video_codec_ctx->height);
        picture_buf = (uint8_t *) av_malloc(picture_size);

        LOGV("creating buffer ,pictureSize -> %d", picture_size);

        avpicture_fill((AVPicture *) t_video_avframe, picture_buf, out_video_codec_ctx->pix_fmt,
                       out_video_codec_ctx->width, out_video_codec_ctx->height);
    } else {
        LOGV("creating buffer for video AVFrame successfully !");
    }

    return 0;
}


int init_audio_encode_key_params() {
    LOGV("init_audio_encode_key_param");

    t_audio_frame = av_frame_alloc();
    t_audio_frame->nb_samples = out_audio_codec_ctx->frame_size;
    t_audio_frame->format = out_audio_codec_ctx->sample_fmt;

    LOGV("creating buffer for audio encoding ...");
    int buffer_size = av_samples_get_buffer_size(NULL, out_audio_codec_ctx->channels,
                                                 out_audio_codec_ctx->frame_size,
                                                 out_audio_codec_ctx->sample_fmt, 0);
    if (buffer_size < 0) {
        LOGE("can not getting buffer size for audio !!!");
        return buffer_size;
    }

    audio_buf = av_malloc(buffer_size);
    if (!audio_buf) {
        LOGE("can not allocating memory for audio encoding ...");
        return -1;
    }

    int res = avcodec_fill_audio_frame(t_audio_frame, out_audio_codec_ctx->channels,
                                       out_audio_codec_ctx->sample_fmt, (const uint8_t *) audio_buf,
                                       buffer_size, 0);
    if (res < 0) {
        LOGE("can not set audio frame !!!");
        return res;
    }

    LOGV("creating buffer for audio successfully");
    return 0;
}


int init_encode_key_params() {
    LOGV("init_encode_key_params");

    int res = init_video_encode_key_params();
    if (res) {
        return res;
    }

    res = init_audio_encode_key_params();
    if (res) {
        return res;
    }

    return 0;
}


int pre_frame_writing() {
    LOGV("pre_frame_writing");

    LOGV("writing header ...");
    int res = avformat_write_header(out_format_ctx, NULL);
    if (res) {
        LOGE("writing header failed !!! ");
        return -1;
    }


    return 0;
}


int streamer_init(const unsigned char *outUrl, int width, int height, int frameRate,
                  int audioSampleRate, int audioChannelLayout) {
    LOGV("streamer_init");


    out_url = outUrl;
    picture_width = width;
    picture_height = height;
    video_frame_rate = frameRate;

    audio_sample_rate = audioSampleRate;

    audio_channel_layout = audioChannelLayout;

    LOGV("\tURL:%s \n\tSize(width*height):%d*%d\n\tFrameRate:%d\n\tAudioSampleInHz:%d", outUrl,
         width, height, frameRate, audioSampleRate);

    LOGV("regisster all ...");
    av_register_all();

    LOGV("init network ...");
    avformat_network_init();

    av_log_set_callback(custom_log);


    int res = 0;

    res = init_input();
    if (res) {
        return on_error(res);
    }

    res = init_output();
    if (res) {
        return on_error(res);
    }

    res = init_encoder();
    if (res) {
        return on_error(res);
    }

    res = init_encode_key_params();
    if (res) {
        return on_error(res);
    }

    res = streamer_pcm_to_fltp_init(AV_SAMPLE_FMT_S16, AV_SAMPLE_FMT_FLTP, AV_CH_LAYOUT_MONO,
                                    AV_CH_LAYOUT_MONO ,2048);
    if (res) {
        return on_error(res);
    }

    status = INITIALED;
    return 0;
}


int streamer_prepair() {
    LOGV("streamer_prepair");
    int res;
    res = pre_frame_writing();
    if (res) {
        return on_error(res);
    }

    status = PREPAIRED;
    return 0;
}

void check_start_time() {
    if (encode_start_time < 0) {
        encode_start_time = av_gettime();
        LOGV("\t encode_start_time has not been set , set it to %ld", encode_start_time);
    }
}


int streamer_encode_video(jbyte *data) {
    LOGV("streamer_encode_video");

    LOGV("reading data into t_video_avframe from NV21 to YUV420P");

    LOGV("reading Y ...");
    memcpy(t_video_avframe->data[0], data, picture_pixels_length);                   //Y
    LOGV("reading UV ...");
    int i = 0;
    for (i = 0; i < picture_uv_pixels_length; i++) {
        *(t_video_avframe->data[1] + i) = *(data + picture_pixels_length + 2 * i + 1); //U
        *(t_video_avframe->data[2] + i) = *(data + picture_pixels_length + 2 * i);    //V
    }

    LOGV("init encoded packet ...");
    AVPacket t_encoded_packet;
    av_init_packet(&t_encoded_packet);
    t_encoded_packet.data = NULL;
    t_encoded_packet.size = 0;


//    t_video_avframe->pts = video_frame_counter;

    int res = -1, got_output = -1;

    res = avcodec_encode_video2(out_video_codec_ctx, &t_encoded_packet, t_video_avframe,
                                &got_output);
    if (res < 0) {
        LOGE("Error in encoding video !!! code is -> %d", res);
        return res;
    }

    if (got_output) {
        LOGV("successful in encoding video ,trying setting PTS/DTS and delay if neccesary ,frame -> %d ",
             video_frame_counter);


        AVRational timeBase = out_video_codec_ctx->time_base;
        int64_t frame_duration = (double) AV_TIME_BASE * av_q2d(timeBase);


        if (t_encoded_packet.pts == AV_NOPTS_VALUE) {
            LOGV("setting PTS ...");
            t_encoded_packet.pts = t_encoded_packet.dts = video_frame_counter;
            t_encoded_packet.duration = timeBase.den;
//            t_encoded_packet.pos = -1;
        } else {
            LOGV("PTS exits ...");
        }

        int64_t dts_time = av_rescale_q(t_encoded_packet.dts, out_video_codec_ctx->time_base,
                                        AV_TIME_BASE_Q);
        int64_t current_time = av_gettime() - encode_start_time;
        LOGV("dts_time %ld , current_time %ld", dts_time, current_time);

//        if (dts_time > current_time) {
//            LOGV("\t dts_time > current_time , need wait %ld", (dts_time - current_time));
//            av_usleep(dts_time - current_time);
//        } else {
//            LOGV("\t no need waiting ...");
//        }

        LOGV("start to writing video packet ...");

        av_interleaved_write_frame(out_format_ctx, &t_encoded_packet);

        free(data);
        av_free_packet(&t_encoded_packet);

        LOGV(":) successfully with video frame %d", video_frame_counter);

        video_frame_counter++;
        return 0;
    } else {
        LOGE(" failed in get video encoded frame ??? ");
        return -1;
    }

}


int streamer_convert(enum AVSampleFormat srcSampleFormat,
                     enum AVSampleFormat dstSampleFormat,
                     int64_t srcChLayout, int64_t dstChLayout,
                     int srcSampeRate, int dstSampleRate, uint8_t *srcData, int srcNmSamples,
                     uint8_t *dstData, int dstNmSamples) {
    LOGV("streamer_convert");

    struct SwrContext *swrContext;


//    swrContext = swr_alloc_set_opts(NULL,
//                                    dstChLayout, dstSampleFormat, dstSampleRate,
//                                    srcChLayout, srcSampleFormat, srcSampeRate,
//                                    1, NULL);


    swrContext = swr_alloc();
    if (!swrContext) {
        LOGE("Can not creating SwrContext for AudioFormat converting !!!");
        return -1;
    }

    LOGV("setting params ...");

    av_opt_set_int(swrContext, "in_channel_layout", srcChLayout, 0);
    av_opt_set_int(swrContext, "in_sample_rate", srcSampeRate, 0);
    av_opt_set_sample_fmt(swrContext, "in_sample_fmt", srcSampleFormat, 0);

    av_opt_set_int(swrContext, "out_channel_layout", dstChLayout, 0);
    av_opt_set_int(swrContext, "out_sample_rate", dstSampleRate, 0);
    av_opt_set_sample_fmt(swrContext, "out_sample_fmt", dstSampleFormat, 0);

//    if (!swrContext) {
//        LOGE("Creating SwrContext failed !!!");
//        return -1;
//    }


    LOGV("creating SwrContext successfully , try to init it ...");
    int ret = swr_init(swrContext);


    if (ret < 0) {
        LOGE("Init SwrContext failed !!!");
        return -1;
    }

    ret = swr_convert(swrContext, &dstData, dstNmSamples, &srcData, srcNmSamples);

    if (ret < 0) {
        LOGE("Error when converting !!!");
        return -1;
    }

    return 0;
}

int streamer_encode_audio(uint8_t *dataPCM16, int sampleCount) {
    LOGV("streamer_encode_audio");


    uint8_t *data;

    LOGV("dataPCM16 addr -> %d", dataPCM16);

    int m = 0;
    for (m = 0; m < 16; m++) {
        LOGV("value %dst is %x , "
                     BYTETOBINARYPATTERN, m, dataPCM16[m], BYTETOBINARY(dataPCM16[m]));
    }


    int res = -1, got_output = 1;

    LOGV("converting pcm to fltp");

    if (dataPCM16) {
        data = streamer_pcm_to_fltp_convert(dataPCM16, sampleCount, data, sampleCount);
    }

//    res = streamer_convert(AV_SAMPLE_FMT_FLTP, AV_SAMPLE_FMT_S16, AV_CH_LAYOUT_MONO,
//                           AV_CH_LAYOUT_MONO, audio_sample_rate, audio_sample_rate, dataPCM16,
//                           sampleCount, data, sampleCount);

    if (data == NULL) {
        LOGE("\t can not convert PCM to FLTP!!!");
        return res;
    }

    LOGV("printing converted audio data ... ");
    for (m = 0; m < 16; m++) {
        LOGV("res value %dst is %x", m, data[m]);
    }



    LOGV("init encoded packet ...");
    AVPacket t_encoded_packet;

    av_init_packet(&t_encoded_packet);
    t_encoded_packet.data = NULL;
    t_encoded_packet.size = 0;

//    t_encoded_packet.data = data;
//    t_encoded_packet.size = sizeof(t_encoded_packet);

    t_audio_frame->data[0] = data;
//    t_audio_frame->pts = audio_frame_counter * 100;
    t_audio_frame->pts = av_frame_get_best_effort_timestamp(t_audio_frame);

    LOGV("start encoing audio ...");
    res = avcodec_encode_audio2(out_audio_codec_ctx, &t_encoded_packet, t_audio_frame, &got_output);
    if (res < 0) {
        LOGE("failed to encode audio !!! res -> %d",res);
        return res;
    }

//    LOGV("t_encoded_packet.data -> %s\n",t_encoded_packet.data);
    LOGV("t_encoded_packet.size -> %d\n",t_encoded_packet.size);

    if (got_output == 1) {
        LOGV("encode audio packet successfully ,try setting extra info\t [size:%5d] ...",
             t_encoded_packet.size);

//        t_encoded_packet.data
        t_encoded_packet.stream_index = out_audio_avstream->index;


        LOGV("start to write audio packet ...");
        av_interleaved_write_frame(out_format_ctx, &t_encoded_packet);

        free(data);
        av_free_packet(&t_encoded_packet);

        LOGV(":) successfully with audio frame %d", audio_frame_counter);
        audio_frame_counter++;
        return 0;
    } else {
        LOGV("failed in encode audio frame ???");
        return -1;
    }

}


int streamer_flush() {
    LOGV("streamer_flush");

    LOGV("do nothing ...");

}

int streamer_pause() {
    LOGV("streamer_pause");

    LOGV("pausing ,actually do nothing ...");
    status = PAUSED;
}


int streamer_stop() {
    LOGV("streamer_stop");
    av_write_trailer(out_format_ctx);

    encode_start_time = -1;
}


int streamer_release() {
    LOGV("streamer_release");

    streamer_flush();

    streamer_stop();

    if (out_video_avstream) {
        avio_close(out_format_ctx->pb);
    }
    avformat_free_context(out_format_ctx);

    status = IDLE;
    return 0;
}

JNIEXPORT jint JNICALL Java_volador_demoffmpeg_FFMpegStreamer_init
        (JNIEnv *env, jobject jo, jstring url, jint width, jint height, jint frameRate,
         jint audioSample, jint audioChannelLayout) {
    const char *output = (*env)->GetStringUTFChars(env, url, 0);
    streamer_init(output, width, height, frameRate, audioSample, audioChannelLayout);
}


JNIEXPORT jint JNICALL Java_volador_demoffmpeg_FFMpegStreamer_prepair
        (JNIEnv *env, jobject jo) {
    streamer_prepair();
}

JNIEXPORT jint JNICALL Java_volador_demoffmpeg_FFMpegStreamer_encodeVideo
        (JNIEnv *env, jobject jo, jbyteArray srcData) {
    uint8_t *data = (uint8_t *) (*env)->GetByteArrayElements(env, srcData, 0);
    return streamer_encode_video(data);
}

JNIEXPORT jint JNICALL Java_volador_demoffmpeg_FFMpegStreamer_encodeAudio
        (JNIEnv *env, jobject jo, jshortArray srcData, jint sampleCount) {
    uint8_t *data = (uint8_t *) (*env)->GetShortArrayElements(env, srcData, 0);
    return streamer_encode_audio(data, sampleCount);
}

JNIEXPORT jint JNICALL Java_volador_demoffmpeg_FFMpegStreamer_pause
        (JNIEnv *env, jobject jo) {
    streamer_pause();
}

JNIEXPORT jint JNICALL Java_volador_demoffmpeg_FFMpegStreamer_release
        (JNIEnv *env, jobject jo) {
    streamer_release();
}

JNIEXPORT jint JNICALL Java_volador_demoffmpeg_FFMpegStreamer_setLogPath
        (JNIEnv *env, jobject jo, jstring logPath) {
    log_path = (*env)->GetStringUTFChars(env, logPath, 0);
}