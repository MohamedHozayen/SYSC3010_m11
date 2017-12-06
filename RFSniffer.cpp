/*
	*RF Receiver
	*Author: Umar Sheikh-Omar 
	*Version 1.3
	*Created: Novermber 7,2017
	*Modified from git://github.com/ninjablocks/433Utils.git
*/


#include "../rc-switch/RCSwitch.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream> 
     
using namespace std;    
RCSwitch mySwitch;
 


int main(int argc, char *argv[]) {
  
     // This pin is not the first pin on the RPi GPIO header!
     // Consult https://projects.drogon.net/raspberry-pi/wiringpi/pins/
     // for more information.
     int PIN = 2;
     
     if(wiringPiSetup() == -1) {
       printf("wiringPiSetup failed, exiting...");
       return 0;
     }

     int pulseLength = 0;
     if (argv[1] != NULL) pulseLength = atoi(argv[1]);

     mySwitch = RCSwitch();
     if (pulseLength != 0) mySwitch.setPulseLength(pulseLength);
     mySwitch.enableReceive(PIN);  // Receiver on interrupt 0 => that is pin #2
     
    
     while(1) {
  
      if (mySwitch.available()) {
    
        int value = mySwitch.getReceivedValue();
    
        if (value == 0) {
          printf("Unknown encoding\n");
        } else {    
   	  int received = mySwitch.getReceivedValue();	
          printf("Received %i\n", mySwitch.getReceivedValue() );
	
	stringstream ss1;
	ss1 << received;
	string receivedString = ss1.str();
	int receivedLength = receivedString.length();
	//Handle any static noise
        if(receivedLength < 7 || receivedLength > 7){
		continue;
	}
    stringstream ss2;
	ss2 << received;
	string rec = ss2.str();
	time_t now=time(0);
	struct tm tstruct;
	char buf[80];
	tstruct =*localtime(&now);
	strftime(buf,sizeof(buf),"%Y-%m-%d.%X",&tstruct);
	string input = rec.substr(0,2)+','+rec.substr(2,2)+','+rec.substr(4,2)+'0'+','+rec.substr(6,1)+','+buf;
	//write data to text file to be used by database and server
	ofstream out("BirdALert.txt");
	out << input;
	out.close();
	
	}
        mySwitch.resetAvailable();
    
      }
      
  
  }

  exit(0);


}

