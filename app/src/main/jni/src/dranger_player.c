/** This player is a copy from dranger.com , and modified for Android. This program is a great tutorial of [ffmpeg-decoing] **/

#include <math.h>
#include <stdio.h>
#include <assert.h>

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h> // software scaling lib

#include <SDL.h>
#include <SDL_thread.h>

#include "my_log.h"

//#include "../FFMpeg/include/libavcodec/avcodec.h"
//#include "../FFMpeg/include/libavformat/avformat.h"
//#include "../FFMpeg/include/libswscale/swscale.h"
//#include "../FFMpeg/include/libavutil/frame.h"
//
//#include "../FFMpeg/include/libavcodec/avfft.h"
//#include "../FFMpeg/include/libavcodec/d3d11va.h"
//#include "../FFMpeg/include/libavcodec/dv_profile.h"
//#include "../FFMpeg/include/libavcodec/dxva2.h"
//#include "../FFMpeg/include/libavcodec/old_codec_ids.h"
//#include "../FFMpeg/include/libavcodec/qsv.h"
//#include "../FFMpeg/include/libavcodec/vaapi.h"
//#include "../FFMpeg/include/libavcodec/vda.h"
//#include "../FFMpeg/include/libavcodec/vdpau.h"
//#include "../FFMpeg/include/libavcodec/version.h"
//#include "../FFMpeg/include/libavcodec/videotoolbox.h"
//#include "../FFMpeg/include/libavcodec/vorbis_parser.h"
//#include "../FFMpeg/include/libavcodec/xvmc.h"
//
//#include "../FFMpeg/include/libavutil/adler32.h"
//#include "../FFMpeg/include/libavutil/aes.h"
//#include "../FFMpeg/include/libavutil/attributes.h"
//#include "../FFMpeg/include/libavutil/audio_fifo.h"
//#include "../FFMpeg/include/libavutil/audioconvert.h"
//#include "../FFMpeg/include/libavutil/avassert.h"
//#include "../FFMpeg/include/libavutil/avconfig.h"
//#include "../FFMpeg/include/libavutil/avstring.h"
//#include "../FFMpeg/include/libavutil/avutil.h"
//#include "../FFMpeg/include/libavutil/base64.h"
//
//#include "../FFMpeg/include/libavutil/blowfish.h"
//#include "../FFMpeg/include/libavutil/bprint.h"
//#include "../FFMpeg/include/libavutil/bswap.h"
//#include "../FFMpeg/include/libavutil/buffer.h"
//#include "../FFMpeg/include/libavutil/camellia.h"
//#include "../FFMpeg/include/libavutil/cast5.h"
//#include "../FFMpeg/include/libavutil/channel_layout.h"
//#include "../FFMpeg/include/libavutil/common.h"
//#include "../FFMpeg/include/libavutil/cpu.h"
//#include "../FFMpeg/include/libavutil/crc.h"
//
//#include "../FFMpeg/include/libavutil/dict.h"
//#include "../FFMpeg/include/libavutil/display.h"
//#include "../FFMpeg/include/libavutil/downmix_info.h"
//#include "../FFMpeg/include/libavutil/error.h"
//#include "../FFMpeg/include/libavutil/eval.h"
//#include "../FFMpeg/include/libavutil/ffversion.h"
//#include "../FFMpeg/include/libavutil/fifo.h"
//#include "../FFMpeg/include/libavutil/file.h"
//
//#include "../FFMpeg/include/libavutil/hash.h"
//
//#include "../FFMpeg/include/libavutil/hmac.h"
//#include "../FFMpeg/include/libavutil/imgutils.h"
//#include "../FFMpeg/include/libavutil/intfloat.h"
//#include "../FFMpeg/include/libavutil/intreadwrite.h"
//#include "../FFMpeg/include/libavutil/lfg.h"
//#include "../FFMpeg/include/libavutil/log.h"
//#include "../FFMpeg/include/libavutil/lzo.h"
//#include "../FFMpeg/include/libavutil/macros.h"
//#include "../FFMpeg/include/libavutil/mathematics.h"
//#include "../FFMpeg/include/libavutil/md5.h"
//
//#include "../FFMpeg/include/libavutil/mem.h"
//#include "../FFMpeg/include/libavutil/motion_vector.h"
//#include "../FFMpeg/include/libavutil/murmur3.h"
//#include "../FFMpeg/include/libavutil/old_pix_fmts.h"
//#include "../FFMpeg/include/libavutil/opt.h"
//#include "../FFMpeg/include/libavutil/parseutils.h"
//#include "../FFMpeg/include/libavutil/pixdesc.h"
//#include "../FFMpeg/include/libavutil/pixelutils.h"
//#include "../FFMpeg/include/libavutil/pixfmt.h"
//#include "../FFMpeg/include/libavutil/random_seed.h"
//
//#include "../FFMpeg/include/libavutil/rational.h"
//#include "../FFMpeg/include/libavutil/replaygain.h"
//#include "../FFMpeg/include/libavutil/ripemd.h"
//#include "../FFMpeg/include/libavutil/samplefmt.h"
//#include "../FFMpeg/include/libavutil/sha.h"
//#include "../FFMpeg/include/libavutil/sha512.h"
//#include "../FFMpeg/include/libavutil/stereo3d.h"
//#include "../FFMpeg/include/libavutil/tea.h"
//#include "../FFMpeg/include/libavutil/threadmessage.h"
//#include "../FFMpeg/include/libavutil/time.h"
//
//#include "../FFMpeg/include/libavutil/timecode.h"
//#include "../FFMpeg/include/libavutil/timestamp.h"
//#include "../FFMpeg/include/libavutil/twofish.h"
//#include "../FFMpeg/include/libavutil/version.h"
//#include "../FFMpeg/include/libavutil/xtea.h"
//
//#include "../FFMpeg/include/libavdevice/avdevice.h"
//#include "../FFMpeg/include/libavdevice/version.h"
//
//#include "../FFMpeg/include/libavfilter/asrc_abuffer.h"
//#include "../FFMpeg/include/libavfilter/avfilter.h"
//#include "../FFMpeg/include/libavfilter/avfiltergraph.h"
//#include "../FFMpeg/include/libavfilter/buffersink.h"
//#include "../FFMpeg/include/libavfilter/buffersrc.h"
//#include "../FFMpeg/include/libavfilter/version.h"
//
//#include "../FFMpeg/include/libavformat/avformat.h"
//#include "../FFMpeg/include/libavformat/avio.h"
//#include "../FFMpeg/include/libavformat/version.h"
//
//#include "../FFMpeg/include/libswscale/swscale.h"
//#include "../FFMpeg/include/libswscale/version.h"
//
//#include "../FFMpeg/include/libpostproc/version.h"
//#include "../FFMpeg/include/libpostproc/postprocess.h"
//
//#include "../FFMpeg/include/libswresample/version.h"
//#include "../FFMpeg/include/libswresample/swresample.h"


