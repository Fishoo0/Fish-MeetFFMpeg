// tutorial02.c
// A pedagogical video player that will stream through every video frame as fast as it can.
//
// Code based on FFplay, Copyright (c) 2003 Fabrice Bellard,
// and a tutorial by Martin Bohme (boehme@inb.uni-luebeckREMOVETHIS.de)
// Tested on Gentoo, CVS version 5/01/07 compiled with GCC 4.1.1
// With updates from https://github.com/chelyaev/ffmpeg-tutorial
// Updates tested on:
// LAVC 54.59.100, LAVF 54.29.104, LSWS 2.1.101, SDL 1.2.15
// on GCC 4.7.2 in Debian February 2015
//
// Use
//
// gcc -o tutorial02 tutorial02.c -lavformat -lavcodec -lswscale -lz -lm `sdl-config --cflags --libs`
// to build (assuming libavformat and libavcodec are correctly installed,
// and assuming you have sdl-config. Please refer to SDL docs for your installation.)
//
// Run using
// tutorial02 myvideofile.mpg
//
// to play the video stream on your screen.

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>

#include "SDL.h"
#include "SDL_thread.h"

#include "volador_demoffmpeg_NDKUtils.h"

#ifdef __MINGW32__
#undef main /* Prevents SDL from overriding main() */
#endif

#include <stdio.h>

// compatibility with newer API
#if LIBAVCODEC_VERSION_INT < AV_VERSION_INT(55, 28, 1)
#define av_frame_alloc avcodec_alloc_frame
#define av_frame_free avcodec_free_frame
#endif


