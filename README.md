Transmitting Data received from Arduino to second RPI
======================================================
To obtain data from the Arduino first download the following script: 

https://github.com/MohamedHozayen/SYSC3010_m11/blob/umar/read_arduino.py

Now connect the Arduino to the RPI. Exceptions may occur when running the script due to permission denials on the usb port. To overcome this, type in the terminal:

Sudo chmod 777 /dev/X

Where x represents the serial port;e.g-/ttyACM0

Now that data can be read, transmitting it is the next step. Follow the instructions from this technical memo:

https://github.com/MohamedHozayen/SYSC3010_m11/blob/umar/Technical%20memo-GITGUB.docx

We will be using a modified version of RFSniffer.cpp for this project which will take the data received and transmit it. Download the modified version from this link and replace the RFSniffer:

https://github.com/MohamedHozayen/SYSC3010_m11/blob/umar/RFSniffer.cpp 

To run the communication script, in the terminal type:

Python read_arduino.py

NOTE: Make sure the python script is in the same directory as codesend.cpp which is mentioned in the technical memo.

You have now transmitted data between two RPIS read from an Arduino.
