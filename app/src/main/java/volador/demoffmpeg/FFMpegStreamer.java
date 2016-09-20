package volador.demoffmpeg;

import android.media.AudioFormat;
import android.media.MediaFormat;
import android.media.MediaRecorder;

import java.nio.ShortBuffer;

/**
 * Created by volador on 16/4/28.
 */
public class FFMpegStreamer {

    public static final int RAW_DATA_VIDEO = 1;
    public static final int RAW_DATA_AUDIO = 2;


    static {
        System.loadLibrary("demoffmpeg");
    }

    public native int init(String outputUrl, int picWidth, int picHeight, int frameRate, int audioSample, int audioChannelLayout);

    public native int prepair();

    public native int encodeVideo(byte[] data);

    public native int encodeAudio(short[] data ,int sampleCount);

    public native int pause();

    public native int release();

    public native int setLogPath(String path);


}
