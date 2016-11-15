package volador.retrofit;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import com.google.gson.JsonElement;

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


    public class PTestApi {

        String company;

        @Override
        public String toString() {
            return "company -> " + company;
        }
    }


    public interface TestApi {

        @GET("users/basil2style")
        Call<JsonElement> basil2style();

    }


    public void performRequest() {

        Retrofit retrofit =
                new Retrofit.Builder()
                        .baseUrl("https://api.github.com/")
                        .addConverterFactory(GsonConverterFactory.create())
                        .build();

        TestApi api = retrofit.create(TestApi.class);
        Call<JsonElement> call = api.basil2style();


        mMonitor.setText("Sending ...");

        call.enqueue(new Callback<JsonElement>() {

            @Override
            public void onResponse(Call<JsonElement> call, Response<JsonElement> response) {
                Log.i(TAG, "onResponse");

                Log.v(TAG, "\t response -> " + response.body());

                mMonitor.setText("response -> " + response.body());

            }

            @Override
            public void onFailure(Call<JsonElement> call, Throwable t) {
                Log.e(TAG, "onFailure -> " + t.getMessage());
                mMonitor.setText("onFailure -> " + t.getMessage());
            }
        });


    }


}
