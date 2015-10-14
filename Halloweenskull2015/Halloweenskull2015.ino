/*
  Halloween 2015 Animatronic Skull Candy Jar
  by Andrew Terranova
  Version 1.0

  BE SURE TO CALIBRATE YOUR SERVOS AND ADJUST THE FOLLOWING VARIABLES FOR THE SERVO MAX/MIN POSITIONS:
    liddown (variable to store the lid down postition)
    lidup (variable to store the lid open position)
    jawclosed (variable to store the jaw closed position)
    jawopen (variable to store the jaw open position)
*/

#define DEBUG 1  // set to 1 to print to serial monitor, 0 to disable

#include <Servo.h>

Servo lidservo;  // creates the servo to control the skull's lid
Servo jawservo;  //  create the servo to control the skull's jaw

// Define pins
// LED pins
int eyetopred = 2;
int eyerightred = 3;
int eyebottomred = 4;
int eyeleftred = 5;
int eyecenterred = 6;
int eyetopblue = 7;
int eyerightblue = 8;
int eyebottomblue = 9;
int eyeleftblue = 10;
int eyecenterblue = 11;
// Servo pins
int jawpin = 12; // define the pin for the jaw servo
int lidpin = 13;  // defines the pin for the lid servo
// Sound module pin
int soundpin = 14;  // defines the pin for the sound module; using A0 referenced as digital pin 14
//  PIR sensor pin
int sensorpin = 15;  // defines the pin for the PIR sensor input; using A1 referenced as digital pin 15

// Define variables
int color = 0;  // 0=blue; 1=red; 2=purple
int eyedir = 4; // 0=up; 1=down; 2=right; 3=left; 4=center
int lidpos = 0;  // variable to store the lid servo position
int liddown = 55;  // variable to store the lid down postition
int lidup = 9;  // variable to store the lid open position
int jawpos = 0;  // variable to store the jaw servo position
int jawclosed = 80;  // variable to store the jaw closed position
int jawopen = 10;  // variable to store the jaw open position
int sensorval; // variable to read the value from the PIR sensor
int behavior = 0;           // initialize variable for random behavior

// Timing stuff
long previousblink = 0;     // keep track of previous millis value for timing blinks
long previousdo = 0;        // keep track of previous millis value for timing behaviors
long blinkinterval = 3000;  // initialize blink interval; this is randomized later
long dointerval = 2000;     // initialize the interval for activity duration; randomized later

// the setup function runs once when you press reset or power the board
void setup() {

  Serial.begin(9600);
  Serial.println("Monitor");

  // initialize digital pins 2 to 14 as outputs.
  for (int i = 2; i <= 14; i += 1)
  {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }

  // initialize digital pin 15 as input
  pinMode(15, INPUT);

  // Initialize and test LEDs
  alloff();  // turn the LEDs off
  allonblue(); // turn on the blue LEDs
  delay(500);
  alloff();
  allonred();  // turn on the red LEDs
  delay(500);
  allon();  // turn on both red and blue (purple)
  delay(500);
  alloff();

  // Set up the servos
  // Attach servos
  lidservo.attach(lidpin);   // attach the lid servo to a pin
  jawservo.attach(jawpin);  //  attach the jaw servo to a pin
  // Initialize servos
  jawservo.write(jawclosed);
  lidservo.write(liddown);
  delay(500);  // wait so the servos can get into position
  // Detach servos so they won't be noisy
  jawservo.detach();  //  detach the jaw servo
  lidservo.detach();   // detach the lid servo

  // Randomization stuff
  randomSeed(analogRead(1));  // random seed for randomizing stuff

  // Initialize the sound module pin
  digitalWrite(soundpin, HIGH);  // initialize HIGH; sound module triggers on LOW
}

