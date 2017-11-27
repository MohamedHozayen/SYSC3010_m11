import json
import socket

port = 5001
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

sock.bind(("",port))

while True:
    received = sock.recvfrom(1024)
    json_obj = json.loads(received[0].decode('utf-8'))
    print(json_obj)
    