#include "../FFMpeg/include/libavcodec/avcodec.h"
#include "../FFMpeg/include/libavutil/frame.h"
#include "../FFMpeg/include/libavformat/avformat.h"

#include "../FFMpeg/include/libswresample/swresample.h"

#include "../FFMpeg/include/libavutil/samplefmt.h"
#include "../FFMpeg/include/libavutil/attributes.h"
#include "../FFMpeg/include/libavutil/avutil.h"
#include "../FFMpeg/include/libavutil/buffer.h"
#include "../FFMpeg/include/libavutil/cpu.h"
#include "../FFMpeg/include/libavutil/channel_layout.h"
#include "../FFMpeg/include/libavutil/dict.h"
#include "../FFMpeg/include/libavutil/frame.h"
#include "../FFMpeg/include/libavutil/log.h"
#include "../FFMpeg/include/libavutil/pixfmt.h"
#include "../FFMpeg/include/libavutil/rational.h"

#include "../FFMpeg/include/libavutil/avutil.h"
#include "../FFMpeg/include/libavutil/buffer.h"
#include "../FFMpeg/include/libavutil/dict.h"
#include "../FFMpeg/include/libavutil/rational.h"
#include "../FFMpeg/include/libavutil/samplefmt.h"
#include "../FFMpeg/include/libavutil/pixfmt.h"
#include "../FFMpeg/include/libavutil/version.h"


#include "../FFMpeg/include/libavcodec/avcodec.h"
#include "../FFMpeg/include/libavutil/dict.h"
#include "../FFMpeg/include/libavutil/log.h"

#include "../FFMpeg/include/libavformat/avio.h"

#include "../SDL/include/SDL.h"
#include "../SDL/include/SDL_thread.h"
#include "../SDL/include/SDL_main.h"
#include "../SDL/include/SDL_stdinc.h"
#include "../SDL/include/SDL_assert.h"
#include "../SDL/include/SDL_atomic.h"
#include "../SDL/include/SDL_audio.h"
#include "../SDL/include/SDL_clipboard.h"
#include "../SDL/include/SDL_cpuinfo.h"
#include "../SDL/include/SDL_endian.h"
#include "../SDL/include/SDL_error.h"
#include "../SDL/include/SDL_events.h"
#include "../SDL/include/SDL_filesystem.h"
#include "../SDL/include/SDL_joystick.h"
#include "../SDL/include/SDL_gamecontroller.h"
#include "../SDL/include/SDL_haptic.h"
#include "../SDL/include/SDL_hints.h"
#include "../SDL/include/SDL_loadso.h"
#include "../SDL/include/SDL_log.h"
#include "../SDL/include/SDL_messagebox.h"
#include "../SDL/include/SDL_mutex.h"
#include "../SDL/include/SDL_power.h"
#include "../SDL/include/SDL_render.h"
#include "../SDL/include/SDL_rwops.h"
#include "../SDL/include/SDL_system.h"
#include "../SDL/include/SDL_thread.h"
#include "../SDL/include/SDL_timer.h"
#include "../SDL/include/SDL_version.h"
#include "../SDL/include/SDL_video.h"


#define MAX_AUDIO_FRAME_SIZE 192000
#define VIDEO_PICTURE_QUEUE_SIZE 1

#define TAG "DrangerPlayer"

#define FLUSH "FLUSH"

#define FF_REFRESH_EVENT (SDL_USEREVENT)
#define FF_QUIT_EVENT (SDL_USEREVENT + 1)

