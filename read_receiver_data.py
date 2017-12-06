import json
import socket
from time import sleep
UDP_IP = 'localhost'  #ip of device that is receiving                                                                                         
PORT = 5002 #port the information is being sent to
def read_receiver():
     receiver_file=open("/home/pi/433Utils/RPi_utils/BirdALert.txt","r") #open the file containing the info from rf receiver
     receiver_string = receiver_file.read() #read file
     receiver_file.close() #close it
     metrics = receiver_string.split(",") #separate data using delimiter, and put into array
     return metrics #return array containing the data
     

def get_Temp(metricsList): #this function receives the metrics array as a parameter
     temp = metricsList[0] #first index will always be temp
     return temp #return the temp
     
def get_FlowSpeed(metricsList): #this function receives the metrics array as a parameter
     flowSpeed = metricsList[1] #second index will always be the flowspeed of the water
     return flowSpeed #return the flowspeed

def get_FlowML(metricsList):#this function receives the metrics array as a parameter
     flowML = metricsList[2] #third index will always be the flowspeed in units ml/sec
     return flowML #return the flowspeed
    
def get_birdSensed(metricsList):#this function receives the metrics array as a parameter
     birdSensed = metricsList[3] #fourth index will always be whether a bird has been sensed or not
     return birdSensed #return whether the bird was sensed
     
def get_date(metricsList): #this function receives the metrics array as a parameter
     date = metricsList[4] #fifth index is always the date
     return date #return date of when metrics were recorded


#function used to create a JSON object to store the metrics 
def create_json(temp, flowSpeed, flowML, Sensed): #receives the metric values as parameters
    pythonDictionary = {'temp':str(temp), 'flowSpeed': str(flowSpeed), 'flowML':str(flowML),'birdSensed':str(Sensed)} #creates a python dictionary from the metrics
    dictionaryToJson = json.dumps(pythonDictionary) #dumps the dictionary into a json object
    return dictionaryToJson #returns json object
def main():
    while True:
        metrics = read_receiver()  #get metrics 
        if(len(metrics)!=5):  #metrics must be an array with length 5. This line is used to ignore incomplete information from the rf receiver
             continue
        temp= get_Temp(metrics) 
        flowSpeed = get_FlowSpeed(metrics)
        flowML = get_FlowML(metrics)
        birdSense = get_birdSensed(metrics)
        date = get_date(metrics)

        metricsJson = create_json(temp,flowSpeed,flowML,birdSense)  #json object is created
          
        if(int(birdSense) >1 or int(birdSense)< 0):  #this line is used to ignore garbage values received by the rf receiver
             continue
          
        sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM) #socket is opened
        sock.sendto(bytes(metricsJson, 'UTF-8'),(UDP_IP,PORT)) #information is encoded into bytes and is sent to the ip at provided port
        
          
    
if __name__ == "__main__": main()   
