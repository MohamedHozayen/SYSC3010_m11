/**
   BIRDALERT

   Arduino system that detects water status and protect rafts from birds dirts

   Devices and sensor used:
   1. Digital temperatur sensor
   2. Water flow sensor
   3. Double ultrasound
   4. Single ultrasound
   5. Buzzer
   6. Additional on a seprate Arduino
      a) motor shield
      b) water pumper motor
      c) Lead-Acid Battery

   Functionality:
   Read data from all different sensors and send throught the serial port to an RPI

    @author Mohamed Hozayen mohamedhozayen@sce.carleton.ca
    @version 27.11.2017
    GIT URL: https://github.com/MohamedHozayen/SYSC3010_m11
*/


/********************************************************************/
/**                  Buzzer                       */
int BuzzerPin = 52;
int BuzzerFreq = 1900;
int BuzzerDuration = 10000; //miliseconds
/********************************************************************/
/**                  Digital Temperatur Sensor                      */
// First we include the libraries
#include <OneWire.h>
#include <DallasTemperature.h>
// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 4
// Setup a oneWire instance to communicate with any OneWire devices
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
/********************************************************************/
/*                   Double cups Ultrasound sensor                       */
/**
   Description:
    Connect the ultrasonic sensor to the Arduino as per the
    hardware connections below. Run the sketch and open a serial
    monitor. The distance read from the sensor will be displayed
    in centimeters and inches.

   Hardware Connections:
    Arduino | HC-SR04
    -------------------
      5V    |   VCC
      7     |   Trig
      8     |   Echo
      GND   |   GND
*/

// Pins
const int TRIG_PIN = 7;
const int ECHO_PIN = 8;

// Anything over 400 cm (23200 us pulse) is "out of range"
const unsigned int MAX_DIST = 23200;
/********************************************************************/
/**                 Single Ultrasound Sensor                      */
int singleUltra = 3;
/********************************************************************/
/**                 Water Flow Sensor                      */
/*
  Liquid flow rate sensor -DIYhacking.com Arvind Sanjeev

  Measure the liquid/water flow rate using this code.
  Connect Vcc and Gnd of sensor to arduino, and the
  signal line to arduino digital pin 2.

*/
// The Hall-effect sensor is connected to pin 2 which uses interrupt 0.
// Configured to trigger on a FALLING state change (transition from HIGH
// state to LOW state)
byte sensorInterrupt = 0;  // 0 = digital pin 2
byte sensorPin       = 2;

// The hall-effect flow sensor outputs approximately 4.5 pulses per second per
// litre/minute of flow.
float calibrationFactor = 4.5;

volatile byte pulseCount = 0;

float flowRate = 0.0;
unsigned int flowMilliLitres = 0;
unsigned long totalMilliLitres = 0;

unsigned long oldTime = 0;
/********************************************************************/
/*       logic to decide if motor should turn on                          */
#include <stdio.h>
#define pin13 13
int timer1_counter;

int minRange = 25;
int MaxRange = 80;                                                              //RANGE HERE

boolean Human = false;// no human on the raft

boolean buzzer = false;
/********************************************************************/
int motorPin = 9;
boolean mpin = false;

int testPin = 10;

/*
   setup for all pins and interrupts
   this subroutine runs only once at the begining of execution
*/
void setup() {
  Serial.begin(9600);
  /* digital temperature */
  sensors.begin();
  /* buzzer */
  pinMode(BuzzerPin, OUTPUT);
  /* double ultra sound */
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
  /*single ultra sound   */
  pinMode(singleUltra, OUTPUT);
  /*  waterflow sensor  */
  // Set up the status LED line as an output
  pinMode(sensorPin, INPUT);
  digitalWrite(sensorPin, HIGH);
  attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
  /*Time Interrupt setup*/
  pinMode(pin13, OUTPUT);//check LED
  // initialize timer1
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;

  // Set timer1_counter to the correct value for our interrupt interval
  timer1_counter = 3036;    //65536 - 16MHZ/256/1Hz interrupt once every second

  TCNT1 = timer1_counter;   // preload timer
  TCCR1B |= (1 << CS12);    // 256 prescaler
  TCCR2B |= (1 << CS12);    // 256 prescaler
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
  interrupts();             // enable all interrupts
  /* End of interrupt setipup*/
  pinMode(motorPin, OUTPUT);
  digitalWrite(motorPin, LOW);
  pinMode(testPin, INPUT);
}

/*
   always loop to read data off sensors and detect birds

   if a bird is detected set a pin high to the motor shield
*/
void loop() {
  digitalTemp();//if -127 is return; fail read
  waterflow();

  float range1;
  range1 = doubleUltra();
  //Serial.print("range1 ");
  //Serial.println(range1);
  float range2;
  range2 = singleUltrasound();
  //Serial.print("range2 ");
  //Serial.println(range2);

  if ((range2 > minRange && range2 < MaxRange && !Human)
      || range2 > minRange && range2 < MaxRange && !Human  ) {
    buzzerON();
    digitalWrite(motorPin, HIGH);
    mpin = true;
    //Serial.println("An object is sensed, turning on waterpump and buzzer");
    Serial.println("S:1");
    delay(5000);//keep buzzer on for 3 seconds - just for experiment
    //buzzerOff();
    //turn off motor pin using interrupts
  } else {
    Serial.println("S:0");
    delay(5000);
    digitalWrite(motorPin, LOW);
  }
  //  if(!buzzer){
  //    //if buzzer is off, set motorpin to low
  //    //don't know why digitalWrite doesn't work in interrupts
  //    digitalWrite(motorPin, LOW);
  //  }
//  if (mpin == false) {
//    digitalWrite(motorPin, LOW);
//  }
  //delay(1000);
  //Serial.print(digitalR
  //Serial.println();ead(testPin));
}