#define MAX_AUDIOQ_SIZE (5 * 16 * 1024)
#define MAX_VIDEOQ_SIZE (5 * 256 * 1024)

#define AV_SYNC_THRESHOLD 0.01
#define AV_NOSYNC_THRESHOLD 10.0

#define AUDIO_DIFF_AVG_NB 16

#define SDL_AUDIO_BUFFER_SIZE 1024
#define SAMPLE_CORRECTION_PERCENT_MAX 10


/**
 * The sync type of this player
 */
enum {
    AV_SYNC_AUDIO_MASTER,
    AV_SYNC_VIDEO_MASTER,
    AV_SYNC_EXTERNAL_MASTER
};


typedef struct PacketQueen {
    AVPacketList *first_pkt;
    AVPacketList *last_pkt;

    int nb_packets; //The number of packets
    int size;   //The size of all the packets

    SDL_mutex *mutex;
    SDL_cond *cond;
} PacketQueen;

typedef struct VideoPicture {
    AVFrame *frame;
    int allocated;
    double pts_seconds;
} VideoPicture;


typedef struct VideoPictureQueue {
    VideoPicture queue[VIDEO_PICTURE_QUEUE_SIZE];
    int pictq_size, pictq_windex, pictq_rindex;
    SDL_mutex *mutex;
    SDL_cond *cond;
} VideoPictureQueue;


/**
 * The state of this player
 */
typedef struct VideoState {
    AVFormatContext *pFormatCtx;    // AVFormatContext is the format context of MediaFile ,the biggest structure in FFMpeg.

    int av_sync_type;   // The sync type of this player.Since video & audio need to sync to each other ,a sync operation is needed.

    /*
     * External clock ,ie computer clock
     */
    double external_clock; // external clock base
    double external_clock_time;

    /**
     * Audio
     */
    int audio_stream;
    AVStream *audio_st;     // AVStream is one component of a Media file's components
    AVCodecContext *audio_ctx;  //AVCodecContext is the codec context of an AVCodec. No need to keep AVCodec because we can get it from AVStream/AVCodecContext/etc
    PacketQueen audioq;
    int audio_byte_per_sample; // each audio sample's byte count
    uint8_t audio_buf[(MAX_AUDIO_FRAME_SIZE * 3) / 2];
    unsigned int audio_buf_size;
    unsigned int audio_buf_index;
    AVFrame audio_frame;
    AVPacket audio_pkt;
    uint8_t *audio_pkt_data;    //The data of AVPacket
    int audio_pkt_size; //The size of remaining data of AVPacket
    //audio clock start here
    double audio_clock; //The clock of audio decoding
    int audio_hw_buf_size;
    double audio_diff_cum;
    double audio_diff_avg_coef; //?????
    double audio_diff_threshold;    //The threshold of audio delay
    int audio_diff_avg_count;
    //staff for audio playing
    SDL_AudioSpec *sdl_audioSpec;
    SwrContext *audio_swr_ctx;
    int audio_swr_inited;


    /**
     * Video
     */
    int video_stream;
    AVStream *video_st;
    AVCodecContext *video_ctx;
    PacketQueen videoq;
    struct SwsContext *sws_ctx;
    struct VideoPictureQueue pictq;
    //video clock start here
    double video_clock;
    double video_current_pts;
    int64_t video_current_pts_time;
    // The real world timer
    double frame_timer;
    double frame_last_pts;
    double frame_last_delay;
    //staff for video playing
    SDL_Window *sdl_window;
    SDL_Rect sdl_rect;
    SDL_Renderer *sdl_render;
    SDL_Texture *sdl_texture;


    /**
     * Others
     */

    SDL_Thread *parse_tid;
    SDL_Thread *video_tid;

    char *file_name;
    int quit;

} VideoState;


VideoState *global_video_state;

AVPacket flush_pkt;

/**
 * Logging the packet
 */
static void log_packet(const AVFormatContext *fmt_ctx, const AVPacket *pkt) {
    AVRational *time_base = &fmt_ctx->streams[pkt->stream_index]->time_base;

    LOGV(TAG,
         "pts:%s pts_time:%s dts:%s dts_time:%s duration:%s duration_time:%s stream_index:%d\n",
         av_ts2str(pkt->pts), av_ts2timestr(pkt->pts, time_base),
         av_ts2str(pkt->dts), av_ts2timestr(pkt->dts, time_base),
         av_ts2str(pkt->duration), av_ts2timestr(pkt->duration, time_base),
         pkt->stream_index);
}


/**
 * Init the queue
 */
static void packet_queue_init(PacketQueen *q) {
    memset(q, 0, sizeof(PacketQueen));
    q->mutex = SDL_CreateMutex();
    q->cond = SDL_CreateCond();
}

/**
 * Putting an AVPacket
 */
static int packet_queue_put(PacketQueen *q, AVPacket *pkt) {
    AVPacketList *pkt_list;
    if (av_dup_packet(pkt) < 0) {
        return -1;
    }

    pkt_list = av_malloc(sizeof(AVPacketList));
    if (!pkt_list) {
        return -1;
    }

    pkt_list->pkt = pkt;
    pkt_list->next = NULL;

    SDL_LockMutex(q->mutex);

    if (!q->last_pkt) {
        q->first_pkt = pkt_list;
    } else {
        q->last_pkt->next = pkt_list;
    }
    q->last_pkt = pkt_list;
    q->nb_packets++;
    q->size += pkt->size;

    SDL_CondSignal(q->cond);

    SDL_UnlockMutex(q->mutex);
    return 0;
}


