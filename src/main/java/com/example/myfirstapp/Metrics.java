package com.example.myfirstapp;
import java.util.Date;
import com.google.gson.annotations.SerializedName;

/**
 * Created by CFD2 on 12/11/17.
 */

public class Metrics {
    Date date;
    String rpm;
    String temp;
    String charge;
    @SerializedName("id")
    long ID;

    @SerializedName("date")
    Date dateCreated;

    String title;
    String author;
    String url;
    String body;

}
