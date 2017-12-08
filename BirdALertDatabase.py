import mysql.connector
from time import sleep
user = 'root' #username of database
host = 'localhost' #Where the database is stored
database = 'BirdALert' #Name of database
password = 'mohamed01' #password for database
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


def insert_into_db(temp, flowSpeed, flowML, Sensed, date):
    temperature = "'" + temp+ "'"      
    fSpeed =   "'" + flowSpeed + "'"
    flowMili = "'" + flowML + "'"
    birdSensed = "'" + Sensed+ "'"
    stringDate = "'" + date+ "'"
    cnx = mysql.connector.connect(user, password,host,database)#establish a connection to the database
    curs = cnx.cursor() #create cursor for executing sql statements
    try:
        curs.execute("INSERT INTO metrics VALUES (%s,%s,%s,%s,%s)" % (temperature,fSpeed,flowMili,birdSensed,stringDate)) #insert string values into the table called metrics
        cnx.commit() #commit the changes
        curs.close()#close cursor
        cnx.close()#close connection

    except:
        cnx.rollback() #if there was an error with the insert, rollback changes
        curs.close()#close cursor
        cnx.close()#close connection

def main():
    while True:
        metrics = read_receiver() #get metrics
       
        if(len(metrics)!=5): #metrics must be an array with length 5. This line is used to ignore incomplete information from the rf receiver
             continue
        temp= get_Temp(metrics)
        flowSpeed = get_FlowSpeed(metrics)
        flowML = get_FlowML(metrics)
        birdSense = get_birdSensed(metrics)
        date = get_date(metrics)

        
        if(int(birdSense) >1): #this line is used to ignore garbage values received by the rf receiver
            continue
       
        insert_into_db(temp,flowSpeed,flowML,birdSense,date) #insert metrics into the database
        sleep(30) #insert into database every 30 seconds. Up to the user to decide how oftern database should be updated
        
        
          
    
if __name__ == "__main__": main()   
