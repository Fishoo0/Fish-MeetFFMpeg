/**
 * This is a demo player for Android by SDL2.0
 */

#include <math.h>
#include <stdio.h>
#include <assert.h>

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h> // software scaling lib

#include <SDL.h>
#include <SDL_thread.h>

#include "my_log.h"

#include "volador_demoffmpeg_JNIMain.h"

#define DEBUG_INCLUDE

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


#ifdef DEBUG_INCLUDE
//headers for FFMpeg

#include "../FFMpeg/include/libavcodec/avfft.h"
#include "../FFMpeg/include/libavcodec/d3d11va.h"
#include "../FFMpeg/include/libavcodec/dv_profile.h"
#include "../FFMpeg/include/libavcodec/dxva2.h"
#include "../FFMpeg/include/libavcodec/old_codec_ids.h"
#include "../FFMpeg/include/libavcodec/qsv.h"
#include "../FFMpeg/include/libavcodec/vaapi.h"
#include "../FFMpeg/include/libavcodec/vda.h"
#include "../FFMpeg/include/libavcodec/vdpau.h"
#include "../FFMpeg/include/libavcodec/version.h"
#include "../FFMpeg/include/libavcodec/videotoolbox.h"
#include "../FFMpeg/include/libavcodec/vorbis_parser.h"
#include "../FFMpeg/include/libavcodec/xvmc.h"


#include "../FFMpeg/include/libavutil/adler32.h"
#include "../FFMpeg/include/libavutil/aes.h"
#include "../FFMpeg/include/libavutil/attributes.h"
#include "../FFMpeg/include/libavutil/audio_fifo.h"
#include "../FFMpeg/include/libavutil/audioconvert.h"
#include "../FFMpeg/include/libavutil/avassert.h"
#include "../FFMpeg/include/libavutil/avconfig.h"
#include "../FFMpeg/include/libavutil/avstring.h"
#include "../FFMpeg/include/libavutil/avutil.h"
//#include "../FFMpeg/include/libavutil/base64.h"

#include "../FFMpeg/include/libavutil/blowfish.h"
#include "../FFMpeg/include/libavutil/bprint.h"
#include "../FFMpeg/include/libavutil/bswap.h"
#include "../FFMpeg/include/libavutil/buffer.h"
#include "../FFMpeg/include/libavutil/camellia.h"
#include "../FFMpeg/include/libavutil/cast5.h"
#include "../FFMpeg/include/libavutil/channel_layout.h"
#include "../FFMpeg/include/libavutil/common.h"
#include "../FFMpeg/include/libavutil/cpu.h"
#include "../FFMpeg/include/libavutil/crc.h"

#include "../FFMpeg/include/libavutil/dict.h"
#include "../FFMpeg/include/libavutil/display.h"
#include "../FFMpeg/include/libavutil/downmix_info.h"
#include "../FFMpeg/include/libavutil/error.h"
#include "../FFMpeg/include/libavutil/eval.h"
#include "../FFMpeg/include/libavutil/ffversion.h"
#include "../FFMpeg/include/libavutil/fifo.h"
#include "../FFMpeg/include/libavutil/file.h"

#include "../FFMpeg/include/libavutil/hash.h"

#include "../FFMpeg/include/libavutil/hmac.h"
#include "../FFMpeg/include/libavutil/imgutils.h"
#include "../FFMpeg/include/libavutil/intfloat.h"
#include "../FFMpeg/include/libavutil/intreadwrite.h"
#include "../FFMpeg/include/libavutil/lfg.h"
#include "../FFMpeg/include/libavutil/log.h"
#include "../FFMpeg/include/libavutil/lzo.h"
#include "../FFMpeg/include/libavutil/macros.h"
#include "../FFMpeg/include/libavutil/mathematics.h"
#include "../FFMpeg/include/libavutil/md5.h"

#include "../FFMpeg/include/libavutil/mem.h"
#include "../FFMpeg/include/libavutil/motion_vector.h"
#include "../FFMpeg/include/libavutil/murmur3.h"
#include "../FFMpeg/include/libavutil/old_pix_fmts.h"
#include "../FFMpeg/include/libavutil/opt.h"
#include "../FFMpeg/include/libavutil/parseutils.h"
#include "../FFMpeg/include/libavutil/pixdesc.h"
#include "../FFMpeg/include/libavutil/pixelutils.h"
#include "../FFMpeg/include/libavutil/pixfmt.h"
#include "../FFMpeg/include/libavutil/random_seed.h"

