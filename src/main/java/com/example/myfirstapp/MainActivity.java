package com.example.myfirstapp;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;

import java.net.DatagramPacket;
import java.net.DatagramSocket;

public class MainActivity extends AppCompatActivity {
    public static final String EXTRA_MESSAGE = "com.example.myfirstapp.MESSAGE";


    private Receiver receiver = null;
    private int port = 8080;
    private static final int PACKETSIZE = 100;

    private Gson gson;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        updateVals();   //initialize UI fields to zero

        GsonBuilder gsonBuilder = new GsonBuilder();
        gsonBuilder.setDateFormat("M/d/yy hh:mm a");    //need to change the date format
        gson = gsonBuilder.create();


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
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            // Open DGram socket

            try {
                DatagramSocket socket = new DatagramSocket(port);
                DatagramPacket packet = new DatagramPacket(new byte[PACKETSIZE],PACKETSIZE);

                while(running) {
                    socket.receive(packet);

                    // parse JSON and put the values in an instance of Metrics class which has rpm, charge, temp fields
                    String received = new String(packet.getData());
                    metrics = gson.fromJson(received, Metrics.class);
                    runOnUiThread(updateUi);

                    if (socket != null) {
                        socket.close();
                    }
                }
            } catch (Exception e) {
                e.printStackTrace();

            }

        }

        public void kill() {
            running = false;
        }

        public String getRpm() {
            return metrics.rpm;
        }
        public String getCharge() {
            return metrics.charge;
        }
        public String getTemp() {
            return metrics.temp;
        }
    }

    //a runnable interface to set parsed values in UI
    private Runnable updateUi = new Runnable() {
        public void run() {
            if (receiver == null) return;
            TextView textView = (TextView) findViewById(R.id.rpmValView);
            textView.setText(receiver.getRpm());

            textView = (TextView) findViewById(R.id.chargeValView);
            textView.setText(receiver.getCharge());

            textView = (TextView) findViewById(R.id.tempValView);
            textView.setText(receiver.getTemp());


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
        TextView rpmVal = (TextView) findViewById(R.id.rpmValView);
        rpmVal.setText("0");

        TextView chargeVal = (TextView) findViewById(R.id.chargeValView);
        chargeVal.setText("0%");

        TextView tempVal = (TextView) findViewById(R.id.tempValView);
        tempVal.setText("0 C");
    }
}
