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

/**
 * Created by Vadim Yastrebov (CFD2) on 12/11/17.
 * Version 1.3
 */

public class MainActivity extends Activity {
    //this is for passing the date to another activity to http GET history of measurements for that date. Uninmplemented on server side.
    public static final String EXTRA_MESSAGE = "com.example.myfirstapp.MESSAGE";


    //Receiver is the thread instance class, which receives data continuously over UDP
    private Receiver receiver = null;

    //Port for receiving data over UDP
    private int port = 5002;

    //Packetsize is the buffer size
    private static final int PACKETSIZE = 100;

    //an instance of Gson to parse JSON object with measurements received over UDP
    private Gson gson;

    //Is for toast timeout so it will not pop up continuously
    private long time;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        updateVals();   //initialize UI fields to zero

        //Build gson object to use it from another thread
        GsonBuilder gsonBuilder = new GsonBuilder();
        gson = gsonBuilder.create();


    }

    //toast notification when the bird is sensed
    //uses if the bird is detected continuously it would use the 6 seconds timeout feature to prevent constant annoying toasts
    private void birdSensedToast() {
        //the whole purpose of this is to measure how many seconds have passed since the last toast
        //if it is less than 6 then the toast will not be shown
        long newTime = System.currentTimeMillis();
        if (newTime - time >= 6000) {

            //setting up a toast
            Context context = getApplicationContext();
            CharSequence text = "Bird Sensed";  //toast notification message
            int duration = Toast.LENGTH_SHORT;
            Toast toast = Toast.makeText(context, text, duration);
            time = newTime;
            toast.show();   //showing the toast


        }

    }

    protected void onResume() {
        super.onResume();
        //creating the receiver to start another thread.
        //we do it on resume so the app would still work after reopening it from background
        receiver = new Receiver();
        receiver.start();
    }

    protected void onPause() {
        super.onPause();
        //we need to kill a  receiver thread when the app is paused
        receiver.kill();
    }
    //The class which receives and parses data from UDP. Parsing is done with help of Gson.
    private class Receiver extends Thread {
        //the flag for thread management
        private boolean running = true;
        //instance of a class which only has the fields of measurements to parse JSON into them
        private Metrics metrics;


        public void run() {

            try {
                //Opening DGram socket
                DatagramSocket socket = new DatagramSocket(port);
                DatagramPacket packet = new DatagramPacket(new byte[PACKETSIZE],PACKETSIZE);
                //the loop to manage receiving.
                while(running) {
                    //logging if we are in a loop
                    Log.i("Loop: ", "1");
                    //receive the packet
                    socket.receive(packet);

                    // parse JSON and put the values in an instance of Metrics class which has birdSensed, temp, waterflow etc. fields
                    String received = new String(packet.getData()).trim();
                    Log.i("Received: ", received);
                    metrics = gson.fromJson(received, Metrics.class);   //using gson to parse JSON into instance of Metrics.class
                    runOnUiThread(updateUi);    //calling this interface to update UI textViews with parsed values

                }
                //need to make sure the socket is closed.
                if (!socket.isClosed()) {
                    socket.close();
                }
            } catch (Exception e) {
                e.printStackTrace();

            }

        }

        public void kill() {
            //need to stop the thread from receiving when this is called.
            Log.i("KILL: ", "was called");
            running = false;
        }

        //getters for the metrics object fields where the parsed values are stored
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
            if (receiver == null) return;   //if receiver is never run, we just dont update the UI

            //find the corresponding textView for each parsed value and update that textView
            TextView textView = (TextView) findViewById(R.id.flowSpeedValView);
            textView.setText(receiver.getFlowSpeed());

            textView = (TextView) findViewById(R.id.flowMLValView);
            textView.setText(receiver.getFlowML());

            textView = (TextView) findViewById(R.id.tempValView);
            textView.setText(receiver.getTemp());

            //if the birdSensed is 1, we need to show a toast to notify the user.
            if (receiver.getBirdSensed().equals("1")) birdSensedToast();
        }
    };


    /* Called when user taps the Send button
     * TODO this message as a date to request for logs
     * unimplemented on the server side*/
    public void sendMessage (View view){
        //Do something in response
        Intent intent = new Intent(this, DisplayMessageActivity.class);
        EditText editText = (EditText) findViewById(R.id.editText);
        String message = editText.getText().toString();
        intent.putExtra(EXTRA_MESSAGE, message);
        startActivity(intent);

    }
    //initializes metrics fields in UI to zero. Called from onCreate()
    //only done when program is launched
    public void updateVals() {
        TextView rpmVal = (TextView) findViewById(R.id.flowSpeedValView);
        rpmVal.setText("0");

        TextView chargeVal = (TextView) findViewById(R.id.flowMLValView);
        chargeVal.setText("0%");

        TextView tempVal = (TextView) findViewById(R.id.tempValView);
        tempVal.setText("0 C");
    }
}