#include "../FFMpeg/include/libavutil/rational.h"
#include "../FFMpeg/include/libavutil/replaygain.h"
#include "../FFMpeg/include/libavutil/ripemd.h"
#include "../FFMpeg/include/libavutil/samplefmt.h"
#include "../FFMpeg/include/libavutil/sha.h"
#include "../FFMpeg/include/libavutil/sha512.h"
#include "../FFMpeg/include/libavutil/stereo3d.h"
#include "../FFMpeg/include/libavutil/tea.h"
#include "../FFMpeg/include/libavutil/threadmessage.h"
#include "../FFMpeg/include/libavutil/time.h"

#include "../FFMpeg/include/libavutil/timecode.h"
#include "../FFMpeg/include/libavutil/timestamp.h"
#include "../FFMpeg/include/libavutil/twofish.h"
#include "../FFMpeg/include/libavutil/version.h"
#include "../FFMpeg/include/libavutil/xtea.h"

#include "../FFMpeg/include/libavdevice/avdevice.h"
#include "../FFMpeg/include/libavdevice/version.h"

#include "../FFMpeg/include/libavfilter/asrc_abuffer.h"
#include "../FFMpeg/include/libavfilter/avfilter.h"
#include "../FFMpeg/include/libavfilter/avfiltergraph.h"
#include "../FFMpeg/include/libavfilter/buffersink.h"
#include "../FFMpeg/include/libavfilter/buffersrc.h"
#include "../FFMpeg/include/libavfilter/version.h"

#include "../FFMpeg/include/libavformat/avformat.h"
#include "../FFMpeg/include/libavformat/avio.h"
#include "../FFMpeg/include/libavformat/version.h"

#include "../FFMpeg/include/libswscale/swscale.h"
#include "../FFMpeg/include/libswscale/version.h"

#include "../FFMpeg/include/libpostproc/version.h"
#include "../FFMpeg/include/libpostproc/postprocess.h"

#include "../FFMpeg/include/libswresample/version.h"
#include "../FFMpeg/include/libswresample/swresample.h"



//headers for SDL
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

#endif


#define MAX_AUDIO_FRAME_SIZE 192000 // 1 second of 48khz 32bit audio

// Let us beat this simple player


typedef struct MyFFMpeg {
    AVFormatContext *format_ctx;
    int videoStream;
    int audioStream;

    AVCodecContext *video_ctx;
    AVCodecContext *audio_ctx;

    AVPacket packet;

    AVFrame *video_frame;
    AVFrame *audio_frame;

    struct SwsContext *image_convert_ctx;
    SwrContext *audio_convert_ctx;


    //we need a buffer for this AVFrame since it is temp
    AVFrame *video_frame_yuv;
    int videoWidth;
    int videoHeight;

    uint8_t *audio_buffer; // for SDL reading
    uint64_t audio_out_channel_layout;
    int audio_out_nb_sample;
    int audio_out_sample_rate;
    int audio_out_channels;
    enum AVSampleFormat audio_out_sample_format;
    int audio_out_buffer_size;

    uint8_t *audio_data;
    int audio_data_len;
    int audio_data_index;

} MyFFMpeg;


typedef struct MySDL {
    SDL_Window *sdl_window;
    SDL_Renderer *sdl_renderer;
    SDL_Texture *sdl_texture;
    SDL_Rect sdl_rect;

    SDL_AudioSpec sdl_audioSpec;

} MySDL;


