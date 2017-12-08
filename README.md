# SYSC3010_m11
BIRD-A-LERT Android App

The app was made to work in conjuction with Bird-A-Lert project.
Its purpose is to constantly receive JSON string with measurements from the server over UDP, parse it and display values in UI in real time.


MainActivity.java class is the main class in this application. Metrics.java is used to parse received values of measurements into fields of this class.
DisplayMessageActivity.java is the class for requesting measurements history. This class is not used because the team decided not to utilize http requests but it is partly implemented. See the commented sections of code.

The code was written in Android Studio and the whole project folder is located in this branch. (Excuse the lame name for the folder "MyFirstApp"... But it was the first app indeed)
Just open the project folder and run the app.
The UI was made to fit Google Pixel smartphone and was tested on this device as well as in Genymotion emulator.

Libraries used:
Gson
Volley (for DisplayMessageActivity)

Author: Vadim Yastrebov
