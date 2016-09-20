#include "volador_demoffmpeg_NDKUtils.h"

#include "my_log.h"

#include "libavutil/opt.h"
#include "libavutil/frame.h"
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"

#include <stdio.h>
#include <stdint.h>


#include "../FFMpeg/include/libavutil/opt.h"
#include "../FFMpeg/include/libavcodec/avcodec.h"
#include "../FFMpeg/include/libavformat/avformat.h"
#include "../FFMpeg/include/libavutil/frame.h"


int flush_encoder(AVFormatContext *fmt_ctx, unsigned int stream_index) {
    int ret;
    int got_frame;
    AVPacket enc_pkt;
    if (!(fmt_ctx->streams[stream_index]->codec->codec->capabilities & CODEC_CAP_DELAY))
        return 0;
    while (1) {
        enc_pkt.data = NULL;
        enc_pkt.size = 0;
        av_init_packet(&enc_pkt);
        ret = avcodec_encode_video2(fmt_ctx->streams[stream_index]->codec, &enc_pkt,
                                    NULL, &got_frame);
        av_frame_free(NULL);
        if (ret < 0)
            break;
        if (!got_frame) {
            ret = 0;
            break;
        }
        printf("Flush Encoder: Succeed to encode 1 frame!\tsize:%5d\n", enc_pkt.size);
        /* mux encoded frame */
        ret = av_write_frame(fmt_ctx, &enc_pkt);
        if (ret < 0)
            break;
    }
    return ret;
}


