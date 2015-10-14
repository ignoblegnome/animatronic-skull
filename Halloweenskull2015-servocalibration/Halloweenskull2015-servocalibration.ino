/* 
 Controlling a servo position using a potentiometer (variable resistor) 
 by Michal Rinott <http://people.interaction-ivrea.it/m.rinott> 

 modified on 8 Nov 2013
 by Scott Fitzgerald
 http://arduino.cc/en/Tutorial/Knob

  modified on 14 Oct 2015
  by Andrew Terranova
  -Added a Serial monitor to report on potentiometer value.
  
  Use a potentiometer with the center contact connected to analog A4, one contact to GND, and one contact to 5V.
  Set the potentiomter at about mid-sweep through its range before powering on the Arduino.
  Connect and test the servos one at a time: the jaw servo to pin 12 and the lid servo to pin 13.
*/

#include <Servo.h>

Servo jawservo;  // create servo object to control a servo
Servo lidservo;  // create servo object to control a servo

int potpin = 4;  // analog pin used to connect the potentiometer
int val;    // variable to read the value from the analog pin
int jawpin = 12; // define the pin for the jaw servo
int lidpin = 13;  // defines the pin for the lid servo

void setup()
{
  Serial.begin(9600);
  Serial.println("Monitor");
  
  jawservo.attach(jawpin);  // attach the servo on pin 12 for the jaw servo
  lidservo.attach(lidpin);  // attach the servo on pin 12 for the jaw servo
}

void loop() 
{ 
  val = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023) 
  val = map(val, 0, 1023, 0, 180);     // scale it to use it with the servo (value between 0 and 180) 
  Serial.println(val);
  jawservo.write(val);                  // sets the servo position according to the scaled value 
  lidservo.write(val);                  // sets the servo position according to the scaled value 
  delay(15);                           // waits for the servo to get there 
} 

