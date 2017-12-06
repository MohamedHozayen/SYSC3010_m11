import time 
import serial
import subprocess

ser=serial.Serial('/dev/ttyACM0',9600)
past_temp='0'
past_flow='0'

def read_arduino():
	arduinoData=ser.readline()
	return arduinoData

def write_file(data):
	file=open('values.txt','w')
	file.write(str(data))
	file.close()

def main():
        temp='0'
        flow='0'
        current='0'
        sensed='0'
	while True:
		read=read_arduino()
		data=str(read)
		print data
		
		if len(data.split()) < 2:
                        temp=data.split()
                        temp=temp.split(':')[1]
                if len(data.split()) < 3 and len(data.split()) > 1 :
                        temp,flow=data.split()
                        temp=temp.split(':')[1]
                        flow=flow.split(':')[1]        
                        
                if len(data.split()) < 4 and len(data.split()) > 2 :
                        temp,flow,current=data.split()
                        temp=temp.split(':')[1]
                        flow=flow.split(':')[1]        
                         
                        current=current.split(':')[1]        
                   
                if len(data.split())  == 4:
                        temp,flow,current,sensed=data.split()
                        temp=temp.split(':')[1]
                        flow=flow.split(':')[1]        
                       
                                
                        current=current.split(':')[1]
                                
                        sensed=sensed.split(':')[1]
               
                
                          
               
                if len(flow) ==1:
                        flow='0'+flow;
                
                if len(current)==1:
                          current='0'+current
                if len(current)==3:
                          current=current[:2]
                
              
                
                transmit = temp+flow+current+sensed
               
                        
                
		subprocess.call(['sudo','./codesend',transmit])
                time.sleep(0.5)
		subprocess.call(['sudo','./codesend',transmit])

if __name__ == "__main__": main()