///*
// * Class:     volador_demoffmpeg_NDKUtils
// * Method:    studyLesson2
// * Signature: (Ljava/lang/String;)I
// */
//JNIEXPORT jint JNICALL Java_volador_demoffmpeg_NDKUtils_studyLesson2(JNIEnv *env, jobject jobj,
//                                                                     jstring videoPathJS) {
//    LOGV("studyLesson2");
//
//    AVFormatContext *pFormatCtx = NULL;
//    int i, videoStream;
//    AVCodecContext *pCodecCtxOrig = NULL;
//    AVCodecContext *pCodecCtx = NULL;
//    AVCodec *pCodec = NULL;
//    AVFrame *pFrame = NULL;
//    AVPacket packet;
//    int frameFinished;
//    float aspect_ratio;
//    struct SwsContext *sws_ctx = NULL;
//
//    SDL_Overlay *bmp;
//    SDL_Surface *screen;
//    SDL_Rect rect;
//    SDL_Event event;
//
//
//
//    const char *videoPath = (*env)->GetStringUTFChars(env, videoPathJS, 0);
//    LOGV("\t VideoPath -> %s", videoPath);
//
//    // Register all formats and codecs
//    av_register_all();
//
//    LOGV("SDL_init ...");
//    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
//        LOGE("SDL_Init failed !!");
//        exit(1);
//    }
//
//    LOGV("Open video ...");
//    // Open video file
//    if (avformat_open_input(&pFormatCtx, videoPath, NULL, NULL) != 0) {
//        return -1; // Couldn't open file
//    }
//
//    LOGV("avformat_find_stream_info ...");
//    // Retrieve stream information
//    if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
//        return -1; // Couldn't find stream information
//    }
//
//    LOGV("\t av_dump_format ...");
//    // Dump information about file onto standard error
//    av_dump_format(pFormatCtx, 0, videoPath, 0);
//
//    LOGV("\t Find the first video stream ...");
//    // Find the first video stream
//    videoStream = -1;
//    for (i = 0; i < pFormatCtx->nb_streams; i++)
//        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
//            videoStream = i;
//            break;
//        }
//    if (videoStream == -1)
//        return -1; // Didn't find a video stream
//
//    LOGV("\t Get a pointer to the codec context for the video stream ...");
//    // Get a pointer to the codec context for the video stream
//    pCodecCtxOrig = pFormatCtx->streams[videoStream]->codec;
//
//    LOGV("\t Find the decoder for the video stream ...");
//    // Find the decoder for the video stream
//    pCodec = avcodec_find_decoder(pCodecCtxOrig->codec_id);
//    if (pCodec == NULL) {
//        fprintf(stderr, "Unsupported codec!\n");
//        return -1; // Codec not found
//    }
//
//    LOGV("\t Copy context...");
//    // Copy context
//    pCodecCtx = avcodec_alloc_context3(pCodec);
//    if (avcodec_copy_context(pCodecCtx, pCodecCtxOrig) != 0) {
//        fprintf(stderr, "Couldn't copy codec context");
//        return -1; // Error copying codec context
//    }
//
//    LOGV("\t Open codec ...");
//    // Open codec
//    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0)
//        return -1; // Could not open codec
//
//    LOGV("\t Allocate video frame ...");
//    // Allocate video frame
//    pFrame = av_frame_alloc();
//
//
//    LOGV("\t Make a screen to put our video ...");
//    // Make a screen to put our video
//#ifndef __DARWIN__
//    LOGD("NOT __DARWIN__");
//    screen = SDL_SetVideoMode(pCodecCtx->width, pCodecCtx->height, 0, 0);
//#else
//    LOGD("__DARWIN__");
//    screen = SDL_SetVideoMode(pCodecCtx->width, pCodecCtx->height, 24, 0);
//#endif
//    if (!screen) {
//        fprintf(stderr, "SDL: could not set video mode - exiting\n");
//        exit(1);
//    }
//
//    LOGV("Allocate a place to put our YUV image on that screen ...");
//    // Allocate a place to put our YUV image on that screen
//    bmp = SDL_CreateYUVOverlay(pCodecCtx->width,
//                               pCodecCtx->height,
//                               SDL_YV12_OVERLAY,
//                               screen);
//
//    LOGV("\t initialize SWS context for software scaling ...");
//    // initialize SWS context for software scaling
//    sws_ctx = sws_getContext(pCodecCtx->width,
//                             pCodecCtx->height,
//                             pCodecCtx->pix_fmt,
//                             pCodecCtx->width,
//                             pCodecCtx->height,
//                             PIX_FMT_YUV420P,
//                             SWS_BILINEAR,
//                             NULL,
//                             NULL,
//                             NULL
//    );
//
//
//    LOGV("\t Read frames and output to the screen ...");
//    // Read frames and save first five frames to disk
//    i = 0;
//    while (av_read_frame(pFormatCtx, &packet) >= 0) {
//        // Is this a packet from the video stream?
//        if (packet.stream_index == videoStream) {
//            // Decode video frame
//            avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);
//
//            // Did we get a video frame?
//            if (frameFinished) {
//                SDL_LockYUVOverlay(bmp);
//
//                AVPicture pict;
//                pict.data[0] = bmp->pixels[0];
//                pict.data[1] = bmp->pixels[2];
//                pict.data[2] = bmp->pixels[1];
//
//                pict.linesize[0] = bmp->pitches[0];
//                pict.linesize[1] = bmp->pitches[2];
//                pict.linesize[2] = bmp->pitches[1];
//
//                // Convert the image into YUV format that SDL uses
//                sws_scale(sws_ctx, (uint8_t const *const *) pFrame->data,
//                          pFrame->linesize, 0, pCodecCtx->height,
//                          pict.data, pict.linesize);
//
//                SDL_UnlockYUVOverlay(bmp);
//
//                rect.x = 0;
//                rect.y = 0;
//                rect.w = pCodecCtx->width;
//                rect.h = pCodecCtx->height;
//
//                LOGV("display one >>>> ");
//
//                SDL_DisplayYUVOverlay(bmp, &rect);
//
//            }
//        }
//
//        LOGV("\t Free the packet that was allocated by av_read_frame ...");
//        // Free the packet that was allocated by av_read_frame
//        av_free_packet(&packet);
//
//        LOGV("\t PollEvent ...");
//        SDL_PollEvent(&event);
//        switch (event.type) {
//            case SDL_QUIT:
//                SDL_Quit();
//                exit(0);
//                break;
//            default:
//                break;
//        }
//
//    }
//
//    LOGV("\t Free the YUV frame ...");
//    // Free the YUV frame
//    av_frame_free(&pFrame);
//
//    LOGV("\t Close the codec ...");
//    // Close the codec
//    avcodec_close(pCodecCtx);
//    avcodec_close(pCodecCtxOrig);
//
//    LOGV("\t Close the video file ...");
//    // Close the video file
//    avformat_close_input(&pFormatCtx);
//
//    return 2016;
//}