// the loop function runs over and over again forever
void loop() {

  unsigned long currentMillis = millis();

  if (currentMillis - previousblink > blinkinterval) {
    previousblink = currentMillis;
    blinkinterval = random(3000, 12000);
    color = 0;   // 0=blue; 1=red; 2=purple
    blink(color);
  }

  sensorval = digitalRead(sensorpin);  // Read the value of the PIR sensor
  if (DEBUG) {
    Serial.print("Sensor Val = ");
    Serial.println(sensorval);
  }

  if (sensorval == HIGH) {
    color = 1;  // select LED color red
    look(color, 4); // Look center
    speak();
    lidopen();
    delay(5000);  // wait a minute
    color = 2;  // select LED color purple

    // flash purple warning LEDs before shutting the lid
    int x = 0;  // use to reduce the delay each loop.
    for (int i = 0; i <= 6; i += 1)
    {
      allon();
      x = x + 100; // add 100 to the x-factor
      delay(1000 - x);
      alloff();
      delay(1000 - x);
    }
    lidclose();
    delay(5000);  // make sure the lid is closed so it doesn't trigger the sensor.
  }
  else {

    // Pick a random behavior

    if (currentMillis - previousdo > dointerval) {
      previousdo = currentMillis;
      dointerval = random(2000, 10000);

      behavior = random(0, 8);  // pick a random number from 0 to 7
      switch (behavior) {
        case 0:      // Look Left Blue
          look(0, 3);
          break;
        case 1:      // Look Right Blue
          look(0, 2);
          break;
        case 2:      // Look Up Blue
          look(0, 0);
          break;
        case 3:      // Look Down Blue
          look(0, 1);
          break;
        case 4:      // Look Center Blue
          look(0, 4);
          break;
        case 5:      // Eye Roll Blue
          eyeroll(0);
          eyeroll(0);
          eyeroll(0);
          break;
        case 6:      // Eye Left-Center-Right-Center Blue
          // dir 0=up; 1=down; 2=right; 3=left; 4=center
          look(0, 4);
          delay(1500);
          look(0, 3);
          delay(1500);
          look(0, 4);
          delay(1500);
          look(0, 2);
          delay(1500);
          look(0, 4);
          delay(2000);
          break;
        case 7:      // Eye Left-Right-Up-Down Blue
          // dir 0=up; 1=down; 2=right; 3=left; 4=center
          look(0, 4);
          delay(1500);
          look(0, 3);
          delay(1500);
          look(0, 2);
          delay(1500);
          look(0, 0);
          delay(1500);
          look(0, 1);
          delay(1500);
          look(0, 4);
          delay(2000);
          break;
        default:     // Look Blue Center is default
          if (DEBUG) {
            Serial.println("Default Case - BAD!!");
          }
          look(0, 4);
      }
    }
  }
}

int blink(int color) {
  if (DEBUG) {
    Serial.println("BLINK");
    Serial.print("Color = ");
    Serial.println(color);
  }

  alloff();
  delay(50);
  switch (color) {
    case 0:  // blue
      digitalWrite(eyecenterblue, HIGH);   // turn the LED on
      break;
    case 1:  // red
      digitalWrite(eyecenterred, HIGH);   // turn the LED on
      break;
    case 2:  // purple
      digitalWrite(eyecenterblue, HIGH);   // turn the LED on
      digitalWrite(eyecenterred, HIGH);   // turn the LED on
      break;
    default:  // default is blue
      digitalWrite(eyecenterblue, HIGH);   // turn the LED on
  }
}