/**
 * Getting an AVPacket
 */
static int packet_queue_get(PacketQueen *q, AVPacket *pkt, int block) {
    AVPacketList *pkt_list;
    int ret;

    SDL_LockMutex(q->mutex);

    for (; ;) {
        if (global_video_state->quit) {
            ret = -1;
            break;
        }

        pkt_list = q->last_pkt;
        if (pkt_list) {
            q->first_pkt = pkt_list->next;
            if (!q->first_pkt) {
                q->last_pkt = NULL;
            }
            q->nb_packets--;
            q->size -= pkt->size;
            *pkt = pkt_list->pkt;
            av_free(pkt_list);
            ret = 1;
            break;
        } else if (!block) {
            ret = -1;
            break;
        } else {
            SDL_CondWait(q->cond, q->mutex);
            continue;
        }
    }

    SDL_UnlockMutex(q->mutex);

    return ret;
}

void quit() {
    SDL_Event event;
    event.type = FF_QUIT_EVENT;
    event.user.data1 = global_video_state;
    SDL_PumpEvents(event);
}


/**
 * Getting the audio clock
 */
double get_audio_clock(VideoState *is) {
    double pts = is->audio_clock;

    int hw_buf_size = is->audio_buf_size - is->audio_buf_index;
    double hw_buf_size_time = (double) hw_buf_size /
                              (double) (is->audio_ctx->channels * 2 * is->audio_ctx->sample_rate);
    pts -= hw_buf_size;
    return pts;
}


double get_video_clock(VideoState *is) {
    double delta = (av_gettime() - is->video_current_pts_time) / 1000000.0;
    return is->video_current_pts + delta;
}

/**
 * This is not right ! Make a fix to it
 */
double get_external_clock(VideoState *is) {
    return av_gettime() / 1000000.0;
}


double get_master_clock(VideoState *is) {
    switch (is->av_sync_type) {
        case AV_SYNC_AUDIO_MASTER:
            return get_audio_clock(is);
        case AV_SYNC_VIDEO_MASTER:
            return get_video_clock(is);
        default:
            return get_external_clock(is);
    }
}


/**
 * Trim the audio samples for synchronise
 */
int synchronize_audio(VideoState *is, short *samples, int sample_size, double pts) {
    if (is->av_sync_type != AV_SYNC_AUDIO_MASTER) {
        double diff = get_audio_clock(is) - get_master_clock(is);
        if (diff < AV_NOSYNC_THRESHOLD) {
            is->audio_diff_cum = diff + is->audio_diff_avg_coef * is->audio_diff_cum;
            if (is->audio_diff_avg_count < AUDIO_DIFF_AVG_NB) {
                is->audio_diff_avg_count++;
            } else {
                double avg_diff = diff * (1.0 - is->audio_diff_avg_coef);

                if (fabs(avg_diff) >= is->audio_diff_threshold) {
                    int wanted_size = sample_size + (int) (diff * is->audio_st->codec->sample_rate *
                                                           is->audio_byte_per_sample);
                    int min_size = sample_size * ((100 - SAMPLE_CORRECTION_PERCENT_MAX) / 100);
                    int max_size = sample_size * ((100 + SAMPLE_CORRECTION_PERCENT_MAX) / 100);

                    if (wanted_size < min_size) {
                        wanted_size = min_size;
                    } else if (wanted_size > max_size) {
                        wanted_size = max_size;
                    }

                    if (wanted_size < sample_size) {
                        // just remove it
                        sample_size = wanted_size;
                    } else if (wanted_size > sample_size) {
                        int byte_per_sample = is->audio_ctx->channels * 2;
                        int extend_nb = wanted_size - sample_size;
                        uint8_t *samples_extent = samples + sample_size;
                        uint8_t *samples_end_pre = samples_extent - byte_per_sample;

                        while (extend_nb > 0) {
                            memcpy(samples_extent, samples_end_pre, byte_per_sample);
                            samples_extent += byte_per_sample;
                            extend_nb -= byte_per_sample;
                        }
                        sample_size = wanted_size;
                    }
                }
            }
        } else {
            is->audio_diff_avg_count = 0;
            is->audio_diff_cum = 0;
        }
    }
    return sample_size;
}


/**
 * Convert audio to AV_SAMPLE_FMT_S16 ,and result data would replace the original ones in AVFrame
 */
