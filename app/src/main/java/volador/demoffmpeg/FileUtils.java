package volador.demoffmpeg;

import android.os.Environment;
import android.util.Log;

import java.io.File;

/**
 * Created by volador on 16/4/12.
 */
public class FileUtils {


    public static final String VideoName = "test.MOV";

    public static final String VideoOutName = System.currentTimeMillis() + "_out.MOV";

    public static File videoDir = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_MOVIES);


    public static String getTestMovPath() {
        File videoFile = new File(videoDir, VideoName);
        return videoFile.getPath();
    }

    public static String getTestOutPath() {
        File videoOut = new File(videoDir, VideoOutName);
        return videoOut.getPath();
    }


    public static File getMoveFile(String fileName) {
        return new File(videoDir,fileName);
    }

    public static String getMoveFilePath(String fileName) {
        return getMoveFile(fileName).getPath();
    }

}