int initFFMpeg(MyFFMpeg *ffMpeg, char *name) {
    LOGV("initFFMpeg");

    int ret;

    av_register_all();
    avformat_network_init();

    ffMpeg->format_ctx = avformat_alloc_context();

    if (avformat_open_input(&ffMpeg->format_ctx, name, NULL, NULL) != 0) {
        LOGE("Can not open the input file !");
        return -1;
    }

    //print details
    av_dump_format(ffMpeg->format_ctx, 0, name, 0);

    if (avformat_find_stream_info(ffMpeg->format_ctx, NULL) < 0) {
        LOGE("Can not find stream info !");
        return -1;
    }

    ffMpeg->videoStream = -1;
    ffMpeg->audioStream = -1;
    int i = 0;
    for (i = 0; i < ffMpeg->format_ctx->nb_streams; i++) {
        if (ffMpeg->format_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            ffMpeg->videoStream = i;
            LOGV("Found videoStream -> %d", i);
        } else if (ffMpeg->format_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
            ffMpeg->audioStream = i;
            LOGV("Found audioStream -> %d", i);
        }
    }

    if (ffMpeg->videoStream < 0) {
        LOGE("Can not find video stream !");
        return -1;
    }

    if (ffMpeg->audioStream < 0) {
        LOGE("Can not find audio stream !");
        return -1;
    }

    //when open codec , AVCodec must be initiate yourself ,but AVCodecContext can be reused from AVCodecContext
    AVCodec *codec;

    ffMpeg->video_ctx = ffMpeg->format_ctx->streams[ffMpeg->videoStream]->codec;
    codec = avcodec_find_decoder(
            ffMpeg->format_ctx->streams[ffMpeg->videoStream]->codec->codec_id);
    if (codec == NULL) {
        LOGE("Can not find AVCodec for video !");
        return -1;
    }
    ffMpeg->videoWidth = ffMpeg->video_ctx->width;
    ffMpeg->videoHeight = ffMpeg->video_ctx->height;

    LOGV("ffMpeg->videoWidth ffMpeg->videoHeight -> %d %d", ffMpeg->videoWidth,
         ffMpeg->videoHeight);
    ret = avcodec_open2(ffMpeg->video_ctx, codec, NULL);
    if (ret < 0) {
        LOGE("Error while open video codec context ! code -> %s", av_err2str(ret));
        return -1;
    }

    ffMpeg->audio_ctx = ffMpeg->format_ctx->streams[ffMpeg->audioStream]->codec;
    codec = avcodec_find_decoder(ffMpeg->audio_ctx->codec_id);
    if (codec == NULL) {
        LOGE("Can not find audio decoder !");
        return -1;
    }
    ret = avcodec_open2(ffMpeg->audio_ctx, codec, NULL);
    if (ret < 0) {
        LOGE("Error while open audio codec context ! code -> %s ", av_err2str(ret));
        return -1;
    }

    ffMpeg->video_frame = av_frame_alloc();
//    ffMpeg->packet = (AVPacket *) av_malloc(sizeof(AVPacket));
    enum AVPixelFormat dstPixelFormat = AV_PIX_FMT_YUV420P;
    // alloc an AVFrame & setting it a buffer
    ffMpeg->video_frame_yuv = av_frame_alloc();
    unsigned char *bufferYUV = av_malloc(
            av_image_get_buffer_size(dstPixelFormat, ffMpeg->videoWidth, ffMpeg->videoHeight, 1));

    av_image_fill_arrays(ffMpeg->video_frame_yuv->data, ffMpeg->video_frame_yuv->linesize,
                         bufferYUV, dstPixelFormat, ffMpeg->videoWidth, ffMpeg->videoHeight);
    ffMpeg->image_convert_ctx = sws_getContext(ffMpeg->videoWidth,
                                               ffMpeg->videoHeight,
                                               ffMpeg->video_ctx->pix_fmt,

                                               ffMpeg->videoWidth,
                                               ffMpeg->videoHeight,
                                               dstPixelFormat,

                                               SWS_BICUBIC, NULL, NULL, NULL);


    ffMpeg->audio_frame = av_frame_alloc();
    ffMpeg->audio_out_channel_layout = AV_CH_LAYOUT_STEREO;
    ffMpeg->audio_out_nb_sample = ffMpeg->audio_ctx->frame_size;    //aac:1024 mp3:1152
    ffMpeg->audio_out_sample_rate = 44100;
    ffMpeg->audio_out_channels = av_get_channel_layout_nb_channels(
            ffMpeg->audio_out_channel_layout);
    ffMpeg->audio_out_sample_format = AV_SAMPLE_FMT_S16;
    ffMpeg->audio_out_buffer_size = av_samples_get_buffer_size(NULL, ffMpeg->audio_out_channels,
                                                               ffMpeg->audio_out_nb_sample,
                                                               ffMpeg->audio_out_sample_format, 1);

    //why bigger than MAX_AUDIO_FRAME_SIZE ? Find more info from Dranger.com
    ffMpeg->audio_buffer = (uint8_t *) av_malloc(MAX_AUDIO_FRAME_SIZE * 1.5);

    ffMpeg->audio_convert_ctx = swr_alloc_set_opts(NULL,
                                                   ffMpeg->audio_out_channel_layout,
                                                   ffMpeg->audio_out_sample_format,
                                                   ffMpeg->audio_out_sample_rate,
                                                   ffMpeg->audio_ctx->channel_layout,
                                                   ffMpeg->audio_ctx->sample_fmt,
                                                   ffMpeg->audio_ctx->sample_rate, 0, NULL);

    if (ffMpeg->audio_convert_ctx == NULL) {
        LOGE("Can not allocate audio converter !");
        return -1;
    }
    ret = swr_init(ffMpeg->audio_convert_ctx);
    if (ret != 0) {
        LOGE("Can not init audio converter ,code -> %s", av_err2str(ret));
        return -1;
    }

    //ok ,we have init the ffmpeg and converter ,next is showing them !
    return 0;
}

