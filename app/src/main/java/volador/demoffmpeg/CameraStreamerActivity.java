package volador.demoffmpeg;

import android.app.Activity;
import android.hardware.Camera;
import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder;
import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Looper;
import android.os.Message;
import android.util.Log;
import android.view.Surface;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;

import java.nio.ByteBuffer;
import java.nio.ShortBuffer;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

public class CameraStreamerActivity extends Activity implements View.OnClickListener {


    static final String TAG = CameraStreamerActivity.class.getSimpleName();

    public static final int TARGET_PREVIEW_WIDTH = 480;
    public static final int TARGET_PREVIEW_HEIGHT = 800;
    public static final int TARGET_FRAME_RATE = 15;
    public static final int TARGET_AUDIO_SAMPLE = 44100;

    public static final int TARGET_AUDIO_CHANNEL_LAYOUT = AudioFormat.CHANNEL_IN_MONO;


    private int mImageWidth = TARGET_PREVIEW_WIDTH;
    private int mImageHeight = TARGET_PREVIEW_HEIGHT;
    private int mFrameRate = TARGET_FRAME_RATE;

    private int mAudioSample = TARGET_AUDIO_SAMPLE;

    private int mAudioChannelLayout = TARGET_AUDIO_CHANNEL_LAYOUT;

    private CameraSurfaceView mCameraTextureView;

    private Camera mCamera;


    private FFMpegStreamer mStreamer;


    private static String RTMP_URL = "rtmp://192.168.1.188:1935/fishdemo/fish";

    private VideoRecorderManager mVideoRecorderManager;
    private AudioRecorderManager mAudioRecorderManager;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_camera_streamer);


        mCameraTextureView = (CameraSurfaceView) findViewById(R.id.camera_texture_view);

        mCamera = Camera.open();