///*
// * Class:     volador_demoffmpeg_NDKUtils
// * Method:    studyLesson2
// * Signature: (Ljava/lang/String;)I
// */
//JNIEXPORT jint JNICALL Java_volador_demoffmpeg_NDKUtils_studyLesson2(JNIEnv *env, jobject jobj,
//                                                                     jstring videoPathJS) {
//
//
//
//    const char *videoPath = (*env)->GetStringUTFChars(env, videoPathJS, 0);
//    LOGV("\t VideoPath -> %s", videoPath);
//
//    AVFormatContext *pFormatCtx = NULL;
//    int videoStream;
//    unsigned i;
//    AVCodecContext *pCodecCtxOrig = NULL;
//    AVCodecContext *pCodecCtx = NULL;
//    AVCodec *pCodec = NULL;
//    AVFrame *pFrame = NULL;
//    AVPacket packet;
//    int frameFinished;
//    struct SwsContext *sws_ctx = NULL;
//
//    SDL_Event event;
//    SDL_Window *screen;
//    SDL_Renderer *renderer;
//    SDL_Texture *texture;
//    Uint8 *yPlane, *uPlane, *vPlane;
//    size_t yPlaneSz, uvPlaneSz;
//    int uvPitch;
//
//    // Register all formats and codecs
//    av_register_all();
//
//    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
//        fprintf(stderr, "Could not initialize SDL - %s\n", SDL_GetError());
//        exit(1);
//    }
//
//    // Open video file
//    if (avformat_open_input(&pFormatCtx, videoPath, NULL, NULL) != 0)
//        return -1; // Couldn't open file
//
//    // Retrieve stream information
//    if (avformat_find_stream_info(pFormatCtx, NULL) < 0)
//        return -1; // Couldn't find stream information
//
//    // Dump information about file onto standard error
//    av_dump_format(pFormatCtx, 0, videoPath, 0);
//
//    // Find the first video stream
//    videoStream = -1;
//    for (i = 0; i < pFormatCtx->nb_streams; i++)
//        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
//            videoStream = i;
//            break;
//        }
//    if (videoStream == -1)
//        return -1; // Didn't find a video stream
//
//    // Get a pointer to the codec context for the video stream
//    pCodecCtxOrig = pFormatCtx->streams[videoStream]->codec;
//    // Find the decoder for the video stream
//    pCodec = avcodec_find_decoder(pCodecCtxOrig->codec_id);
//    if (pCodec == NULL) {
//        fprintf(stderr, "Unsupported codec!\n");
//        return -1; // Codec not found
//    }
//
//    // Copy context
//    pCodecCtx = avcodec_alloc_context3(pCodec);
//    if (avcodec_copy_context(pCodecCtx, pCodecCtxOrig) != 0) {
//        fprintf(stderr, "Couldn't copy codec context");
//        return -1; // Error copying codec context
//    }
//
//    // Open codec
//    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0)
//        return -1; // Could not open codec
//
//    // Allocate video frame
//    pFrame = av_frame_alloc();
//
//    // Make a screen to put our video
//    screen = SDL_CreateWindow(
//            "FFmpeg Tutorial",
//            SDL_WINDOWPOS_UNDEFINED,
//            SDL_WINDOWPOS_UNDEFINED,
//            pCodecCtx->width,
//            pCodecCtx->height,
//            0
//    );
//
//    if (!screen) {
//        fprintf(stderr, "SDL: could not create window - exiting\n");
//        exit(1);
//    }
//
//    renderer = SDL_CreateRenderer(screen, -1, 0);
//    if (!renderer) {
//        fprintf(stderr, "SDL: could not create renderer - exiting\n");
//        exit(1);
//    }
//
//    // Allocate a place to put our YUV image on that screen
//    texture = SDL_CreateTexture(
//            renderer,
//            SDL_PIXELFORMAT_YV12,
//            SDL_TEXTUREACCESS_STREAMING,
//            pCodecCtx->width,
//            pCodecCtx->height
//    );
//    if (!texture) {
//        fprintf(stderr, "SDL: could not create texture - exiting\n");
//        exit(1);
//    }
//
//    // initialize SWS context for software scaling
//    sws_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height,
//                             pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height,
//                             PIX_FMT_YUV420P,
//                             SWS_BILINEAR,
//                             NULL,
//                             NULL,
//                             NULL);
//
//    // set up YV12 pixel array (12 bits per pixel)
//    yPlaneSz = pCodecCtx->width * pCodecCtx->height;
//    uvPlaneSz = pCodecCtx->width * pCodecCtx->height / 4;
//    yPlane = (Uint8*)malloc(yPlaneSz);
//    uPlane = (Uint8*)malloc(uvPlaneSz);
//    vPlane = (Uint8*)malloc(uvPlaneSz);
//    if (!yPlane || !uPlane || !vPlane) {
//        fprintf(stderr, "Could not allocate pixel buffers - exiting\n");
//        exit(1);
//    }
//
//    uvPitch = pCodecCtx->width / 2;
//    while (av_read_frame(pFormatCtx, &packet) >= 0) {
//        // Is this a packet from the video stream?
//        if (packet.stream_index == videoStream) {
//            // Decode video frame
//            avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);
//
//            // Did we get a video frame?
//            if (frameFinished) {
//                AVPicture pict;
//                pict.data[0] = yPlane;
//                pict.data[1] = uPlane;
//                pict.data[2] = vPlane;
//                pict.linesize[0] = pCodecCtx->width;
//                pict.linesize[1] = uvPitch;
//                pict.linesize[2] = uvPitch;
//
//                // Convert the image into YUV format that SDL uses
//                sws_scale(sws_ctx, (uint8_t const * const *) pFrame->data,
//                          pFrame->linesize, 0, pCodecCtx->height, pict.data,
//                          pict.linesize);
//
//                SDL_UpdateYUVTexture(
//                        texture,
//                        NULL,
//                        yPlane,
//                        pCodecCtx->width,
//                        uPlane,
//                        uvPitch,
//                        vPlane,
//                        uvPitch
//                );
//
//                SDL_RenderClear(renderer);
//                SDL_RenderCopy(renderer, texture, NULL, NULL);
//                SDL_RenderPresent(renderer);
//
//            }
//        }
//
//        // Free the packet that was allocated by av_read_frame
//        av_free_packet(&packet);
//        SDL_PollEvent(&event);
//        switch (event.type) {
//            case SDL_QUIT:
//                SDL_DestroyTexture(texture);
//                SDL_DestroyRenderer(renderer);
//                SDL_DestroyWindow(screen);
//                SDL_Quit();
//                exit(0);
//                break;
//            default:
//                break;
//        }
//
//    }
//
//    // Free the YUV frame
//    av_frame_free(&pFrame);
//    free(yPlane);
//    free(uPlane);
//    free(vPlane);
//
//    // Close the codec
//    avcodec_close(pCodecCtx);
//    avcodec_close(pCodecCtxOrig);
//
//    // Close the video file
//    avformat_close_input(&pFormatCtx);
//
//    return 100;
//}