int audio_convert_audio_format(VideoState *is, AVFrame *decoded_frame) {
    int ret;
    int64_t channel_layout;
    static enum AVSampleFormat DST_SAMPLE_FMT = AV_SAMPLE_FMT_S16;
    if (decoded_frame->channel_layout == NULL) {
        decoded_frame->channel_layout = av_get_default_channel_layout(decoded_frame->channels);
    }

    // we init here not #stream_component_open because AVFrame contains more specific information
    if (!is->audio_swr_inited) {
        av_opt_set_int(is->audio_swr_ctx, "in_channel_layout", channel_layout, 0);
        av_opt_set_int(is->audio_swr_ctx, "out_channel_layout", channel_layout, 0);
        av_opt_set_int(is->audio_swr_ctx, "in_sample_rate", decoded_frame->sample_rate, 0);
        av_opt_set_int(is->audio_swr_ctx, "out_sample_rate", decoded_frame->sample_rate, 0);
        av_opt_set_int(is->audio_swr_ctx, "in_sample_fmt", decoded_frame->format, 0);
        av_opt_set_int(is->audio_swr_ctx, "out_sample_fmt", DST_SAMPLE_FMT, 0);

        ret = swr_init(is->audio_swr_ctx);
        if (ret < 0) {
            LOGE(TAG, "Can not inti SWRContext for AudioConverter,errMsg -> %s", av_err2str(ret));
            return -1;
        }
        is->audio_swr_inited = 1;
    }

    const uint8_t **src = (const uint8_t **) decoded_frame->data;
    uint8_t **dst;
    int dst_linesize;

    ret = av_samples_alloc_array_and_samples(&dst, &dst_linesize, decoded_frame->channels,
                                             decoded_frame->nb_samples, DST_SAMPLE_FMT, 0);
    if (ret < 0) {
        LOGE(TAG, "Can not alloc array and samples ,errMsg -> %s", av_err2str(ret));
        return -1;
    }

    ret = swr_convert(is->sws_ctx, dst, decoded_frame->nb_samples, src, decoded_frame->nb_samples);
    if (ret < 0) {
        LOGE(TAG, "Can not convert audio to PCM S16 ,errMsg -> %s", av_err2str(ret));
        return -1;
    }

    decoded_frame->linesize = dst_linesize;
    decoded_frame->format = DST_SAMPLE_FMT;
    decoded_frame->data[0] = dst[0];

    if (src != NULL) {
        av_freep(src);
        av_free(&src);
    }

    return 0;
}


/**
 * 1 decode the AVPacket from AudioQueen ,and return the decoded bytes ,and has the ability of
 *      decoding remaining data of AVPacket
 *
 * 2 set the audio clock ,ie the pts time of the audio
 */
int audio_decode_frame(VideoState *is, uint8_t *buffer, int buffer_size, double *pts) {
    AVPacket *pkt = &is->audio_pkt;
    int ret;
    int got_frame = 0;
    AVFrame *frame;

    //is->audio_clock is up to the AVPacket read index
    for (; ;) {
        while (is->audio_pkt_size > 0) {
            int len_consumed = avcodec_decode_audio4(is->audio_ctx, frame, &got_frame,
                                                     &is->audio_pkt);
            if (len_consumed <= 0) {
                LOGW(TAG, "Decoded none data for audio ,skip this AVPacket.errMsg -> %s",
                     av_err2str(len_consumed));
                is->audio_pkt_size = 0;
                break;
            }

            int data_size = 0;
            if (got_frame) {

                if (frame->format != AV_SAMPLE_FMT_S16) {
                    ret = audio_convert_audio_format(is, frame);
                    if (ret < 0) {
                        LOGE(TAG, "Error when converting audio format to target S16!");
                        return -1;
                    }
                }

                // calculate the size of one audio frame
                data_size = av_samples_get_buffer_size(NULL, frame->channels,
                                                       frame->nb_samples,
                                                       frame->format, 1);

                assert(data_size <= buffer_size);
                memcpy(buffer, frame->data[0], data_size);
            }

            is->audio_pkt_size -= len_consumed;
            is->audio_pkt_data += len_consumed;

            if (data_size <= 0) {
                // got no data from the AVPacket
                continue;
            }

            // the pts of the last AVPacket
            *pts = is->audio_clock;
            is->audio_clock += ((double) data_size / (double) *is->audio_byte_per_sample) *
                               (double) av_q2d(is->audio_st->time_base);

            return data_size;
        }

        if (&is->audio_pkt != NULL && &is->audio_pkt->data != NULL) {
            av_free_packet(&is->audio_pkt);
        }

        if (is->quit) {
            return -1;
        }

        ret = packet_queue_get(is->audioq, &is->audio_pkt, 1);
        if (ret < 0) {
            LOGE(TAG, "Can not get audio packet !");
            return -1;
        }

        if (&is->audio_pkt->data == flush_pkt.data) {
            //TODO Flush Action
            continue;
        }

        is->audio_pkt_data = is->audio_pkt.data;
        is->audio_pkt_size = is->audio_pkt.size;

        //record last AVPacket's pts
        if (is->audio_pkt.pts != AV_NOPTS_VALUE) {
            is->audio_clock = is->audio_pkt.pts * av_q2d(is->audio_st->time_base);
        }
    }
}

/**
 * Audio callbacks from SDL
 *
 */
