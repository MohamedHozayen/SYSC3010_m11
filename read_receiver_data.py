import mysql.connector
import json
import socket
UDP_IP = 'localhost'
PORT = 5001
def read_receiver():
     receiver_file=open("receiverFile.txt","r") 
     receiver_string = receiver_file.read()  #Receive metrics from rf receiver
     metrics = receiver_string.split(",")   #split by comma delimiter
     return metrics #return array
     

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

def insert_into_db(temp, pumpSpeed, Sensed, date):
    stringDate = "'" + date + "'"
    cnx = mysql.connector.connect(user='root', password='mohamed01',host='localhost',database = 'TestReceiver') #connect to the database
    curs = cnx.cursor() #assign a cursor
    try:
        curs.execute("INSERT INTO metrics VALUES (%f,%f,%i,%s)" % (temp,pumpSpeed,Sensed,stringDate)) #insert passed values into the database
        cnx.commit() #commit changes
        curs.close() #close cursor
        cnx.close()  #close connection

    except:
        cnx.rollback() #rollback if any errors happen with the insert
        curs.close()
        cnx.close()

def create_json(temp,pumpSpeed,birdSense,date): 
    pythonDictionary = {'temp':str(temp), 'pumpSpeed': str(pumpSpeed),'birdSensed':str(birdSense), 'date':date} #create a dictionary using the passed paramaters
    dictionaryToJson = json.dumps(pythonDictionary) #make the dictionary into a json object
    return dictionaryToJson #return json object
def main():
    while True:
        metrics = read_receiver()
        temp= get_Temp(metrics)
        pumpSpeed = get_pumpSpeed(metrics)
        birdSense = get_birdSensed(metrics)
        date = get_date(metrics)

        metricsJson = create_json(temp,pumpSpeed,birdSense,date)

        sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM) #initialize socket used from data transfer
        sock.sendto(bytes(metricsJson, 'UTF-8'),(UDP_IP,PORT)) #encode into bytes and send to specified address and port
        #insert_into_db(temp,pumpSpeed,birdSense,date)
                    
        #print ('The temp is %.2f' %temp)
        #print ('The pumpSpeed is %.2f' %pumpSpeed)
        #print ('The birdSensed value is %i' %birdSense)
        #print ('The date is %s' %date)
        #break
    
if __name__ == "__main__": main()   