/**
 * When sdl want to play audio ,call this method to fetch audio data
 */
int audio_callback(void *udata, Uint8 *stream, int len) {
    LOGV("audio_callback");
    SDL_memset(stream, 0, len); //init sdl memory
    if (len == 0) {
        return -1;
    }

    MyFFMpeg *ffMpeg = (MyFFMpeg *) udata;

    LOGV("audio_callback params are :buffer_length->%d,buffer_index->%d",
         ffMpeg->audio_data_len, ffMpeg->audio_data_index);

    uint32_t data_size = ffMpeg->audio_data_len - ffMpeg->audio_data_index;
    uint8_t *data = (uint8_t *) (ffMpeg->audio_data + ffMpeg->audio_data_index);
    uint32_t data_can_read = data_size > len ? len : data_size;
    if (data_size > 0) {
        //has data for reading
        SDL_MixAudio(stream, data, data_size, SDL_MIX_MAXVOLUME);
        ffMpeg->audio_data_index += data_can_read;

        LOGV("\t data read for %d", data_can_read);
        return 0;
    } else {
        LOGE("No data for reading !");
        return -1;
    }
}

int initSDL(MyFFMpeg *ffMpeg, MySDL *sdl) {
    LOGV("initSDL");
    int ret;
    ret = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);
    if (ret) {
        LOGE("Can not init SDL ! error -> %s", SDL_GetError());
        return -1;
    }

    int width = ffMpeg->video_ctx->width;
    int height = ffMpeg->video_ctx->height;

    LOGV("The video width*height -> %d*%d", width, height);

    sdl->sdl_rect.x = 0;
    sdl->sdl_rect.y = 0;
    sdl->sdl_rect.w = width;
    sdl->sdl_rect.h = height;


    sdl->sdl_window = SDL_CreateWindow("SDL Player Demo Window", 0, 0, width, height,
                                       SDL_WINDOW_OPENGL);
    sdl->sdl_renderer = SDL_CreateRenderer(sdl->sdl_window, -1, 0); // why the params ???

    sdl->sdl_texture = SDL_CreateTexture(sdl->sdl_renderer, SDL_PIXELFORMAT_IYUV,
                                         SDL_TEXTUREACCESS_STREAMING, width, height);


    sdl->sdl_audioSpec.freq = ffMpeg->audio_out_sample_rate;
    sdl->sdl_audioSpec.format = AUDIO_S16SYS;
    sdl->sdl_audioSpec.channels = ffMpeg->audio_out_channels;
    sdl->sdl_audioSpec.silence = 0;
    sdl->sdl_audioSpec.samples = ffMpeg->audio_out_nb_sample;
    sdl->sdl_audioSpec.callback = audio_callback;
    sdl->sdl_audioSpec.userdata = ffMpeg;

    ret = SDL_OpenAudio(&sdl->sdl_audioSpec, NULL);
    if (ret < 0) {
        LOGE("Can not open audio ! code -> %s", SDL_GetError());
        return -1;
    }

    SDL_PauseAudio(0);

    return 0;
}