void audio_callback(void *args, Uint8 *stream, int len) {
    VideoState *is = (VideoState *) args;
    int len1, audio_size;
    double pts;

    while (len > 0) {
        if (is->audio_buf_index >= is->audio_buf_size) {
            audio_size = audio_decode_frame(is, is->audio_buf, sizeof(is->audio_buf), &pts);

            if (audio_size < 0) {
                LOGW("No audio decoded , set buffer to zero .");
                is->audio_buf_size = is->audio_ctx->frame_size;
                memset(is->audio_buf, 0, is->audio_buf_size);
            } else {
                audio_size = synchronize_audio(is, (uint16_t *) is->audio_buf, audio_size, pts);
                is->audio_buf_size = audio_size;
            }
            is->audio_buf_index = 0;
        }

        int delta = is->audio_buf_size - is->audio_buf_index;
        int len_read = delta > len ? len : delta;

        memcpy(stream, (uint8_t *) is->audio_buf, len_read);

        is->audio_buf_index += len_read;
        len -= len_read;
        stream += len_read;
    }
}

static Uint32 sdl_refresh_timer_cb(Uint32 interval, void *opaque) {
    SDL_Event event;
    event.type = FF_REFRESH_EVENT;
    event.user.data1 = opaque;
    SDL_PumpEvents(&event);
    return 0;
}

static void schedule_refresh(VideoState *is, int delay) {
    SDL_AddTimer(delay, sdl_refresh_timer_cb, is);
}

/**
 * Displaying the video actually
 */
void video_display(VideoState *is) {
    VideoPicture *vp = is->pictq.queue[is->pictq.pictq_rindex];

    SDL_UpdateYUVTexture(is->sdl_texture, &is->sdl_rect,
                         vp->frame->data[0], vp->frame->linesize[0],
                         vp->frame->data[1], vp->frame->linesize[1],
                         vp->frame->data[2], vp->frame->linesize[2]
    );
    SDL_RenderClear(is->sdl_render);
    SDL_RenderCopy(is->sdl_render, is->sdl_texture, NULL, &is->sdl_rect);
    SDL_RenderPresent(is->sdl_render);
}


void video_refresh_timer(void *userdata) {
    VideoState *is = (VideoState *) userdata;
    VideoPicture *vp;
    double delay, sync_threshold;
    if (is->video_st) {
        if (is->pictq.pictq_size == 0) {
            schedule_refresh(is, 1);
        } else {
            //we got some data for reading
            vp = &is->pictq.queue[is->pictq.pictq_rindex];

            is->video_current_pts = vp->pts_seconds;
            is->video_current_pts_time = av_gettime();

            delay = vp->pts_seconds - is->frame_last_pts;
            if (delay <= 0 || delay > 1.0) {
                delay = is->frame_last_delay;
            }

            is->frame_last_pts = vp->pts_seconds;
            is->frame_last_delay = delay;

            // if not video master ,sync video to master
            if (is->av_sync_type != AV_SYNC_VIDEO_MASTER) {
                double diff = vp->pts_seconds - get_master_clock(is);
                sync_threshold = (delay > AV_SYNC_THRESHOLD) ? delay : AV_SYNC_THRESHOLD;
                if (fabs(diff) < AV_NOSYNC_THRESHOLD) {
                    if (diff <= -sync_threshold) {
                        delay = 0;
                    } else if (diff > sync_threshold) {
                        delay = 2 * delay;
                    }
                }
            }

            is->frame_timer += delay;
            double actual_delay = is->frame_timer - av_gettime() / 1000000.0;

            if (actual_delay < 0.01) {
                actual_delay = 0.01;
            }

            schedule_refresh(is, actual_delay);

            video_display(is);
            if (++is->pictq.pictq_size == VIDEO_PICTURE_QUEUE_SIZE) {
                is->pictq.pictq_size = 0;
            }

            SDL_LockMutex(is->pictq.mutex);
            is->pictq.pictq_size--;
            SDL_CondSignal(is->pictq.cond);
            SDL_UnlockMutex(is->pictq.mutex);

        }


    } else {
        schedule_refresh(is, 100);
    }
}

/**
 * Queue picture for SDL
 */
int queue_picture(VideoState *is, AVFrame *frame, double pts_second) {
    VideoPicture *vp;

    SDL_LockMutex(is->pictq.mutex);
    while (is->pictq.pictq_size >= VIDEO_PICTURE_QUEUE_SIZE && !is->quit) {
        SDL_CondWait(is->pictq.cond, is->pictq.mutex);
    }
    SDL_UnlockMutex(is->pictq.mutex);

    //check for video thread
    if (is->quit) {
        return -1;
    }

    vp = is->pictq.queue[is->pictq.pictq_windex];
    sws_scale(is->sws_ctx, (const unsigned char *const *) frame->data,
              frame->linesize, 0, is->video_ctx->height, vp->frame->data, vp->frame->linesize
    );
    vp->pts_seconds = pts_second;
    if (is->pictq.pictq_windex == VIDEO_PICTURE_QUEUE_SIZE) {
        is->pictq.pictq_windex = 0;
    }
    SDL_LockMutex(is->pictq.mutex);
    is->pictq.pictq_size++;
    SDL_UnlockMutex(is->pictq.mutex);
}


/**
 * This is something like a protector of the pts in case of pts of value '0'
 */
double synchronize_video(VideoState *is, AVFrame *frame, double pts) {
    double pts_second = pts * av_q2d(is->video_st->time_base);
    if (pts_second != 0) {
        is->video_clock = pts_second;
    } else {
        pts_second = is->video_clock;
    }

    double frame_delay = av_q2d(is->video_st->time_base);

    frame_delay += frame->repeat_pict * (frame_delay * 0.5); // why 0.5 ???
    is->video_clock += frame_delay;

    return pts_second;
}