int look(int color, int dir) {
  if (DEBUG) {
    Serial.println("LOOK");
    Serial.print("Color (0=blue; 1=red; 2=purple)= ");
    Serial.println(color);
    Serial.print("Direction (0=up; 1=down; 2=right; 3=left; 4=center)= ");
    Serial.println(dir);
  }

  alloff();
  //  delay(50);
  switch (color) {
    case 0:  // blue
      allonblue();
      break;
    case 1:  // red
      allonred();
      break;
    case 2:  // purple
      allon();
      break;
    default:  // default is blue
      allonblue();
  }

  switch (dir) {
    case 0:  // up
      digitalWrite(eyecenterblue, LOW);   // turn the LED off
      digitalWrite(eyecenterred, LOW);   // turn the LED off
      digitalWrite(eyebottomblue, LOW);   // turn the LED off
      digitalWrite(eyebottomred, LOW);   // turn the LED off
      digitalWrite(eyerightblue, LOW);   // turn the LED off
      digitalWrite(eyerightred, LOW);   // turn the LED off
      digitalWrite(eyeleftblue, LOW);   // turn the LED off
      digitalWrite(eyeleftred, LOW);   // turn the LED off
      break;
    case 1:  // down
      digitalWrite(eyecenterblue, LOW);   // turn the LED off
      digitalWrite(eyecenterred, LOW);   // turn the LED off
      digitalWrite(eyetopblue, LOW);   // turn the LED off
      digitalWrite(eyetopred, LOW);   // turn the LED off
      digitalWrite(eyerightblue, LOW);   // turn the LED off
      digitalWrite(eyerightred, LOW);   // turn the LED off
      digitalWrite(eyeleftblue, LOW);   // turn the LED off
      digitalWrite(eyeleftred, LOW);   // turn the LED off
      break;
    case 2:  // right
      digitalWrite(eyecenterblue, LOW);   // turn the LED off
      digitalWrite(eyecenterred, LOW);   // turn the LED off
      digitalWrite(eyetopblue, LOW);   // turn the LED off
      digitalWrite(eyetopred, LOW);   // turn the LED off
      digitalWrite(eyebottomblue, LOW);   // turn the LED off
      digitalWrite(eyebottomred, LOW);   // turn the LED off
      digitalWrite(eyeleftblue, LOW);   // turn the LED off
      digitalWrite(eyeleftred, LOW);   // turn the LED off
      break;
    case 3:  // left
      digitalWrite(eyecenterblue, LOW);   // turn the LED off
      digitalWrite(eyecenterred, LOW);   // turn the LED off
      digitalWrite(eyetopblue, LOW);   // turn the LED off
      digitalWrite(eyetopred, LOW);   // turn the LED off
      digitalWrite(eyebottomblue, LOW);   // turn the LED off
      digitalWrite(eyebottomred, LOW);   // turn the LED off
      digitalWrite(eyerightblue, LOW);  // turn the LED off
      digitalWrite(eyerightred, LOW);   // turn the LED off
      break;
    case 4:  // center
      digitalWrite(eyetopblue, LOW);   // turn the LED off
      digitalWrite(eyetopred, LOW);   // turn the LED off
      digitalWrite(eyebottomblue, LOW);   // turn the LED off
      digitalWrite(eyebottomred, LOW);   // turn the LED off
      digitalWrite(eyeleftblue, LOW);   // turn the LED off
      digitalWrite(eyeleftred, LOW);   // turn the LED off
      digitalWrite(eyerightblue, LOW);   // turn the LED off
      digitalWrite(eyerightred, LOW);   // turn the LED off
      break;
  }
}