//        mCamera.setPreviewCallback(this);

        setCameraDisplayOrientation(this, 0, mCamera);


        ViewGroup.LayoutParams params = mCameraTextureView.getLayoutParams();
        params.width = mImageHeight * 2;
        params.height = mImageWidth * 2;
        mCameraTextureView.setLayoutParams(params);

        mCameraTextureView.setCamera(mCamera);

        setCameraPreviewSize();

        mStreamer = new FFMpegStreamer();
        mStreamer.setLogPath(FileUtils.getMoveFilePath("av_log" + System.currentTimeMillis()+".txt"));
                mStreamer.init(RTMP_URL, mImageWidth, mImageHeight, mFrameRate, mAudioSample, mAudioChannelLayout);
        mStreamer.prepair();

        mVideoRecorderManager = new VideoRecorderManager(mCamera, mStreamer);
        mAudioRecorderManager = new AudioRecorderManager(mStreamer, mAudioSample);

        mVideoRecorderManager.init();
        mAudioRecorderManager.init();

    }


    private void setCameraPreviewSize() {

        Log.v(TAG, "setCameraPreviewSize");

        Camera.Parameters camParams = mCamera.getParameters();
        List<Camera.Size> sizes = camParams.getSupportedPreviewSizes();

        List<Integer> frameRates = camParams.getSupportedPreviewFrameRates();

        // Sort the list in ascending order
        Collections.sort(sizes, new Comparator<Camera.Size>() {

            public int compare(final Camera.Size a, final Camera.Size b) {
                return a.width * a.height - b.width * b.height;
            }
        });

        Log.v(TAG, "\t surported previewSize is : ");
        for (Camera.Size size : sizes) {
            Log.v(TAG, "\t width x height " + size.width + " x " + size.height);
        }

        Log.v(TAG, "\t surported frameRates is : ");
        for (int value : frameRates) {
            Log.v(TAG, "\t FrameRate : " + value);
        }

        // Pick the first preview size that is equal or bigger, or pick the last (biggest) option if we cannot
        // reach the initial settings of mImageWidth/mImageHeight.
        for (int i = 0; i < sizes.size(); i++) {
            if ((sizes.get(i).width >= mImageWidth && sizes.get(i).height >= mImageHeight) || i == sizes.size() - 1) {
                mImageWidth = sizes.get(i).width;
                mImageHeight = sizes.get(i).height;
                Log.v(TAG, "Changed to supported resolution: " + mImageWidth + "x" + mImageHeight);
                break;
            }
        }
        camParams.setPreviewSize(mImageWidth, mImageHeight);

        for (int i = 0; i < frameRates.size(); i++) {
            if (frameRates.get(i) >= mFrameRate || i == frameRates.size() - 1) {
                mFrameRate = frameRates.get(i);
                Log.v(TAG, "Changed to supported mFrameRate : " + mFrameRate);
                break;
            }
        }


        Log.v(TAG, "Setting mImageWidth: " + mImageWidth + " mImageHeight: " + mImageHeight + " mFrameRate: " + mFrameRate);

        camParams.setPreviewFrameRate(mFrameRate);

        mCamera.setParameters(camParams);
    }

    /**
     * From Google.doc
     *
     * @param activity
     * @param cameraId
     * @param camera
     */
    public static void setCameraDisplayOrientation(Activity activity, int cameraId, android.hardware.Camera camera) {
        android.hardware.Camera.CameraInfo info = new android.hardware.Camera.CameraInfo();
        android.hardware.Camera.getCameraInfo(cameraId, info);
        int rotation = activity.getWindowManager().getDefaultDisplay()
                .getRotation();
        int degrees = 0;
        switch (rotation) {
            case Surface.ROTATION_0:
                degrees = 0;
                break;
            case Surface.ROTATION_90:
                degrees = 90;
                break;
            case Surface.ROTATION_180:
                degrees = 180;
                break;
            case Surface.ROTATION_270:
                degrees = 270;
                break;
        }

        int result;
        if (info.facing == Camera.CameraInfo.CAMERA_FACING_FRONT) {
            result = (info.orientation + degrees) % 360;
            result = (360 - result) % 360;  // compensate the mirror
        } else {  // back-facing
            result = (info.orientation - degrees + 360) % 360;
        }
        camera.setDisplayOrientation(result);
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.controller_btn:
                Button btn = (Button) v;
                if (!btn.getText().equals("Click To Stop Stream")) {
                    startStreamer();
                    btn.setText("Click To Stop Stream");
                } else {
                    stopStreamer();
                    btn.setText("Click To Start Stream");
                }
                break;
        }
    }


    @Override
    protected void onStop() {
        super.onStop();

    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        Log.v(TAG, "onDestroy");

        mCameraTextureView.release();

        mStreamer.release();

        mCamera.release();
        mCamera = null;

        mVideoRecorderManager.release();
        mAudioRecorderManager.release();
    }

    static final int STREAM_STATUS_IDLE = 0;
    static final int STREAM_STATUS_ING = 1;
    static final int STREAM_STATUS_PAUSE = 2;
    static final int STREAM_STATUS_ERROR = -1;

    private int mStreamStatus = STREAM_STATUS_IDLE;

    private void startStreamer() {
        Log.v(TAG, "startStreamer");
        mVideoRecorderManager.start();
//        mAudioRecorderManager.start();

        mStreamStatus = STREAM_STATUS_ING;
    }

    private void stopStreamer() {
        Log.v(TAG, "stopStreamer");

        mVideoRecorderManager.stop();
//        mAudioRecorderManager.stop();

        mStreamer.pause();

        mStreamStatus = STREAM_STATUS_IDLE;
    }


    static class AsynStreamerHandler extends Handler {

        static Looper createAsynThreadLooper(String name) {
            HandlerThread innerThread = new HandlerThread(name);
            innerThread.start();
            return innerThread.getLooper();
        }

        public FFMpegStreamer mStreamer;

        public AsynStreamerHandler(String name, FFMpegStreamer streamer) {
            super(createAsynThreadLooper(name));
            mStreamer = streamer;
        }

        boolean mHasFlyPacket = false;


        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            mHasFlyPacket = true;

            switch (msg.what) {
                case FFMpegStreamer.RAW_DATA_VIDEO:
                    mStreamer.encodeVideo((byte[]) msg.obj);
                    break;
                case FFMpegStreamer.RAW_DATA_AUDIO:
                    mStreamer.encodeAudio((short[]) msg.obj, msg.arg1);
                    break;
            }


            mHasFlyPacket = false;
        }

        public void setStreamer(FFMpegStreamer streamer) {
            mStreamer = streamer;
        }

        public void postEncodeData(final byte[] data, int type) {
            Log.v(TAG, "postEncodeData");
            if (!mHasFlyPacket) {
                Log.v(TAG, "\t no fly packet , post encode task ...");
                Message msg = Message.obtain();
                msg.what = type;
                msg.obj = data;
                sendMessage(msg);
            } else {
                Log.w(TAG, "\t has flying packet ,skip this packet ");
            }

        }

        public void release() {
            removeCallbacksAndMessages(null);
            getLooper().quit();
        }

    }


    static class VideoRecorderManager implements Camera.PreviewCallback {

        static String TAG = VideoRecorderManager.class.getSimpleName();

        private long lastTime = 0;
        private long strideTime = 0;

        private Camera mCamera;

        public String getTimeFromMilli(long millis) {
//        return String.format("%02d min, %02d sec",
//                TimeUnit.MILLISECONDS.toMinutes(millis),
//                TimeUnit.MILLISECONDS.toSeconds(millis) - TimeUnit.MINUTES.toSeconds(TimeUnit.MILLISECONDS.toMinutes(millis))
//        );

            return String.valueOf(millis);
        }

        private AsynStreamerHandler mVideoImageTask;

        public VideoRecorderManager(Camera camera, FFMpegStreamer streamer) {
            mVideoImageTask = new AsynStreamerHandler("VideoEncodeTask", streamer);
            mCamera = camera;
        }

        public void init() {
            Log.v(TAG, "init");

        }

        public void start() {
            Log.v(TAG, "start");
            mCamera.setPreviewCallback(this);
        }


        public void stop() {
            Log.v(TAG, "stop");
            mCamera.setPreviewCallback(null);
        }

        public void release() {
            Log.v(TAG, "release");
        }

        @Override
        public void onPreviewFrame(byte[] data, Camera camera) {
            Log.v(TAG, "onPreviewFrame -> " + data.length);

            long currentTime = System.currentTimeMillis();
            strideTime = currentTime - lastTime;
            lastTime = currentTime;

            Log.v(TAG, " currentTime -> " + getTimeFromMilli(currentTime) + " stride -> " + strideTime);

            //default data format is YUV420sp (NV21)

            mVideoImageTask.postEncodeData(data, FFMpegStreamer.RAW_DATA_VIDEO);
//        mCamera.setPreviewCallback(null);
        }

    }


    static class AudioRecorderManager {

        String TAG = AudioRecorderManager.class.getSimpleName();


        private AudioRecord mAudioRecorder;

        private int mAudioBufferSize;

        private int mSampleRateInHz;

        private FFMpegStreamer mStreamer;

        private Thread mStreamerThread;

        private boolean mSwitch = false;

        private AsynStreamerHandler mAsynHandler;

        class AudioRecorderRunnable implements Runnable {

            @Override
            public void run() {
                android.os.Process.setThreadPriority(android.os.Process.THREAD_PRIORITY_URGENT_AUDIO);

                while (mSwitch) {

                    ShortBuffer mAudioBuffer = ShortBuffer.allocate(mAudioBufferSize);
                    int bufferReadResult = mAudioRecorder.read(mAudioBuffer.array(), 0, mAudioBuffer.capacity());
                    mAudioBuffer.limit(bufferReadResult);

                    int sampleCount = bufferReadResult / 2; // because we use AudioFormat.ENCODING_PCM_16BIT ,each sample 2 bytes,so sampleNumber = totalBytes / 2 .

                    Log.v(TAG, "buffer -> " + bufferReadResult + "\t samples -> " + sampleCount);

                    if (bufferReadResult > 0) {

                        for (int i = 0; i < bufferReadResult && i < 10; i++) {
                            Log.v(TAG, "\t The " + i + "st short sample -> " + shortToBinaryString(mAudioBuffer.array()[i]));
                        }

//                        try {
//                            mStreamer.encodeAudio(mAudioBuffer.array(), sampleCount);
//                        } catch (Exception e ) {
//                            e.printStackTrace();
//                        }

                        Message msg = Message.obtain();
                        msg.what = FFMpegStreamer.RAW_DATA_AUDIO;
                        msg.obj = mAudioBuffer.array();
                        msg.arg1 = sampleCount;

                        mAsynHandler.sendMessage(msg);

//                        mSwitch = false;

                        Log.v(TAG, "\t encoding audio " + bufferReadResult + " ...");
                    } else {
                        Log.w(TAG, "reading audio failed ...");

                    }
                }
            }
        }

        private static final String shortToBinaryString(short value) {
//            return value + " : " + (Integer.toBinaryString(value & 0xFFFF));

//            return String.format("%016d", Integer.parseInt(Integer.toBinaryString(value))) + " [" + value + "]";

            return String.format("%16s", Integer.toBinaryString(value & 0xFFFF)).replace(' ', '0') + " [" + value + "]";
        }

        public AudioRecorderManager(FFMpegStreamer streamer, int sampleRateInHz) {
            mStreamer = streamer;
            mSampleRateInHz = sampleRateInHz;
        }

        public void init() {
            Log.v(TAG, "init");

            mAsynHandler = new AsynStreamerHandler("AudioStreamerTask",mStreamer);

            mAudioBufferSize = AudioRecord.getMinBufferSize(mSampleRateInHz, AudioFormat.CHANNEL_IN_STEREO, AudioFormat.ENCODING_PCM_16BIT);
            Log.v(TAG, "\t audioBufferSize is -> " + mAudioBufferSize);

            mAudioRecorder = new AudioRecord(MediaRecorder.AudioSource.MIC, mSampleRateInHz, AudioFormat.CHANNEL_IN_STEREO, AudioFormat.ENCODING_PCM_16BIT, mAudioBufferSize);


        }

        public void start() {
            Log.v(TAG, "start");
            if (mStreamerThread != null) {
                throw new RuntimeException("Illegal state because thread is running !!!");
            }
            mAudioRecorder.startRecording();
            mSwitch = true;
            mStreamerThread = new Thread(new AudioRecorderRunnable());
            mStreamerThread.start();
        }

        public void stop() {
            Log.v(TAG, "stop");
            if (mStreamerThread == null) {
                return;
            }
            mSwitch = false;
            try {
                mStreamerThread.join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            mStreamerThread = null;
            mAudioRecorder.stop();

        }

        public void release() {
            Log.v(TAG, "release");

            stop();
            mAudioRecorder.release();
            mAudioRecorder = null;
        }

    }


}
