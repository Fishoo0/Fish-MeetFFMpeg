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

#include "../FFMpeg/include/libavcodec/avcodec.h"
#include "../FFMpeg/include/libavformat/avformat.h"
#include "../FFMpeg/include/libswscale/swscale.h"
#include "../FFMpeg/include/libavutil/frame.h"

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
    SDL_Overlay *bmp;
    int width;
    int height;
    int allocated;
    double pts;
} VideoPicture;


/**
 * The state of this player
 */
typedef struct VideoState {
    AVFormatContext *pFormatCtx;    // AVFormatContext is the format context of MediaFile ,the biggest structure in FFMpeg.

    int av_sync_type;   // The sync type of this player.Since video & audio need to sync to each other ,a sync operation is needed.

    double external_clock; // external clock base
    double external_clock_time;

    /**
     * Audio
     */
    int audioStream;
    AVStream *audio_st;     // AVStream is one component of a Media file's components
    AVCodecContext *audio_ctx;  //AVCodecContext is the codec context of an AVCodec. No need to keep AVCodec because we can get it from AVStream/AVCodecContext/etc
    PacketQueen *audioq;
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


    /**
     * Video
     */
    int videoStream;
    AVStream *video_st;
    AVCodecContext *video_ctx;
    PacketQueen *videoq;
    struct SwsContext *sws_ctx;
    //Picture Queue
    VideoPicture pictq[VIDEO_PICTURE_QUEUE_SIZE];
    int pictq_size, pictq_windex, pictq_rindex;
    SDL_mutex *pictq_mutex;
    SDL_cond *pictq_cond;

    //video clock start here
    double video_clock;
    double video_current_pts;
    int64_t video_current_pts_time;

    double frame_timer; // The real world timer
    double frame_last_pts;
    double frame_last_delay;

    /**
     * Others
     */

    SDL_Thread *parse_tid;
    SDL_Thread *video_tid;

    char filename[1024];
    int quit;

} VideoState;


SDL_Surface *screen;
SDL_mutex *screen_mutex;

VideoState *global_video_state;

#define TAG "DrangerPlayer"

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
            brek;
        } else {
            SDL_CondWait(q->cond, q->mutex);
        }
    }

    SDL_UnlockMutex(q->mutex);
}


int start(char *videoPath) {
    LOGV("start -> %s", videoPath);

    VideoState *is;
    is = av_malloc(sizeof(VideoState));

    av_register_all();

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
        LOGE("Error when init SDL !");
        return -1;
    }


}


int stop() {

}