int eyeroll(int color) {
  if (DEBUG) {
    Serial.println("EYE ROLL");
    Serial.print("Color (0=blue; 1=red; 2=purple)= ");
    Serial.println(color);
  }
  int wait = 300;

  alloff();
  switch (color) {
    case 0:  // blue
      digitalWrite(eyerightblue, HIGH);
      delay(wait);
      digitalWrite(eyerightblue, LOW);
      digitalWrite(eyebottomblue, HIGH);
      delay(wait);
      digitalWrite(eyebottomblue, LOW);
      digitalWrite(eyeleftblue, HIGH);
      delay(wait);
      digitalWrite(eyeleftblue, LOW);
      digitalWrite(eyetopblue, HIGH);
      delay(wait);
      alloff();
      break;
    case 1:  // red
      digitalWrite(eyerightred, HIGH);
      delay(wait);
      digitalWrite(eyerightred, LOW);
      digitalWrite(eyebottomred, HIGH);
      delay(wait);
      digitalWrite(eyebottomred, LOW);
      digitalWrite(eyeleftred, HIGH);
      delay(wait);
      digitalWrite(eyeleftred, LOW);
      digitalWrite(eyetopred, HIGH);
      delay(wait);
      alloff();
      break;
    case 2:  // purple
      digitalWrite(eyerightred, HIGH);
      digitalWrite(eyerightblue, HIGH);
      delay(wait);
      digitalWrite(eyerightred, LOW);
      digitalWrite(eyebottomred, HIGH);
      digitalWrite(eyerightblue, LOW);
      digitalWrite(eyebottomblue, HIGH);
      delay(wait);
      digitalWrite(eyebottomred, LOW);
      digitalWrite(eyeleftred, HIGH);
      digitalWrite(eyebottomblue, LOW);
      digitalWrite(eyeleftblue, HIGH);
      delay(wait);
      digitalWrite(eyeleftred, LOW);
      digitalWrite(eyetopred, HIGH);
      digitalWrite(eyeleftblue, LOW);
      digitalWrite(eyetopblue, HIGH);
      delay(wait);
      alloff();
      break;
    default:  // default is blue
      digitalWrite(eyerightblue, HIGH);
      delay(wait);
      digitalWrite(eyerightblue, LOW);
      digitalWrite(eyebottomblue, HIGH);
      delay(wait);
      digitalWrite(eyebottomblue, LOW);
      digitalWrite(eyeleftblue, HIGH);
      delay(wait);
      digitalWrite(eyeleftblue, LOW);
      digitalWrite(eyetopblue, HIGH);
      delay(wait);
      alloff();
  }
}


int lookup(int color) {
  if (DEBUG) {
    Serial.println("LOOKUP");
    Serial.print("Color = ");
    Serial.println(color);
  }

  alloff();
  delay(50);
  switch (color) {
    case 0:  // blue
      digitalWrite(eyetopblue, HIGH);   // turn the LED on (HIGH is the voltage level)
      digitalWrite(eyerightblue, HIGH);   // turn the LED on (HIGH is the voltage level)
      digitalWrite(eyeleftblue, HIGH);   // turn the LED on (HIGH is the voltage level)
      digitalWrite(eyecenterblue, HIGH);   // turn the LED on (HIGH is the voltage level)
      break;
    case 1:  // red
      digitalWrite(eyetopred, HIGH);   // turn the LED on (HIGH is the voltage level)
      digitalWrite(eyerightred, HIGH);   // turn the LED on (HIGH is the voltage level)
      digitalWrite(eyeleftred, HIGH);   // turn the LED on (HIGH is the voltage level)
      digitalWrite(eyecenterred, HIGH);   // turn the LED on (HIGH is the voltage level)
      break;
    case 2:  // purple
      digitalWrite(eyetopblue, HIGH);   // turn the LED on (HIGH is the voltage level)
      digitalWrite(eyerightblue, HIGH);   // turn the LED on (HIGH is the voltage level)
      digitalWrite(eyeleftblue, HIGH);   // turn the LED on (HIGH is the voltage level)
      digitalWrite(eyecenterblue, HIGH);   // turn the LED on (HIGH is the voltage level)
      digitalWrite(eyetopred, HIGH);   // turn the LED on (HIGH is the voltage level)
      digitalWrite(eyerightred, HIGH);   // turn the LED on (HIGH is the voltage level)
      digitalWrite(eyeleftred, HIGH);   // turn the LED on (HIGH is the voltage level)
      digitalWrite(eyecenterred, HIGH);   // turn the LED on (HIGH is the voltage level)
      break;
    default:  // default is blue
      digitalWrite(eyetopblue, HIGH);   // turn the LED on (HIGH is the voltage level)
      digitalWrite(eyerightblue, HIGH);   // turn the LED on (HIGH is the voltage level)
      digitalWrite(eyeleftblue, HIGH);   // turn the LED on (HIGH is the voltage level)
      digitalWrite(eyecenterblue, HIGH);   // turn the LED on (HIGH is the voltage level)
  }
}

