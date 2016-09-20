package volador.demoffmpeg;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

/**
 * Created by volador on 16/4/12.
 */
public class StreamerActivity extends Activity {

    static final String TAG = StreamerActivity.class.getSimpleName();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.streamer_act);

        final EditText input = (EditText) findViewById(R.id.input);
        final EditText output = (EditText) findViewById(R.id.output);

        input.setText("test1.mp4");
        output.setText("rtmp://192.168.1.101:1935/fishdemo/android");

        input.setText("test.yuv");
        output.setText("testyuv.h264");

        Button startBtn = (Button) findViewById(R.id.action);

        final NDKUtils utils = new NDKUtils();

        startBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
//                String in = FileUtils.getMoveFilePath(input.getText().toString());
//                String out = output.getText().toString();
//                if(!out.contains(":")) {
//                    out = FileUtils.getMoveFilePath(out);
//                }
//                Log.v(TAG, "in -> " + in + " out -> " + out);
//                utils.streamVideo(in,out);


                String in = FileUtils.getMoveFilePath(input.getText().toString());
                String out = output.getText().toString();
                if(!out.contains(":")) {
                    out = FileUtils.getMoveFilePath(out);
                }
                Log.v(TAG, "in -> " + in + " out -> " + out);

                utils.testEncodeYUVToH264(in,out);

            }
        });
    }



}