int doEncode(const char *input, const char *output) {

    LOGV("doEncode input %s ,output %s", input, output);


    AVFormatContext *pFormatCtx;
    AVOutputFormat *outputFormat;

    AVStream *video_stream;

    AVCodecContext *pCodecCtx;
    AVCodec *pCodec;

    AVPacket pkt;

    uint8_t *picture_buf;

    AVFrame *pFrame;

    int ret = 2;

    int picture_size;
    int y_size;

    int framecnt = 0;

    int in_w = 480;
    int in_h = 272;

    int framenum = 100;

    FILE *in_file = fopen(input, "rb");

    av_register_all();
    LOGV("register ffmpeg ...");

//    avcodec_register_all();

    pFormatCtx = avformat_alloc_context();
    LOGV("create AVFormatContext ...");

    outputFormat = av_guess_format(NULL, output, NULL);
    pFormatCtx->oformat = outputFormat;
    LOGV("getting outputFormat -> %s", outputFormat->long_name);


    LOGV("\n");

    LOGV("open output file ...");
    ret = avio_open(&pFormatCtx->pb, output, AVIO_FLAG_READ_WRITE);
    if (ret < 0) {
        LOGE("can not open output file -> %d", ret);
        return -1;
    }


    video_stream = avformat_new_stream(pFormatCtx, 0);
    LOGV("creating AVStream ...");

    if (video_stream == NULL) {
        LOGE("can not create AVStream !!!");
        return -1;
    }


    LOGV("setting key params to AVCodecContext ...");
    //Param that must set
    pCodecCtx = video_stream->codec;
    //pCodecCtx->codec_id =AV_CODEC_ID_HEVC;
    pCodecCtx->codec_id = outputFormat->video_codec;
    pCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
    pCodecCtx->pix_fmt = AV_PIX_FMT_YUV420P;
    pCodecCtx->width = in_w;
    pCodecCtx->height = in_h;

    pCodecCtx->bit_rate = 400000;

    pCodecCtx->gop_size = 250;

    pCodecCtx->time_base.num = 1;
    pCodecCtx->time_base.den = 25;

    //H264
    //pCodecCtx->me_range = 16;
    //pCodecCtx->max_qdiff = 4;
    //pCodecCtx->qcompress = 0.6;
    pCodecCtx->qmin = 10;
    pCodecCtx->qmax = 51;

    //Optional Param
    pCodecCtx->max_b_frames = 3;

    // Set Option
    AVDictionary *param = 0;
    LOGV("setting AVDictionary");
    //H.264
    if (pCodecCtx->codec_id == AV_CODEC_ID_H264) {
        av_dict_set(&param, "preset", "slow", 0);
        av_dict_set(&param, "tune", "zerolatency", 0);
        //av_dict_set(&param, "profile", "main", 0);
    }
    //H.265
    if (pCodecCtx->codec_id == AV_CODEC_ID_H265) {
        av_dict_set(&param, "preset", "ultrafast", 0);
        av_dict_set(&param, "tune", "zero-latency", 0);
    }


    LOGV("show information , see ?");
    printf("what the helel??what the helel??what the helel??what the helel??what the helel??what the helel??what the helel??what the helel??");
    av_dump_format(pFormatCtx, 0, output, 1);


    LOGV("finding encoder ...");
    pCodec = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (!pCodec) {
        LOGE("can not find encoder !!! ");
        return -1;
    }


    LOGV("opening encoder ...");
    if (avcodec_open2(pCodecCtx, pCodec, &param) < 0) {
        LOGE("can not open encoder !!!");
        return -1;
    }


    LOGV("alloc AVFrame ...");
    pFrame = av_frame_alloc();


    picture_size = avpicture_get_size(pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height);
    LOGV("creating buffer ,pictureSize -> %d", picture_size);

    picture_buf = (uint8_t *) av_malloc(picture_size);

    avpicture_fill((AVPicture *) pFrame, picture_buf, pCodecCtx->pix_fmt, pCodecCtx->width,
                   pCodecCtx->height);

    LOGV("write header ...");
    avformat_write_header(pFormatCtx, NULL);

    LOGV("creating AVPacket ...");
    av_new_packet(&pkt, picture_size);

    y_size = pCodecCtx->width * pCodecCtx->height;


    LOGV("Try encoding ...");
    int i;
    for (i = 0; i < framenum; i++) {

        LOGV("try encode %d frame ", i);

        if (fread(picture_buf, 1, y_size * 3 / 2, in_file) <= 0) {
            LOGE("Failed to read raw data ! \n");
            return -1;
        } else if (feof(in_file)) {
            LOGW("feof(in_file) ,break");
            break;
        }


        pFrame->data[0] = picture_buf;              //Y
        pFrame->data[1] = picture_buf + y_size;     //U
        pFrame->data[2] = picture_buf + y_size * 5 / 4; //V

        //what the fuck !!!
        pFrame->pts = i * (video_stream->time_base.den) / ((video_stream->time_base.num) * 25);


        pFrame->pts = i;

        int got_picture = 0;

        ret = avcodec_encode_video2(pCodecCtx, &pkt, pFrame, &got_picture);
        if (ret < 0) {
            LOGE("Failed to encode !!! ");
            return -1;
        }

        if (got_picture == 1) {
            LOGV("Succeed to encode frame: %5d\tsize:%5d\t\n", framecnt, pkt.size);
            framecnt++;
            pkt.stream_index = video_stream->index;
            ret = av_write_frame(pFormatCtx, &pkt);
            av_free_packet(&pkt);
        }

    }

    LOGV("Flush encoder ...");
    ret = flush_encoder(pFormatCtx, 0);
    if (ret < 0) {
        LOGE("Fulshing encoder failed !!!");
        return -1;
    }

    LOGV("write trailer ...");
    av_write_trailer(pFormatCtx);

    LOGV("cleaning ...");
    if (video_stream) {
        avcodec_close(pCodecCtx);
        av_free(pFrame);
        av_free(picture_buf);
    }

    avio_close(pFormatCtx->pb);
    avformat_free_context(pFormatCtx);

    fclose(in_file);

    return 0;

}


JNIEXPORT jint JNICALL Java_volador_demoffmpeg_NDKUtils_testEncodeYUVToH264
        (JNIEnv *env, jobject jo, jstring jsFilePath, jstring jsOutPicDir) {
    const char *input = (*env)->GetStringUTFChars(env, jsFilePath, 0);
    const char *output = (*env)->GetStringUTFChars(env, jsOutPicDir, 0);
    return doEncode(input, output);
}