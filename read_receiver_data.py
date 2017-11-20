
def read_receiver():
	 receiver_file=open("receiverFile.txt","r")
	 receiver_string = receiver_file.read()
	 metrics = receiver_string.split(",")
	 return metrics
	 

def get_Temp(metricsList):
	 temp = metricsList[0]
	 float_temp = float(temp)
	 return float_temp
	 
def get_pumpSpeed(metricsList):
	 pump = metricsList[1]
	 float_pump = float(pump)
	 return float_pump
	
def get_birdSensed(metricsList):
	 birdSensed = metricsList[2]
	 int_sensed = int(birdSensed)
	 return int_sensed
	 
def get_date(metricsList):
	 date = metricsList[3]
	 return date
def main():
	while True:
		metrics = read_receiver()
		temp= get_Temp(metrics)
		pumpSpeed = get_pumpSpeed(metrics)
		birdSense = get_birdSensed(metrics)
		date = get_date(metrics)
		
		print ('The temp is %.2f' %temp)
		print ('The pumpSpeed is %.2f' %pumpSpeed)
		print ('The birdSensed value is %i' %birdSense)
		print ('The date is %s' %date)
		break
    
if __name__ == "__main__": main()   
