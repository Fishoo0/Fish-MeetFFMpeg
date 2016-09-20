
#include "volador_demoffmpeg_NDKUtils.h"


#include <stdio.h>


#include "libavcodec/avcodec.h"

#include "libavformat/avformat.h"
#include "libswscale/swscale.h"



// compatibility with newer API
#if LIBAVCODEC_VERSION_INT < AV_VERSION_INT(55,28,1)
#define av_frame_alloc avcodec_alloc_frame
#define av_frame_free avcodec_free_frame
#endif

void SaveFrame(const char *outFilePath ,AVFrame *pFrame, int width, int height, int iFrame) {
    LOGV("saveFrame iFrame -> %d",iFrame);
    FILE *pFile;
    char szFilename[1024];
    int  y;

    // Open file
    sprintf(szFilename, "%s/frame%d.ppm",outFilePath, iFrame);

    LOGV("szFilename -> %s",szFilename);

    pFile=fopen(szFilename ,"wb");

    if(pFile==NULL) {
        LOGE("file is null ,can not save frame !");
        return;
    }

    LOGV("ready to save frame ");


    // Write header
    fprintf(pFile, "P6\n%d %d\n255\n", width, height);

    // Write pixel data
    for(y=0; y<height; y++)
        fwrite(pFrame->data[0]+y*pFrame->linesize[0], 1, width*3, pFile);

    // Close file
    fclose(pFile);
}


JNIEXPORT int JNICALL Java_volador_demoffmpeg_NDKUtils_studyLesson1(JNIEnv *env, jobject obj, jstring  jsFilePath ,jstring jsOutPicDir) {


// Initalizing these to NULL prevents segfaults!
    AVFormatContext   *pFormatCtx = NULL;

    int               i, videoStream;

    AVCodecContext    *pCodecCtxOrig = NULL;
    AVCodecContext    *pCodecCtx = NULL;


    AVCodec           *pCodec = NULL;


    AVFrame           *pFrame = NULL;
    AVFrame           *pFrameRGB = NULL;

    AVPacket          packet;
    int               frameFinished;
    int               numBytes;
    uint8_t           *buffer = NULL;

    struct SwsContext *sws_ctx = NULL;

    const char *filePath = (*env)->GetStringUTFChars(env, jsFilePath, 0);
    const char *outPicDir = (*env)->GetStringUTFChars(env,jsOutPicDir,0);

    LOGV("videoFile -> %s",filePath);
    LOGV("outDir -> %s",outPicDir);


    LOGV("register all formats and codecs ...");
    // Register all formats and codecs
    av_register_all();
    LOGV("success");


    LOGV("open video file ...");
    // Open video file      打开文件
    if(avformat_open_input(&pFormatCtx, filePath, NULL, NULL)!=0)
        return -1; // Couldn't open file
    LOGV("success");

    LOGV("Retrieve stream information ...");
    // Retrieve stream information
    if(avformat_find_stream_info(pFormatCtx, NULL)<0)
        return -1; // Couldn't find stream information

    // Dump information about file onto standard error
    av_dump_format(pFormatCtx, 0, filePath, 0);
    LOGV("success");


    LOGV("Find the first video stream ...");
    // Find the first video stream
    videoStream=-1;
    for(i=0; i<pFormatCtx->nb_streams; i++)
        if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO) {
            videoStream=i;
            break;
        }
    if(videoStream==-1)
        return -1; // Didn't find a video stream
    LOGV("first video stream is -> %d",videoStream);





    // Get a pointer to the codec context for the video stream
    pCodecCtxOrig=pFormatCtx->streams[videoStream]->codec;

    LOGV("Find the decoder for the video stream ...");
    // Find the decoder for the video stream
    pCodec=avcodec_find_decoder(pCodecCtxOrig->codec_id);
    if(pCodec==NULL) {
        fprintf(stderr, "Unsupported codec!\n");
        return -1; // Codec not found
    }
    LOGV("success");


    // Copy context
    pCodecCtx = avcodec_alloc_context3(pCodec);
    if(avcodec_copy_context(pCodecCtx, pCodecCtxOrig) != 0) {
        fprintf(stderr, "Couldn't copy codec context");
        return -1; // Error copying codec context
    }


    LOGV("Open codec ...");
    // Open codec 打开解码器
    if(avcodec_open2(pCodecCtx, pCodec, NULL)<0)
        return -1; // Could not open codec
    LOGV("success");

    // Allocate video frame 原格式视频帧
    pFrame=av_frame_alloc();


    // Allocate an AVFrame structure RGB        RGB格式视频帧
    pFrameRGB=av_frame_alloc();
    if(pFrameRGB==NULL)
        return -1;


    // Determine required buffer size and allocate buffer   缓存
    numBytes=avpicture_get_size(PIX_FMT_RGB24, pCodecCtx->width,
                                pCodecCtx->height);
    buffer=(uint8_t *)av_malloc(numBytes*sizeof(uint8_t));


    // Assign appropriate parts of buffer to image planes in pFrameRGB
    // Note that pFrameRGB is an AVFrame, but AVFrame is a superset
    // of AVPicture
    avpicture_fill((AVPicture *)pFrameRGB, buffer, PIX_FMT_RGB24,
                   pCodecCtx->width, pCodecCtx->height);





    // initialize SWS context for software scaling
    sws_ctx = sws_getContext(pCodecCtx->width,
                             pCodecCtx->height,
                             pCodecCtx->pix_fmt,
                             pCodecCtx->width,
                             pCodecCtx->height,
                             PIX_FMT_RGB24,
                             SWS_BILINEAR,
                             NULL,
                             NULL,
                             NULL
    );

    // Read frames and save first five frames to disk
    i=0;
    while(av_read_frame(pFormatCtx, &packet)>=0) {
        // Is this a packet from the video stream?
        if(packet.stream_index==videoStream) {

            // Decode video frame
            avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);

            // Did we get a video frame?
            if(frameFinished) {
                // Convert the image from its native format to RGB
                sws_scale(sws_ctx, (uint8_t const * const *)pFrame->data,
                          pFrame->linesize, 0, pCodecCtx->height,
                          pFrameRGB->data, pFrameRGB->linesize);

                // Save the frame to disk
                if(++i<=5)
                    SaveFrame(outPicDir,pFrameRGB, pCodecCtx->width, pCodecCtx->height,
                              i);
            }
        }

        // Free the packet that was allocated by av_read_frame
        av_free_packet(&packet);
    }

    // Free the RGB image
    av_free(buffer);
    av_frame_free(&pFrameRGB);

    // Free the YUV frame
    av_frame_free(&pFrame);

    // Close the codecs
    avcodec_close(pCodecCtx);
    avcodec_close(pCodecCtxOrig);

    // Close the video file
    avformat_close_input(&pFormatCtx);


    (*env)->ReleaseStringUTFChars(env, jsFilePath, filePath);
    (*env)->ReleaseStringUTFChars(env, jsOutPicDir, outPicDir);

    return 2016;


}