/*
   interrupt service routine to check if a human climbed the raft
   or the disabled button is pressed
*/
int sec = 0;
ISR(TIMER1_OVF_vect)        // interrupt service routine
{
  TCNT1 = timer1_counter;   // preload timer
  digitalWrite(pin13, digitalRead(pin13) ^ 1);//testing purposes
  if (!isHuman() || !DisableButton()) {
    //no human
    Human = false;
  } else {
    Human = true;
  }
  if (buzzer && sec > 5) {
    buzzerOff();
    //digitalWrite(motorPin, LOW);
    mpin = false;
    sec = 0;
  }
}

/*
   IMU sensor determines if a human is on the raft
   by looking at the change in xyz
*/
boolean isHuman() {
  /*
     check xyz axis of IMU for the raft
     if there is a significant change
     that means a human just climbed the raft

  */
  return false;
}

/*
   Global disable button for the water pumper
*/
boolean DisableButton() {
  /*
      disable button to disable splashing
      birds by the user
  */
  return false;
}

/*
   Turn buzzor on
*/
void buzzerON() {
  /*Tone needs 2 arguments, but can take three
    1) Pin#
    2) Frequency - this is in hertz (cycles per second) which determines the pitch of the noise made
    3) Duration - how long teh tone plays
  */
  buzzer = true;
  tone(BuzzerPin, BuzzerFreq, BuzzerDuration);
}

/*
   Turn buzzer off
*/
void buzzerOff() {
  buzzer = false;
  noTone(BuzzerPin);
}

/*
   Read and send the water temperature
*/
void digitalTemp() {
  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  /********************************************************************/
  //Serial.print(" Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperature readings
  //Serial.println("DONE");
  /********************************************************************/
  //Serial.print("Temperature is: ");
  Serial.print("T:");
  int value = sensors.getTempCByIndex(0);
  Serial.print(value);
  Serial.print(" ");
}

/*
   Get double ultrasound readings
*/
float doubleUltra() {
  unsigned long t1;
  unsigned long t2;
  unsigned long pulse_width;
  float cm;

  // Hold the trigger pin high for at least 10 us
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Wait for pulse on echo pin
  while (digitalRead(ECHO_PIN) == 0 );

  // Measure how long the echo pin was held high (pulse width)
  // Note: the micros() counter will overflow after ~70 min
  t1 = micros();
  while ( digitalRead(ECHO_PIN) == 1);
  t2 = micros();
  pulse_width = t2 - t1;

  // Calculate distance in centimeters and inches. The constants
  // are found in the datasheet, and calculated from the assumed speed
  //of sound in air at sea level (~340 m/s).
  cm = pulse_width / 58.0;

  return cm;
}

/*
   Get single ultrasound sensor readings
*/
float singleUltrasound() {
  long pulse;
  float inches, cm;
  pinMode(singleUltra, INPUT);
  pulse = pulseIn(singleUltra, HIGH);
  inches = pulse / 147;//Used to read in the pulse that is being sent by the MaxSonar device.
  //Pulse Width representation with a scale factor of 147 uS per Inch.
  cm = inches * 2.54;

  return cm;
}

/*
   Read water flow sensor data and send it
*/
void waterflow()
{

  if ((millis() - oldTime) > 1000)   // Only process counters once per second
  {
    // Disable the interrupt while calculating flow rate and sending the value to
    // the host
    detachInterrupt(sensorInterrupt);

    // Because this loop may not complete in exactly 1 second intervals we calculate
    // the number of milliseconds that have passed since the last execution and use
    // that to scale the output. We also apply the calibrationFactor to scale the output
    // based on the number of pulses per second per units of measure (litres/minute in
    // this case) coming from the sensor.
    flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;

    // Note the time this processing pass was executed. Note that because we've
    // disabled interrupts the millis() function won't actually be incrementing right
    // at this point, but it will still return the value it was set to just before
    // interrupts went away.
    oldTime = millis();

    // Divide the flow rate in litres/minute by 60 to determine how many litres have
    // passed through the sensor in this 1 second interval, then multiply by 1000 to
    // convert to millilitres.
    flowMilliLitres = (flowRate / 60) * 1000;

    // Add the millilitres passed in this second to the cumulative total
    totalMilliLitres += flowMilliLitres;

    unsigned int frac;

    // Print the flow rate for this second in litres / minute
    //Serial.print("Flow rate: ");
    Serial.print("F:");
    Serial.print(int(flowRate));  // Print the integer part of the variable
    Serial.print(" ");
    //Serial.print(".");             // Print the decimal point
    // Determine the fractional part. The 10 multiplier gives us 1 decimal place.
    //frac = (flowRate - int(flowRate)) * 10;
    //Serial.print(frac, DEC) ;      // Print the fractional part of the variable
    //Serial.println("L/min");
    // Print the number of litres flowed in this second
    //Serial.print("Current Liquid Flowing: ");             // Output separator
    Serial.print("C:");
    Serial.print(flowMilliLitres);
    Serial.print(" ");
    //Serial.println("mL/Sec");

    // Reset the pulse counter so we can start incrementing again
    pulseCount = 0;

    // Enable the interrupt again now that we've finished sending output
    attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
  }
}

/*
  Insterrupt Service Routine
*/
void pulseCounter()
{
  // Increment the pulse counter
  pulseCount++;
}