void alloff() {
  // all off
  digitalWrite(eyetopred, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(eyerightred, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(eyebottomred, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(eyeleftred, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(eyecenterred, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(eyetopblue, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(eyerightblue, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(eyebottomblue, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(eyeleftblue, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(eyecenterblue, LOW);    // turn the LED off by making the voltage LOW

}

void allon() {
  digitalWrite(eyetopred, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(eyerightred, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(eyebottomred, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(eyeleftred, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(eyecenterred, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(eyetopblue, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(eyerightblue, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(eyebottomblue, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(eyeleftblue, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(eyecenterblue, HIGH);   // turn the LED on (HIGH is the voltage level)
}

void allonred() {
  digitalWrite(eyetopred, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(eyerightred, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(eyebottomred, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(eyeleftred, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(eyecenterred, HIGH);   // turn the LED on (HIGH is the voltage level)
}

void allonblue() {
  digitalWrite(eyetopblue, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(eyerightblue, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(eyebottomblue, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(eyeleftblue, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(eyecenterblue, HIGH);   // turn the LED on (HIGH is the voltage level)
}

void lidopen() {
  if (DEBUG) {
    Serial.println("Lid Open");
  }

  // Attach the servo
  lidservo.attach(lidpin);   // attach the lid servo to a pin
  delay(10);
  for (lidpos = liddown; lidpos >= lidup; lidpos -= 1)
  {
    lidservo.write(lidpos);
    delay(25);
  }
  // Detach the servo so it won't be noisy
  lidservo.detach();   // detach the lid servo
  delay(10);
}

void lidclose() {
  if (DEBUG) {
    Serial.println("Lid Close");
  }

  // Attach the servo
  lidservo.attach(lidpin);   // attach the lid servo to a pin

  for (lidpos = lidup; lidpos <= liddown; lidpos += 1)
  {
    lidservo.write(lidpos);
    delay(25);
  }
  // Detach the servo so it won't be noisy
  lidservo.detach();   // detach the lid servo
  delay(10);
}

void mouthopen() {
  if (DEBUG) {
    Serial.println("Mouth Open");
  }

  // Attach the servo
  jawservo.attach(jawpin);   // attach the jaw servo to a pin

  for (jawpos = jawclosed; jawpos >= jawopen; jawpos -= 1)
  {
    jawservo.write(jawpos);
    delay(25);
  }
  // Detach the servo so it won't be noisy
  delay(100);
  jawservo.detach();   // detach the jaw servo
  delay(10);
}

void mouthclosed() {
  if (DEBUG) {
    Serial.println("Mouth Closed");
  }

  // Attach the servo
  jawservo.attach(jawpin);   // attach the jaw servo to a pin

  for (jawpos = jawopen; jawpos <= jawclosed; jawpos += 1)
  {
    jawservo.write(jawpos);
    delay(25);
  }
  // Detach the servo so it won't be noisy
  delay(100);
  jawservo.detach();   // detach the jaw servo
  delay(10);
}

// The speak() function triggers the sound module and moves the jaw; you have to try to match the jaw motions to the speaking.
void speak() {
  if (DEBUG) {
    Serial.println("Speak");
  }
  digitalWrite(soundpin, LOW);

  jawservo.attach(jawpin);  // attach the jaw servo to a pin

  // Ha ha ha ha ha ha
  Serial.println("Ha ha ha ha ha ha!");
  for (int i = 1; i <= 5; i += 1)
  {
    jawservo.write(jawopen);
    delay(200);
    jawservo.write(jawclosed);
    delay(200);
  }
  jawservo.write(jawopen);
  delay(400);
  jawservo.write(jawclosed);
  delay(200);


  // Take one.
  Serial.println("Take one.");
  delay(500);
  for (int i = 1; i <= 2; i += 1)
  {
    jawservo.write(jawopen + 10);
    delay(350);
    jawservo.write(jawclosed - 10);
    delay(350);
  }

  // Happy Halloween.
  Serial.println("Happy Halloween!");
  delay(1200);
  for (int i = 1; i <= 5; i += 1)
  {
    jawservo.write(jawopen + 30);
    delay(150);
    jawservo.write(jawclosed - 10);
    delay(150);
  }
  jawservo.write(jawclosed);
  delay(100);

  jawservo.detach();   // detach the jaw servo

  delay(2000);
  digitalWrite(soundpin, HIGH);

}
