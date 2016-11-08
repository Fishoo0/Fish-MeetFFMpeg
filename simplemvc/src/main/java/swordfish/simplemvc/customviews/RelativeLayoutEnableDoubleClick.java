package swordfish.simplemvc.customviews;

import android.annotation.SuppressLint;
import android.content.Context;
import android.util.AttributeSet;
import android.view.GestureDetector;
import android.view.GestureDetector.OnDoubleTapListener;
import android.view.MotionEvent;
import android.view.SoundEffectConstants;
import android.widget.RelativeLayout;


public class RelativeLayoutEnableDoubleClick extends RelativeLayout implements OnDoubleTapListener {

    private static final String TAG = RelativeLayoutEnableDoubleClick.class.getSimpleName();

    @SuppressLint("NewApi")
    public RelativeLayoutEnableDoubleClick(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
        // TODO Auto-generated constructor stub
        init();
    }

    public RelativeLayoutEnableDoubleClick(Context context, AttributeSet attrs) {
        super(context, attrs);
        // TODO Auto-generated constructor stub
        init();
    }

    public RelativeLayoutEnableDoubleClick(Context context) {
        super(context);
        // TODO Auto-generated constructor stub
        init();
    }

    private OnClickListener mDoubleClickListener;

    private boolean bEnableDoubleClickSound = false;

    public void setOnDoubleClickListener(OnClickListener clickListener, boolean enableDoubleClickSound) {
        mDoubleClickListener = clickListener;
        bEnableDoubleClickSound = enableDoubleClickSound;
    }

    public void setOnDoubleClickListener(OnClickListener clickListener) {
        setOnDoubleClickListener(clickListener, false);
    }

    private void init() {
        initGesture();
    }

    private GestureDetector mGestureDetector;

    private void initGesture() {
        mGestureDetector = new GestureDetector(getContext(), new GestureDetector.SimpleOnGestureListener() {

            @Override
            public boolean onDown(MotionEvent e) {
                // TODO Auto-generated method stub
                // if(Logger.isLoggingEnabled())Logs.i(TAG, "onDown");
                // CustomViewImplmentDoubleClickLinearLayout.this.setSelected(true);
                return true;
            }

            @Override
            public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY) {
                // TODO Auto-generated method stub
                // if(Logger.isLoggingEnabled())Logs.i(TAG, "onFling");
                return super.onFling(e1, e2, velocityX, velocityY);
            }

            @Override
            public void onLongPress(MotionEvent e) {
                // TODO Auto-generated method stub
                // if(Logger.isLoggingEnabled())Logs.i(TAG, "onLongPress");
                super.onLongPress(e);
            }

            @Override
            public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY) {
                // TODO Auto-generated method stub
                // if(Logger.isLoggingEnabled())Logs.i(TAG, "onScroll");
                return super.onScroll(e1, e2, distanceX, distanceY);
            }

            @Override
            public void onShowPress(MotionEvent e) {
                // TODO Auto-generated method stub
                // if(Logger.isLoggingEnabled())Logs.i(TAG, "onShowPress");
                super.onShowPress(e);
            }

            @Override
            public boolean onSingleTapUp(MotionEvent e) {
                // TODO Auto-generated method stub
                // if(Logger.isLoggingEnabled())Logs.i(TAG, "onSingleTapUp");
                return super.onSingleTapUp(e);
            }
        });
        mGestureDetector.setOnDoubleTapListener(this);
    }


    public boolean performDoubleClick() {
        if (mDoubleClickListener != null) {
            if (bEnableDoubleClickSound) {
                playSoundEffect(SoundEffectConstants.CLICK);
            }
            mDoubleClickListener.onClick(this);
            return true;
        }
        return false;
    }

    @Override
    public boolean performClick() {
        if (mSingleClickListener != null) {
            playSoundEffect(SoundEffectConstants.CLICK);
            mSingleClickListener.onClick(RelativeLayoutEnableDoubleClick.this);
            return true;
        }
        return false;
    }

    @Override
    public boolean onDoubleTap(MotionEvent e) {
        return performDoubleClick();
    }


    @Override
    public boolean onDoubleTapEvent(MotionEvent e) {
        // TODO Auto-generated method stub
        // if(Logger.isLoggingEnabled())Logs.i(TAG, "onDoubleTapEvent");
        return false;
    }

    @Override
    public boolean onSingleTapConfirmed(MotionEvent e) {
        // TODO Auto-generated method stub
        // if(Logger.isLoggingEnabled())Logs.i(TAG, "onSingleTapConfirmed");
        return performClick();
    }


    private OnClickListener mSingleClickListener;

    @Override
    public void setOnClickListener(OnClickListener l) {
        // TODO Auto-generated method stub
        super.setOnClickListener(l);
        mSingleClickListener = l;
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        // TODO Auto-generated method stub
//        Log.v(TAG, "onTouchEvent -> " + event);
        if (!isClickable()) {
            return super.onTouchEvent(event);
        }
        boolean result = mGestureDetector.onTouchEvent(event);
        switch (event.getAction()) {
            case MotionEvent.ACTION_UP:
                break;
            case MotionEvent.ACTION_DOWN:
                break;
            default:
                break;
        }
        return result;
    }

}
