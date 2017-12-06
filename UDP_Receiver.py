import json
import socket

port = 5002 #port to receive information
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) #open socket

sock.bind(("",port)) #can receive from anyone

while True:
    received = sock.recvfrom(1024) #receive information
    json_obj = json.loads(received[0].decode('utf-8')) #decode it and retreive the json object
    print(json_obj) #print the json object 
    