int video_thread(void *args) {
    VideoState *is = (VideoState *) args;
    AVPacket *packet;
    double pts; //present time stamp
    int ret;

    int got_picture;

    for (; ;) {
        LOGE(TAG, "This is video_thread running ...");

        ret = packet_queue_get(is->audioq, packet, 1);
        if (ret < 0) {
            LOGE(TAG, "Can not get packet");
            break;
        }

        LOGV(TAG, "Printing the video packet:\n");
        log_packet(is->pFormatCtx, packet);

        if (packet->data == flush_pkt.data) {
            LOGE(TAG, "TODO!!! should flush buffers !");
            continue;
        }

        AVFrame *frame;

        ret = avcodec_decode_video2(is->video_ctx, frame, &got_picture, packet);
        if (ret < 0) {
            LOGE(TAG, "Error when decode video ,errMsg -> %s", av_err2str(ret));
            return -1;
        }

        pts = 0;
        if (got_picture) {
            LOGV(TAG, "Got picture ,the frame->pts %s", av_ts2str(frame->pts));
            if (frame->pts != AV_NOPTS_VALUE) {
                pts = av_frame_get_best_effort_timestamp(frame);
            } else {
                pts = 0;
            }

            double pts_seconds = synchronize_video(is, frame, pts);
            LOGV(TAG, "next_pts_seconds %lf", pts_seconds);

            ret = queue_picture(is, frame, pts_seconds);
            if (ret < 0) {
                LOGE(TAG, "Can not queue picture for SDL !");
                break;
            }

        } else {
            LOGE("Can not get picture !");
        }

        av_free_packet(packet);
    }

    return 0;
}


/**
 * Opening the stream component !
 */
int stream_component_open(VideoState *is, int stream_index) {
    LOGV(TAG, "stream_component_open -> %d", stream_index);
    AVCodec *codec = NULL;
    int ret;

    AVStream *stream = is->pFormatCtx->streams[stream_index];

    codec = avcodec_find_decoder(stream->codec->codec->id);
    if (codec == NULL) {
        LOGE(TAG, "Can not find codec !! stream_index -> %d ", stream_index);
        return -1;
    }

    ret = avcodec_open2(stream->codec, codec, NULL);
    if (ret < 0) {
        LOGE(TAG, "Can not open codec ,errMsg -> %s", av_err2str(ret));
        return -1;
    }

    switch (codec->type) {
        case AVMEDIA_TYPE_AUDIO:
            SDL_AudioSpec wanted_spec, spec;
            wanted_spec.freq = stream->codec->sample_rate;
            wanted_spec.format = AUDIO_S16SYS;  //display for the target system
            wanted_spec.channels = stream->codec->channels;
            wanted_spec.silence = 0;
            wanted_spec.samples = SDL_AUDIO_BUFFER_SIZE;
            if (wanted_spec.samples <= 0) {
                wanted_spec.samples = 1024; // aac 1024 mp3 1152
            }
            wanted_spec.callback = audio_callback;
            wanted_spec.userdata = is;

            ret = SDL_OpenAudio(&wanted_spec, &spec);
            if (ret < 0) {
                LOGE(TAG, "Error when opening audio device ! errMsg -> %s", SDL_GetError());
                return -1;
            }

            is->audio_hw_buf_size = spec.size;
            is->audio_st = stream;
            is->audio_ctx = stream->codec;
            //AUDIO Format PCM16  2 bytes/channel
            is->audio_byte_per_sample = 2 * stream->codec->channels;
            is->audio_buf_size = 0;
            is->audio_buf_index = 0;

            is->audio_diff_avg_coef = exp(log(0.01 / AUDIO_DIFF_AVG_NB));
            is->audio_diff_avg_count = 0;
            is->audio_diff_threshold = 2.0 * SDL_AUDIO_BUFFER_SIZE / stream->codec->sample_rate;

            // may need audio
            is->audio_swr_ctx = swr_alloc();
            if (is->audio_swr_ctx != NULL) {
                LOGE("Can not init swr context !");
                return -1;
            }
            is->audio_swr_inited = 0;

            memset(&is->audio_pkt, 0, sizeof(is->audio_pkt));   // this method useful ???
            packet_queue_init(&is->audioq);
            SDL_PauseAudio(0);

            LOGV(TAG, "Audio Component Initialized .");
            break;

        case AVMEDIA_TYPE_VIDEO:
            is->video_st = stream;
            is->video_ctx = stream->codec;

            is->frame_timer = av_gettime() / 1000000.0;
            is->frame_last_delay = 40e-3;

            is->video_current_pts_time = av_gettime();

            packet_queue_init(&is->videoq);

            is->sws_ctx = sws_getContext(stream->codec->width,
                                         stream->codec->height,
                                         stream->codec->pix_fmts,
                                         stream->codec->width, stream->codec->height,
                                         PIX_FMT_YUV420P,
                                         SWS_BICUBIC, NULL, NULL, NULL
            );

            is->sdl_window = SDL_CreateWindow("Dranger Player", SDL_WINDOWPOS_CENTERED,
                                              SDL_WINDOWPOS_CENTERED, stream->codec->width,
                                              stream->codec->height, SDL_WINDOW_OPENGL);
            is->sdl_rect.x = 0;
            is->sdl_rect.y = 0;
            is->sdl_rect.w = stream->codec->width;
            is->sdl_rect.h = stream->codec->height;

            is->sdl_render = SDL_CreateRenderer(is->sdl_window, -1, 0);
            is->sdl_texture = SDL_CreateTexture(is->sdl_render, SDL_PIXELFORMAT_IYUV,
                                                SDL_TEXTUREACCESS_STREAMING, stream->codec->width,
                                                stream->codec->height);

            //create video thread for image display ,make video display behaviors like audio
            is->video_tid = SDL_CreateThread(video_thread, "Video Thread", is);

            if (is->video_tid == NULL) {
                LOGE(TAG, "Can not create video thread !");
                return -1;
            }

            LOGV(TAG, "Video Component Initialized .");
            break;
    }

    return 0;
}


