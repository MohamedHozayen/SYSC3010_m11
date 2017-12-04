package com.project.birdalert;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;

import java.net.DatagramPacket;
import java.net.DatagramSocket;

public class MainActivity extends Activity {
    public static final String EXTRA_MESSAGE = "com.example.myfirstapp.MESSAGE";


    private Receiver receiver = null;
    private int port = 5002;
    private static final int PACKETSIZE = 100;

    private Gson gson;
    private long time;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        updateVals();   //initialize UI fields to zero

        GsonBuilder gsonBuilder = new GsonBuilder();
//        gsonBuilder.setDateFormat("M/d/yy hh:mm a");    //need to change the date format
        gson = gsonBuilder.create();


    }

    private void birdSensedToast() {
        long newTime = System.currentTimeMillis();
        if (newTime - time >= 6000) {
            Context context = getApplicationContext();
            CharSequence text = "Bird Sensed";
            int duration = Toast.LENGTH_SHORT;

            Toast toast = Toast.makeText(context, text, duration);
            time = newTime;
            toast.show();


        }

    }

    protected void onResume() {
        super.onResume();
        receiver = new Receiver();
        receiver.start();
    }

    protected void onPause() {
        super.onPause();
        receiver.kill();
    }

    private class Receiver extends Thread {
        private boolean running = true;
        private Metrics metrics;


        public void run() {
            // Open DGram socket

            try {
                DatagramSocket socket = new DatagramSocket(port);
                DatagramPacket packet = new DatagramPacket(new byte[PACKETSIZE],PACKETSIZE);
                while(running) {
                    Log.i("Loop: ", "1");
                    socket.receive(packet);

                    // parse JSON and put the values in an instance of Metrics class which has rpm, charge, temp fields
                    String received = new String(packet.getData()).trim();
                    Log.i("Received: ", received);
                    metrics = gson.fromJson(received, Metrics.class);
                    runOnUiThread(updateUi);

                }
                if (!socket.isClosed()) {
                    socket.close();
                }
            } catch (Exception e) {
                e.printStackTrace();

            }

        }

        public void kill() {
            Log.i("KILL: ", "HERE");
            running = false;
        }

        public String getFlowSpeed() {
            return metrics.flowSpeed + " L/min";
        }
        public String getFlowML() {
            return metrics.flowML + " ML/s";
        }
        public String getBirdSensed() {
            return metrics.birdSensed;
        }
        public String getTemp() {
            return metrics.temp + " C";
        }
    }

    //a runnable interface to set parsed values in UI
    private Runnable updateUi = new Runnable() {
        public void run() {
            if (receiver == null) return;
            TextView textView = (TextView) findViewById(R.id.flowSpeedValView);
            textView.setText(receiver.getFlowSpeed());

            textView = (TextView) findViewById(R.id.flowMLValView);
            textView.setText(receiver.getFlowML());

            textView = (TextView) findViewById(R.id.tempValView);
            textView.setText(receiver.getTemp());

//            textView = (TextView) findViewById(R.id.birdSensedView);
//            textView.setVisibility(receiver.getBirdSensed() == "1" ? View.VISIBLE : View.INVISIBLE);
            if (receiver.getBirdSensed().equals("1")) birdSensedToast();
        }
    };


    /* Called when user taps the Send button
     * TODO this message as a date to request for logs */
    public void sendMessage (View view){
        //Do something in response
        Intent intent = new Intent(this, DisplayMessageActivity.class);
        EditText editText = (EditText) findViewById(R.id.editText);
        String message = editText.getText().toString();
        intent.putExtra(EXTRA_MESSAGE, message);
        startActivity(intent);

    }
    //initializes metrics fields in UI to zero. Called from onCreate()
    public void updateVals() {
        TextView rpmVal = (TextView) findViewById(R.id.flowSpeedValView);
        rpmVal.setText("0");

        TextView chargeVal = (TextView) findViewById(R.id.flowMLValView);
        chargeVal.setText("0%");

        TextView tempVal = (TextView) findViewById(R.id.tempValView);
        tempVal.setText("0 C");
    }
}
