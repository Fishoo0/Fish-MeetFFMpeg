package volador.demoffmpeg;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Environment;
import android.view.View;

import volador.retrofit.HelloRetrofitActivity;
import volador.sdl.MySDLActivity;

public class MainActivity extends Activity implements View.OnClickListener {


    static final String TAG = "Lesson-MainActivity111";


    static {
        System.loadLibrary("demoffmpeg");
    }


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

//        NDKUtils jni = new NDKUtils();
//
//
//        textView.append("\n" + jni.sayHello());
//
//        int result = -2;
//
//        result = jni.streamVideo(FileUtils.getTestMovPath(), FileUtils.getTestOutPath());
//
//        textView.append("\n result of streamVideo : " + result);

//
//
//
//
//        int result = -2;
//
////        textView.append("\n=================");
////        result = jni.studyLesson1(videoFile.getPath(), videoDir.getPath());
////        textView.append("result Of studyLesson1 -> " +result);
//
//        textView.append("\n=================");

//        result = jni.studyLesson2(videoFile.getPath());
//
//        Toast.makeText(this,""+result,Toast.LENGTH_SHORT).show();

    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.ffmpeg:
                startActivity(new Intent(MainActivity.this, CameraStreamerActivity.class));
                break;

            case R.id.retrofit:
                startActivity(new Intent(MainActivity.this, HelloRetrofitActivity.class));
                break;

            case R.id.main:
                onMainClick(v);
                break;

            case R.id.sdlactivity:
                startActivity(new Intent(MainActivity.this, MySDLActivity.class));
                break;
        }
    }


    public void onMainClick(View view) {
        NDKUtils utils = new NDKUtils();
        utils.studyLesson1(FileUtils.getMoveFilePath("test.mp4"), FileUtils.getMoveInternalDir("out"));
    }
}
