package volador.demoffmpeg;

import android.content.Context;
import android.graphics.SurfaceTexture;
import android.hardware.Camera;
import android.util.AttributeSet;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.TextureView;

import java.io.IOException;

/**
 * Created by volador on 16/4/28.
 */
public class CameraSurfaceView extends SurfaceView implements SurfaceHolder.Callback {


    static final String TAG = CameraSurfaceView.class.getSimpleName();


    private Camera mCamera;

    private SurfaceHolder mHolder;


    public CameraSurfaceView(Context context) {
        super(context);

        init(context);
    }

    public CameraSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);

        init(context);
    }

    public CameraSurfaceView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);

        init(context);
    }


    private SurfaceHolder mLiveHolder;


    public void init(Context context) {

        mHolder = getHolder();
        getHolder().addCallback(this);

        mHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
    }


    public void release() {
        Log.v(TAG, "release");
        setCamera(null);
    }


    public void setCamera(Camera camera) {
        Log.v(TAG, "setCamera");
        if (camera == null) {
            if (mCamera != null) {
                stopPreview();
                mCamera = null;
            }
        } else {
            if (mCamera == null) {
                mCamera = camera;
                if (mLiveHolder != null) {
                    startPreview(mLiveHolder);
                }
            } else {
                throw new RuntimeException("Exiting camera");
            }
        }
    }


    private void startPreview(SurfaceHolder surfaceTexture) {
        Log.v(TAG, "startPreview");

        try {

            mCamera.setPreviewDisplay(surfaceTexture);
            mCamera.startPreview();

        } catch (IOException e) {
            e.printStackTrace();
        }


    }

    private void stopPreview() {
        Log.v(TAG, "stopPreview");
        mCamera.stopPreview();
    }


    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        Log.v(TAG, "surfaceCreated");

        mLiveHolder = holder;

        if (mCamera == null) {
            return;
        }

        startPreview(holder);


    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        Log.v(TAG, "surfaceChanged");
        mLiveHolder = holder;

        if (mCamera == null) {
            return;
        }

        stopPreview();
        startPreview(holder);
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        Log.v(TAG, "surfaceDestroyed");

        mLiveHolder = holder;

        if (mCamera == null) {
            return;
        }

        stopPreview();

    }
}
