package volador.retrofit;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import com.google.gson.JsonElement;

import java.io.IOException;

import okhttp3.Interceptor;
import okhttp3.OkHttpClient;
import okhttp3.logging.HttpLoggingInterceptor;
import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;
import retrofit2.http.GET;

/**
 * Created by Fish on 2016/11/9 in DemoFFMpeg.
 */
public class HelloRetrofitActivity extends Activity {

    public static final String TAG = HelloRetrofitActivity.class.getSimpleName();

    private TextView mMonitor;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        mMonitor = new TextView(this);
        setContentView(mMonitor);

        mMonitor.setText("Click to test ...");

        mMonitor.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                performRequest();
            }
        });
    }


    public class DHelloPhp {

        String content;

        @Override
        public String toString() {
            return "content : " + content;
        }
    }


    public interface TestApi {

        @GET("HelloPhp/Hello.php")
        Call<DHelloPhp> helloPhp();

    }


    public void performRequest() {
        HttpLoggingInterceptor interceptor = new HttpLoggingInterceptor(new HttpLoggingInterceptor.Logger() {
            @Override
            public void log(String message) {
                Log.i("HttpLoggingInterceptor", message);
            }
        });
        interceptor.setLevel(HttpLoggingInterceptor.Level.BODY);
        OkHttpClient client = new OkHttpClient.Builder().addInterceptor(interceptor).build();

        Retrofit retrofit =
                new Retrofit.Builder()
                        .client(client)
                        .baseUrl("http://192.168.1.188/php/")
                        .addConverterFactory(GsonConverterFactory.create())
                        .build();

        TestApi api = retrofit.create(TestApi.class);
        Call<DHelloPhp> call = api.helloPhp();

        mMonitor.setText("Sending ...");

        call.enqueue(new Callback<DHelloPhp>() {

            @Override
            public void onResponse(Call<DHelloPhp> call, Response<DHelloPhp> response) {
                Log.i(TAG, "onResponse");

                Log.v(TAG, "\t response -> " + response.body());

                mMonitor.setText("response -> \n" + response.body());
            }

            @Override
            public void onFailure(Call<DHelloPhp> call, Throwable t) {
                Log.e(TAG, "onFailure -> " + t.getMessage());
                mMonitor.setText("onFailure -> " + t.getMessage());
            }
        });


    }


}
