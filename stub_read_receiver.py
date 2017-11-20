receiver_data = "25,45,1,2017-11-15 12:44"

def read_receiver():
	 receiver_string = receiver_data #received_data used to represent the data received from receiver. It is hardcoded
	 metrics = receiver_string.split(",") #split using the ',' delimiter
	 return metrics  #returns the metrics array

def get_Temp(): 
	 receiver_string = receiver_data #received_data used to represent the data received from receiver. It is hardcoded
	 metrics = receiver_string.split(",") #split using the ',' delimiter 
	 temp = metrics[0]  #first value will always be temp
	 float_temp = float(temp) #converted to float for storage in db
	 return float_temp
	 
def get_pumpSpeed(): 
	 receiver_string = receiver_data #received_data used to represent the data received from receiver. It is hardcoded
	 metrics = receiver_string.split(",") #split using the ',' delimiter
	 pump = metrics[1]    #second value will always be pump
	 float_pump = float(pump) #converted to float for storage in db
	 return float_pump
	
def get_birdSensed(): 
	 receiver_string = receiver_data #received_data used to represent the data received from receiver. It is hardcoded
	 metrics = receiver_string.split(",") #split using the ',' delimiter
	 birdSensed = metrics[2]  #third value is whether a bird was sensed 
	 int_sensed = int(birdSensed) #converted to int for storage in db
	 return int_sensed
	 
def get_date():  
	 receiver_string = receiver_data #received_data used to represent the data received from receiver. It is hardcoded
	 metrics = receiver_string.split(",") #split using the ',' delimiter
	 date = metrics[3]  #4th value is the time the metrics were recorded
	 return date           #returns date in string format