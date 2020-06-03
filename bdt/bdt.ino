#include <Arduino.h>

#define motor_steps 3200
#define steps_int 3975 // steps interval in us ; 4,7046 RPM -> 250,912 steps per second or step every 3984us substract 2x Digital Read -> -9us -> 3975

// PINOUT:
#define DIRECTION 6
#define STEP 4
#define SLEEP 5
#define FWD 2
#define BACK 3

bool run_flag = 0;
int f = 0;
int b = 0;

void setup() {
// I/O setup
pinMode(LED_BUILTIN, OUTPUT);
pinMode(DIRECTION, OUTPUT);
pinMode(STEP, OUTPUT);
pinMode(SLEEP, OUTPUT);
pinMode(FWD, INPUT_PULLUP);
pinMode(BACK, INPUT_PULLUP);

digitalWrite(SLEEP, LOW); //set to sleep stepper driver
digitalWrite(STEP, LOW);
delayMicroseconds(1);
digitalWrite(LED_BUILTIN, LOW);
}

void move_steps(int steps, int dir, int interval){    // minimal interval value 6 , max 16390 us
  int i =0;
  digitalWrite(SLEEP, HIGH);                          //wake up stepper driver
  for( i = 1; i <= steps; i++ ){
  if (dir) digitalWrite(DIRECTION, HIGH); else digitalWrite(DIRECTION, LOW);    //check direction flag and set dirrection pin
  delayMicroseconds(1);       //changing dirrection require 1us delay
  digitalWrite(STEP, HIGH);   // send HIGH state to step pin for 1us
  delayMicroseconds(1);       //
  digitalWrite(STEP, LOW);    //send LOW state to step pin for 1us , next step is made on rising edge
  delayMicroseconds(interval-6);
  }
}

void loop() {

f=digitalRead(FWD);  //4,7us  DigitalRead once in a program loop instead twice at if statement
b=digitalRead(BACK); //4,7us

if (!f && !b) {               // presing both buttons for ~1s sets run_flag to 1 and starts tracker - led will blink twice
  run_flag = 1;
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(2000);
}

if (!f && b) {              // pressing FWD button stops tracker and starts fast forward
  run_flag = 0;
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  while(!digitalRead(FWD) && digitalRead(BACK)){
    move_steps(100,1,56);
  }
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(SLEEP, LOW);
}

if (!b && f) {            // pressing backward button stops tracker and starts fast backward
  run_flag = 0;
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  while(!digitalRead(BACK) && digitalRead(FWD)){
    move_steps(100,0,56);
  }
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(SLEEP, LOW);
}
if (run_flag) move_steps(100,1,steps_int);

}
