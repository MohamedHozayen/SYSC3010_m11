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
     
     
RCSwitch mySwitch;
 
void sniff(){
	
     int PIN = 2;
     
     if(wiringPiSetup() == -1) {
       printf("wiringPiSetup failed, exiting...");
       return 0;
     }
	 int pulseLength = 0;
	 if (str != NULL) pulseLength = atoi(str);

     mySwitch = RCSwitch();
     if (pulseLength != 0) mySwitch.setPulseLength(pulseLength);
     mySwitch.enableReceive(PIN);  // Receiver on interrupt 0 => that is pin #2
     
    
     while(1) {
  
      if (mySwitch.available()) {
    
        int value = mySwitch.getReceivedValue();
    
        if (value == 0) {
          printf("Unknown encoding\n");
        } else {    
   
          printf("Received %i\n", mySwitch.getReceivedValue() );
        }
    
        mySwitch.resetAvailable();
    
      }
      
  
  }
}

int main(int argc, char *argv[]) {
  
     while(1){
		 
size_t pos = 0;
std::string token;
while ((pos = s.find(delimiter)) != std::string::npos) {
    token = s.substr(0, pos);
    std::cout << token << std::endl;
    s.erase(0, pos + delimiter.length());
}
std::cout << s << std::endl;
	 }
	 String str1;
	 str1 = argv[1];
	 sniff(str1);

  exit(0);


}

