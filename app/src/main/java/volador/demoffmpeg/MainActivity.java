package volador.demoffmpeg;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

public class MainActivity extends Activity {


    static final String TAG = "Lesson-MainActivity111";


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);


        TextView textView = (TextView) findViewById(R.id.textview);


        textView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startActivity(new Intent(MainActivity.this ,CameraStreamerActivity.class));
            }
        });

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
}