/**
 * Decoding thread
 */
int decode_thread(void *args) {
    VideoState *is = (VideoState *) args;

    int i = 0;

    is->audio_stream = -1;
    is->video_stream = -1;

    int ret;
    ret = avformat_open_input(&is->pFormatCtx, is->file_name, NULL, NULL);
    if (ret < 0) {
        LOGE(TAG, "Can not open input ,code is %s", av_err2str(ret));
        return -1;
    }

    ret = avformat_find_stream_info(is->pFormatCtx, NULL);
    if (ret < 0) {
        LOGW(TAG, "Can not find any stream info !");
        return -1;
    }

    for (i = 0; i < is->pFormatCtx->nb_streams; i++) {
        if (is->pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
            is->audio_stream = i;
        } else if (is->pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            is->video_stream = i;
        }
    }

    LOGD(TAG, "Audio Stream -> %d\nVideo Stream -> %d", is->audio_stream, is->video_stream);

    if (is->audio_stream >= 0) {
        ret = stream_component_open(is, is->audio_stream);
        if (ret < 0) {
            LOGE("Error when open audio stream !");
            fail();
            return -1;
        }
    }

    if (is->video_stream >= 0) {
        // try open video stream
        ret = stream_component_open(is, is->video_stream);
        if (ret < 0) {
            LOGE("Error when open video stream !");
            fail();
            return -1;
        }
    }

    for (; ;) {
        if (is->quit) {
            break;
        }

        //ok doing some decode staff here
        if (is->audioq.size > MAX_AUDIOQ_SIZE || is->videoq.size > MAX_VIDEOQ_SIZE) {
            LOGD(TAG, "AudioQueen or VideoQueen has reached the limits ,wait a while ...");
            SDL_Delay(10);
            continue;
        }

        AVPacket packet;
        ret = av_read_frame(is->pFormatCtx, &packet);
        if (ret < 0) {
            if (is->pFormatCtx->pb->error == 0) {
                SDL_Delay(100); /* no error; wait for user input */
                continue;
            } else {
                LOGE("Has some IO error when read frame ! errMsg -> %s +++ %s", av_err2str(ret),
                     av_err2str(is->pFormatCtx->pb->error));
                break;
            }
        }

        if (packet.stream_index == is->audio_stream) {
            packet_queue_put(is->audioq, &packet);
        } else if (packet.stream_index == is->video_stream) {
            packet_queue_put(is->videoq, &packet);
        } else {
            av_free_packet(&packet);
        }

    }

    while (!is->quit) {
        SDL_Delay(100);
    }

    quit();
    return 0;
}


/**
 * Starting the player for test
 */
int start(const char *videoPath) {
    LOGV(TAG, "start -> %s", videoPath);

    VideoState *is;
    is = av_malloc(sizeof(VideoState));

    av_register_all();
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
        Log_e(TAG, "Error when init SDL !");
        av_free(is);
        return -1;
    }

    is->file_name = videoPath;

    packet_queue_init(is->audioq);
    packet_queue_init(is->videoq);

    av_init_packet(&flush_pkt);
    flush_pkt.data = FLUSH;

    is->av_sync_type = AV_SYNC_AUDIO_MASTER;
    is->parse_tid = SDL_CreateThread(decode_thread, "Player Decode Thread", is);
    if (is->parse_tid == NULL) {
        av_free(is);
        return -1;
    }

    schedule_refresh(is, 100);

    SDL_Event *sdl_event;
    for (; ;) {
        SDL_WaitEvent(sdl_event);
        switch (sdl_event->type) {
            case SDL_KEYDOWN:
                break;
            case SDL_QUIT:
            case FF_QUIT_EVENT:
                is->quit = 1;
                SDL_CondSignal(is->audioq.cond);
                SDL_CondSignal(is->videoq.cond);
                SDL_Quit();
                return 0;
            case FF_REFRESH_EVENT:
                //refresh video here ,learn from audio refresh
                video_refresh_timer((void *) sdl_event->user.data1);
                break;
        }
    }
}


int stop() {

}


int main(int argc, char *argv[]) {
    LOGV(TAG, "main -> %s", argv[1]);
    start(argv[1]);
}


