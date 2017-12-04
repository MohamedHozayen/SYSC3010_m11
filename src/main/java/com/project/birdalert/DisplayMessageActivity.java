package com.project.birdalert;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.widget.TextView;

/**
 * Created by Vadim Yastrebov (CFD2) on 12/11/17.
 * Version 1.2
 */

public class DisplayMessageActivity extends Activity {
//    private RequestQueue requestQueue;

    //placeholder JSON file
//    private static final String ENDPOINT = "https://kylewbanks.com/rest/posts.json";

//    private Gson gson;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_display_message);

        //Get the Intent that started this activity and extract the string
        Intent intent = getIntent();

        //TODO message as the date request for logs. It's not doing anything for now.
        String message = intent.getStringExtra(MainActivity.EXTRA_MESSAGE);
        //Capture the layout's TextView and set the string as its text
        TextView textView = (TextView) findViewById(R.id.textView2);
        textView.setText(message);


//        requestQueue = Volley.newRequestQueue(getApplicationContext());

//        GsonBuilder gsonBuilder = new GsonBuilder();
//        gsonBuilder.setDateFormat("M/d/yy hh:mm a");    //need to change the date format
//        gson = gsonBuilder.create();

//        fetchMetrics();

    }

//    protected void onStop() {
//
//        super.onStop();
//        //All of the requests should be cancelled
//        requestQueue.cancelAll("req");
//    }

//    private void fetchMetrics() {
//        //forming a GET request to the ENDPOINT
//        StringRequest request = new StringRequest(Request.Method.GET, ENDPOINT, onMetricsLoaded, onMetricsError);
//        request.setTag("req");
//        try {
//            requestQueue.add(request); //send the request and check the response with listeners
//
//        } catch (Exception e) {
//            e.printStackTrace();
//        }
//    }
//
//    private final Response.Listener<String> onMetricsLoaded = new Response.Listener<String>() {
//        @Override
//        public void onResponse(String response) {   //if JSON obj is loaded, this will be called with response in String format
//            try {
//                //parse the response as array of Metrics objects and convert it into list for convenience
//                List<Metrics> metricsList = Arrays.asList(gson.fromJson(response, Metrics[].class));
//                Log.i("Metrics", response);
//                Log.i("Metrics", metricsList.size() + " metrics loaded. ");
//                for (Metrics metric : metricsList) {
//                    Log.i("Metrics", metric.ID + ": " + metric.title);
//                }
//                //TODO display parsed data in UI
//            } catch (Exception e) {
//                e.printStackTrace();
//            }
//        }
//    };
//
//    private final Response.ErrorListener onMetricsError = new Response.ErrorListener() {
//        @Override
//        public void onErrorResponse(VolleyError error) {    //if error occurred, it will be logged
//            Log.e("Metrics", error.toString());
//            //TODO error messages in UI for failed request
//        }
//    };



}
