import time 
import serial
import subprocess

#Author: Umar Sheikh-Omar
#Python Version: Python 2.7
#Uses pySerial Library
#Written: November 28 2017


def read_arduino():
	arduinoData=ser.readline()
	return arduinoData

def main():
        temp='0'
        flow='0'
        current='0'
        sensed='0'
        #Determine if arduino is connected to RPI
        try:
                ser=serial.Serial('/dev/ttyACM0',9600)
                connected = 1
        except: serial.SerialException:
                print "No connection established"
	while connected ==1:
		read=read_arduino()
		data=str(read)
		
		#Splits data read into a temp value
		if len(data.split()) < 2:
                        temp=data.split()
                        temp=temp.split(':')[1]
                #Splits data read into 2 equal parts containing temp and flow 
                if len(data.split()) < 3 and len(data.split()) > 1 :
                        temp,flow=data.split()
                        temp=temp.split(':')[1]
                        flow=flow.split(':')[1]        
                #Splits data read into 3 equal parts containing temp,flow, and current 
                if len(data.split()) < 4 and len(data.split()) > 2 :
                        temp,flow,current=data.split()
                        temp=temp.split(':')[1]
                        flow=flow.split(':')[1]        
                        current=current.split(':')[1]        
                #Splits data read into 4 equal parts containing temp,flow,current, and sensed  
                if len(data.split())  == 4:
                        temp,flow,current,sensed=data.split()
                        temp=temp.split(':')[1]
                        flow=flow.split(':')[1]        
                        current=current.split(':')[1]
                        sensed=sensed.split(':')[1]         
                #Looks at flow, if single digit prepend '0'
                if len(flow) ==1:
                        flow='0'+flow;
                #Looks at current, if single digit prepend '0'
                if len(current)==1:
                          current='0'+current
                #Looks at current, truncate to 2 digits
                if len(current)==3:
                          current=current[:2]
                #combine all the data into a string to be sent
                transmit = temp+flow+current+sensed
                #Call the execuatable file with transmit as argument
		subprocess.call(['sudo','./codesend',transmit])
		#wait for message to be received by other RPI
                time.sleep(0.5)
                #Call executable again to be a fail-safe
		subprocess.call(['sudo','./codesend',transmit])

if __name__ == "__main__": main()
