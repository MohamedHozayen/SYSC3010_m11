package com.project.birdalert;

/**
 * Created by Vadim Yastrebov (CFD2) on 12/11/17.
 * Version 1.2
 */

//this class is used to store parsed measurement values.
public class Metrics {
    //this is for get http requests for measurements for a date. since it is unimplemented on the server it was left alone.
//    @SerializedName("date")
//    Date dateCreated;


    //the measuremnt fields to parse JSON into
    String flowSpeed;
    String flowML;
    String temp;
    String birdSensed;

}
