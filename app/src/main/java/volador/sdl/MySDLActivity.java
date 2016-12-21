package volador.sdl;

import android.os.Bundle;

import org.libsdl.app.SDLActivity;

import volador.demoffmpeg.FileUtils;

/**
 * Created by fish on 2016/12/14.
 */

public class MySDLActivity extends SDLActivity {


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }


    @Override
    protected String[] getArguments() {
        return new String[]{"rtmp://live.hkstv.hk.lxdns.com/live/hks"};
    }
}
