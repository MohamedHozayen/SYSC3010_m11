/* 
Example Timer1 Interrupt
Flash LED every second
*/
#include <stdio.h>

#define ledPin 13
int timer1_counter;
int minRange = 50;
int MaxRange = 500;
boolean Human = false;// no human on the raft
void setup()
{
  pinMode(ledPin, OUTPUT);

  // We'll use the serial monitor to view the sensor output
  Serial.begin(9600);
  
  // initialize timer1 
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;

  // Set timer1_counter to the correct value for our interrupt interval
  //timer1_counter = 64911;   // preload timer 65536-16MHz/256/100Hz
  //timer1_counter = 64286;   // preload timer 65536-16MHz/256/50Hz
  //timer1_counter = 34286;   // preload timer 65536-16MHz/256/2Hz
    timer1_counter = 286;
  
  TCNT1 = timer1_counter;   // preload timer
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TCCR2B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
  interrupts();             // enable all interrupts
}

ISR(TIMER1_OVF_vect)        // interrupt service routine 
{
  TCNT1 = timer1_counter;   // preload timer
  digitalWrite(ledPin, digitalRead(ledPin) ^ 1);
  if(!isHuman() || DisableButton()){
     Human = false;
  }
}

/**
 * Stubbing program that simulates an ultrasound sensor
 * data. These simulated data are being used to test the
 * software bahvoiur 
 * 
*/
void loop()
{
    
    int ultraStub [500];//initialize and fill array of ultrasound stub values 
    createStub(ultraStub);

    /*
    In reality this would be an infinite while loop
    the arduino will constantly keep checking for object
    on the raft and if they are within a certain range, the 
    waterpump and the buzzer would be turned on.
    */
    int i = 100;
    while(i != 0){
        i--;
        int index = rand()%500;
        int ultraValue = ultraStub[index];
        if(ultraValue > minRange && ultraValue < MaxRange && !Human){
            Serial.print("An object is sensed at -> ");
            Serial.print(ultraValue);
            Serial.println(" cm");
            Serial.println(" turn on waterpump and buzzer");
        }
        delay(100);
    }
  delay(1000000);
}


/*
 createStub simultes random data
 for an ultrasound sensor
*/
void createStub(int ultraStub[]){
    int i;
    for(i=0; i<500; i++){
    ultraStub[i] = rand()%1000;
    }
}

boolean isHuman(){
  /*
   * check xyz axis of IMU for the raft
   * if there is a significant change 
   * that means a human just climbed the raft
   * 
   */
   return false;
}

boolean DisableButton(){
 /* 
  *  disable button to disable splashing
  *  birds by the user
  */
  return true;
}






