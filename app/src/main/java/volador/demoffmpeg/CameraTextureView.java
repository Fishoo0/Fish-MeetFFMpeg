package volador.demoffmpeg;

import android.content.Context;
import android.graphics.SurfaceTexture;
import android.hardware.Camera;
import android.util.AttributeSet;
import android.util.Log;
import android.view.TextureView;

import java.io.IOException;

/**
 * Created by volador on 16/4/28.
 */
public class CameraTextureView extends TextureView implements TextureView.SurfaceTextureListener {


    static final String TAG = CameraTextureView.class.getSimpleName();


    private Camera mCamera;


    public CameraTextureView(Context context) {
        super(context);
    }

    public CameraTextureView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public CameraTextureView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
    }


    public CameraTextureView(Context context, Camera camera) {
        super(context);
        mCamera = camera;

        setSurfaceTextureListener(this);

    }


    public void  setCamera(Camera camera) {
        mCamera = camera;
    }




    private void startPreview(SurfaceTexture surfaceTexture) {

        Log.v(TAG, "startPreview");

        try {

            mCamera.setPreviewTexture(surfaceTexture);

            mCamera.startPreview();


        } catch (IOException e) {
            e.printStackTrace();
        }


    }

    private void stopPreview() {
        mCamera.stopPreview();
    }


    @Override
    public void onSurfaceTextureAvailable(SurfaceTexture surface, int width, int height) {
        Log.v(TAG,"onSurfaceTextureAvailable");

        startPreview(surface);
    }

    @Override
    public void onSurfaceTextureSizeChanged(SurfaceTexture surface, int width, int height) {
        Log.v(TAG,"onSurfaceTextureSizeChanged");

        stopPreview();
        startPreview(surface);
    }

    @Override
    public boolean onSurfaceTextureDestroyed(SurfaceTexture surface) {
        Log.v(TAG,"onSurfaceTextureDestroyed");
        stopPreview();
        return false;
    }

    @Override
    public void onSurfaceTextureUpdated(SurfaceTexture surface) {
        Log.v(TAG,"onSurfaceTextureUpdated");

        Log.v(TAG,"????");
    }
}