/*
 * Class:     volador_demoffmpeg_NDKUtils
 * Method:    studyLesson2
 * Signature: (Ljava/lang/String;)I
 */
int main(int argc, char *argv[]) {

    const char *videoPath = "/storage/emulated/0/Movies/test.MOV";

    LOGV("\t VideoPath -> %s", videoPath);

    AVFormatContext *pFormatCtx = NULL;
    int videoStream;
    unsigned i;
    AVCodecContext *pCodecCtxOrig = NULL;
    AVCodecContext *pCodecCtx = NULL;
    AVCodec *pCodec = NULL;
    AVFrame *pFrame = NULL;
    AVPacket packet;
    int frameFinished;
    struct SwsContext *sws_ctx = NULL;

    SDL_Event event;
    SDL_Window *screen;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    Uint8 *yPlane, *uPlane, *vPlane;
    size_t yPlaneSz, uvPlaneSz;
    int uvPitch;

    // Register all formats and codecs
    av_register_all();

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
        fprintf(stderr, "Could not initialize SDL - %s\n", SDL_GetError());
        exit(1);
    }

    // Open video file
    if (avformat_open_input(&pFormatCtx, videoPath, NULL, NULL) != 0)
        return -1; // Couldn't open file

    // Retrieve stream information
    if (avformat_find_stream_info(pFormatCtx, NULL) < 0)
        return -1; // Couldn't find stream information

    // Dump information about file onto standard error
    av_dump_format(pFormatCtx, 0, videoPath, 0);

    // Find the first video stream
    videoStream = -1;
    for (i = 0; i < pFormatCtx->nb_streams; i++)
        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStream = i;
            break;
        }
    if (videoStream == -1)
        return -1; // Didn't find a video stream

    // Get a pointer to the codec context for the video stream
    pCodecCtxOrig = pFormatCtx->streams[videoStream]->codec;
    // Find the decoder for the video stream
    pCodec = avcodec_find_decoder(pCodecCtxOrig->codec_id);
    if (pCodec == NULL) {
        fprintf(stderr, "Unsupported codec!\n");
        return -1; // Codec not found
    }

    // Copy context
    pCodecCtx = avcodec_alloc_context3(pCodec);
    if (avcodec_copy_context(pCodecCtx, pCodecCtxOrig) != 0) {
        fprintf(stderr, "Couldn't copy codec context");
        return -1; // Error copying codec context
    }

    // Open codec
    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0)
        return -1; // Could not open codec

    // Allocate video frame
    pFrame = av_frame_alloc();

    // Make a screen to put our video
    screen = SDL_CreateWindow(
            "FFmpeg Tutorial",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            pCodecCtx->width,
            pCodecCtx->height,
            0
    );

    if (!screen) {
        fprintf(stderr, "SDL: could not create window - exiting\n");
        exit(1);
    }

    renderer = SDL_CreateRenderer(screen, -1, 0);
    if (!renderer) {
        fprintf(stderr, "SDL: could not create renderer - exiting\n");
        exit(1);
    }

    // Allocate a place to put our YUV image on that screen
    texture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_YV12,
            SDL_TEXTUREACCESS_STREAMING,
            pCodecCtx->width,
            pCodecCtx->height
    );
    if (!texture) {
        fprintf(stderr, "SDL: could not create texture - exiting\n");
        exit(1);
    }

    // initialize SWS context for software scaling
    sws_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height,
                             pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height,
                             PIX_FMT_YUV420P,
                             SWS_BILINEAR,
                             NULL,
                             NULL,
                             NULL);

    // set up YV12 pixel array (12 bits per pixel)
    yPlaneSz = pCodecCtx->width * pCodecCtx->height;
    uvPlaneSz = pCodecCtx->width * pCodecCtx->height / 4;
    yPlane = (Uint8*)malloc(yPlaneSz);
    uPlane = (Uint8*)malloc(uvPlaneSz);
    vPlane = (Uint8*)malloc(uvPlaneSz);
    if (!yPlane || !uPlane || !vPlane) {
        fprintf(stderr, "Could not allocate pixel buffers - exiting\n");
        exit(1);
    }

    uvPitch = pCodecCtx->width / 2;
    while (av_read_frame(pFormatCtx, &packet) >= 0) {
        // Is this a packet from the video stream?
        if (packet.stream_index == videoStream) {
            // Decode video frame
            avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);

            // Did we get a video frame?
            if (frameFinished) {
                AVPicture pict;
                pict.data[0] = yPlane;
                pict.data[1] = uPlane;
                pict.data[2] = vPlane;
                pict.linesize[0] = pCodecCtx->width;
                pict.linesize[1] = uvPitch;
                pict.linesize[2] = uvPitch;

                // Convert the image into YUV format that SDL uses
                sws_scale(sws_ctx, (uint8_t const * const *) pFrame->data,
                          pFrame->linesize, 0, pCodecCtx->height, pict.data,
                          pict.linesize);

                SDL_UpdateYUVTexture(
                        texture,
                        NULL,
                        yPlane,
                        pCodecCtx->width,
                        uPlane,
                        uvPitch,
                        vPlane,
                        uvPitch
                );

                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, texture, NULL, NULL);
                SDL_RenderPresent(renderer);

            }
        }

        // Free the packet that was allocated by av_read_frame
        av_free_packet(&packet);
        SDL_PollEvent(&event);
        switch (event.type) {
            case SDL_QUIT:
                SDL_DestroyTexture(texture);
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(screen);
                SDL_Quit();
                exit(0);
                break;
            default:
                break;
        }

    }

    // Free the YUV frame
    av_frame_free(&pFrame);
    free(yPlane);
    free(uPlane);
    free(vPlane);

    // Close the codec
    avcodec_close(pCodecCtx);
    avcodec_close(pCodecCtxOrig);

    // Close the video file
    avformat_close_input(&pFormatCtx);

    return 100;
}