int start(const char *name) {
    LOGV("start -> %s", name);

    MyFFMpeg *ffMpeg = av_malloc(sizeof(MyFFMpeg));
    MySDL *sdl = av_malloc(sizeof(MySDL));

    if (initFFMpeg(ffMpeg, name)) {
        LOGE("Can not init FFMpeg !");
        return -1;
    }

    if (initSDL(ffMpeg, sdl)) {
        LOGE("Can not init SDL !");
        return -1;
    }


    LOGV("init success ... \ntry to play the video ...");

    int decode_ret = 0;
    int ret;

    while (av_read_frame(ffMpeg->format_ctx, &ffMpeg->packet) == 0) {

        if (ffMpeg->packet.stream_index == ffMpeg->videoStream) {
            ret = avcodec_decode_video2(ffMpeg->video_ctx, ffMpeg->video_frame, &decode_ret,
                                        &ffMpeg->packet);

            if (ret < 0) {
                LOGE("Error while decoding video ! msg -> %s", av_err2str(ret));
                return -1;
            }

            if (decode_ret) {
                LOGV("got picture !");
                sws_scale(ffMpeg->image_convert_ctx,
                          (const unsigned char *const *) ffMpeg->video_frame->data,
                          ffMpeg->video_frame->linesize, 0, ffMpeg->videoHeight,
                          ffMpeg->video_frame_yuv->data, ffMpeg->video_frame_yuv->linesize
                );

                SDL_UpdateYUVTexture(sdl->sdl_texture, &sdl->sdl_rect,
                                     ffMpeg->video_frame_yuv->data[0],
                                     ffMpeg->video_frame_yuv->linesize[0],
                                     ffMpeg->video_frame_yuv->data[1],
                                     ffMpeg->video_frame_yuv->linesize[1],
                                     ffMpeg->video_frame_yuv->data[2],
                                     ffMpeg->video_frame_yuv->linesize[2]
                );


                SDL_RenderClear(sdl->sdl_renderer);
                SDL_RenderCopy(sdl->sdl_renderer, sdl->sdl_texture, NULL, &sdl->sdl_rect);
                SDL_RenderPresent(sdl->sdl_renderer);

                SDL_Delay(40);
            } else {
                LOGE("Can not get picture from packet !");
            }
        } else if (ffMpeg->packet.stream_index == ffMpeg->audioStream) {
            ret = avcodec_decode_audio4(ffMpeg->audio_ctx, ffMpeg->audio_frame, &decode_ret,
                                        &ffMpeg->packet);
            if (ret < 0) {
                LOGE("Can not decode audio !");
                return -1;
            }

            if (decode_ret) {
                LOGV("audio data decoded ,ready for playing ");

                ret = swr_convert(ffMpeg->audio_convert_ctx, &ffMpeg->audio_buffer,
                                  MAX_AUDIO_FRAME_SIZE, (uint8_t **) ffMpeg->audio_frame->data,
                                  ffMpeg->audio_frame->nb_samples
                );

                if (ret < 0) {
                    LOGE("Can not convert audio to sdl format ! code -> %s", av_err2str(ret));
                    return -1;
                }

                while (ffMpeg->audio_data_index < ffMpeg->audio_data_len) {
                    SDL_Delay(1);
                    LOGV("waiting data for reading ...");
                }

                ffMpeg->audio_data = ffMpeg->audio_buffer;
                ffMpeg->audio_data_len = ffMpeg->audio_out_buffer_size;
                ffMpeg->audio_data_index = 0;

                LOGV("\t data length -> %d , data_index -> %d", ffMpeg->audio_data_len,
                     ffMpeg->audio_data_index);

            } else {
                LOGE("Getting no data from audio decoder ...");
            }
        }
        av_free_packet(&ffMpeg->packet);
    }

    SDL_CloseAudio();
    SDL_Quit();

    av_frame_free(&ffMpeg->video_frame);
    av_frame_free(&ffMpeg->video_frame_yuv);
    av_frame_free(&ffMpeg->audio_frame);

    avcodec_close(ffMpeg->video_ctx);
    avcodec_close(ffMpeg->audio_ctx);

    avformat_close_input(ffMpeg->format_ctx);

    return 0;
}


JNIEXPORT jint

JNICALL Java_volador_demoffmpeg_JNIMain_main(JNIEnv *env, jobject jo,
                                             jint argc,
                                             jobjectArray args) {


    /* This interface could expand with ABI negotiation, callbacks, etc. */


    LOGV("Java_volador_demoffmpeg_JNIMain_main");
    jobject jo1 = (*env)->GetObjectArrayElement(env, args, 0);

    jstring jstring1 = (jstring) jo1;
    LOGV("jstring -> %s", (char *) jstring1);

    return 1;
}


int main(int argc, char *argv[]) {
    LOGV("main -> %s", argv[1]);

    start(argv[1]);
}

