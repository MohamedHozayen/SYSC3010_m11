import time
import serial


ser=serial.Serial('COM3', baudrate = 9600,timeout = 1)


def read_arduino():
	arduinoData=ser.readline()
	return arduinoData

def write_file(data):
	file=open('test.txt','w')
	file.write(str(data))
	file.close()
	
def main():
	while True:
		data=read_arduino()
		write_file(data)
		time.sleep(1)
    
if __name__ == "__main__": main()    

