package swordfish.simplemvc.mvc.controller;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;

import swordfish.simplemvc.mvc.mode.SimpleBaseMode;


/**
 * Created by Fish on 2016/6/13.
 * <p>
 * Internal MVC
 * <p>
 * Controller of MVC
 */
public class SimpleBaseActivity<T extends SimpleBaseMode> extends Activity {

    protected String TAG = this.getClass().getSimpleName();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);


        Log.i(TAG,"");
    }
}





