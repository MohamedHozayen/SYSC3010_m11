# Reading Data Received From RF Receiver and Sending it to the App and Database

To do this, Two python files are used. The first one is read_receiver_data.py which receives from the RF receiver and sends to the App using UDP. The second file is BirdALertDatabase.py which also reads the RF receiver data and sends to the app. 

To send to the App, run this file:
    https://github.com/MohamedHozayen/SYSC3010_m11/blob/osman/read_receiver_data.py
Before running it, there are some things to know
    1) The IP of the device receiving it must be inputted
    2) The Port being used to send and receive data must be the same for both receiver and sender
    3) Python 3 requires that data sent through the socket be encoded in bytes, so in the receiver code, the data must be decoded before          use

To test whether the data is being sent correctly, use the UDP_Receiver.py file below: 
    https://github.com/MohamedHozayen/SYSC3010_m11/blob/osman/UDP_Receiver.py
   
You have Successfully sent your data to the application through UDP

To Send to the Database, run the following file:
    https://github.com/MohamedHozayen/SYSC3010_m11/blob/osman/BirdALertDatabase.py

You have Successfully sent your data to the database


@author Mohamed Osman mohamedosman@cmail.carleton.ca
@date updated 27.11.2017
