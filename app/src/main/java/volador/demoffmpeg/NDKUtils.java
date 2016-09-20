package volador.demoffmpeg;

import android.hardware.Camera;

/**
 * Created by volador on 16/2/22.
 */
public class NDKUtils {

    static {
        System.loadLibrary("demoffmpeg");
    }

    public native String sayHello();


    public native int studyLesson1(String videoPath ,String outPicPath);

    public native int studyLesson2(String videoPath);




    public native int streamVideo(String inputUrl ,String outputUrl);

    public native int testEncodeYUVToH264(String inputUrl ,String outputUrl);


    /**
     * Preparing FFMpeg for future streaming
     *
     * @param outputURL
     * @return
     */
    public native int initFFMpeg(String outputURL);

    /**
     * Invoked when new frame arrived
     * @param data May have some problems with C/C++
     * @param frameWidth
     * @param frameHeight
     * @param frameRate
     * @return
     */
    public native int onPreviewFrame(byte[] data ,int frameWidth ,int frameHeight ,int frameRate);



    public native int releaseFFMpeg();

}
