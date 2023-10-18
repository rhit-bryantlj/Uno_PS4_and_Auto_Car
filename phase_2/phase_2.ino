#include<Servo.h>
#include<IRremote.h>

Servo rightWheel;
Servo leftWheel;

#define Receivers 3
#define Lreceiver 6
#define Rreceiver 9
#define mreceiver 7

IRrecv *receivers[Receivers];

decode_results results;

#define BLUE A2  //A2on robot, 5 board
#define GREEN A1 //A1 on robot, 6 board
#define RED 5 //A0 on robot, 9 board

unsigned long int ledOffTime;
int distance;
byte num;

#define infraredDistPin A0

#define lsensor A5
#define rsensor A4
#define msensor A3

const int whitelvl = 700;

bool leftLine;
bool rightLine;
bool middleLine;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  rightWheel.attach(12);
  leftWheel.attach(13);
  leftWheel.writeMicroseconds(1600);
  rightWheel.writeMicroseconds(1400);
  receivers[0] = new IRrecv(Rreceiver);
  receivers[1] = new IRrecv(mreceiver);
  receivers[2] = new IRrecv(Lreceiver);

  for (byte i = 0; i < Receivers; i++)
  {
    receivers[i]->enableIRIn();
  }

  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  //read_signal();
  //Serial.print(ledOffTime);
  //Serial.print("\t");
  //Serial.println(millis());
  
  if (millis() > ledOffTime) {
    analogWrite(RED, 0);
    analogWrite(BLUE, 0);
    analogWrite(GREEN, 0);
  }

  readLine();
  follow_line();
}

void follow_line() {

  if (analogRead(infraredDistPin) > 400)
    //if (distance <= 2 && distance > 0)
  {
    moveStop();
    for (int i = 0; i < 20; i++)
    {
      read_signal();
    }
    spinRight();
    delay(1250);
    read_signal();
    moveForward();
    delay(250);
  }
  if (!leftLine && !rightLine && middleLine)
  {
    moveForward();
  }
  //else
  if (!leftLine && !rightLine && !middleLine)
  {
    moveForward();
  }
  //else
  if (leftLine && !rightLine && !middleLine)
  {
    turnLeft();
    delay(10);
    moveForward();
  }
  //else
  if (!leftLine && rightLine && !middleLine)
  {
    turnRight();
    delay(100);
    moveForward();

  }
  //else
  if (rightLine && middleLine)
  {
    spinRight();
    delay(250);
    moveForward();
    delay(50);
  }
  //else
  if (leftLine && rightLine && !middleLine)
  {
    spinRight();
    delay(250);
    moveForward();
    delay(50);
  }
  //else
  if (leftLine && middleLine && !rightLine)
  {
    spinRight();
    delay(300);
    moveForward();
    delay(50);
  }
}

void readLine() {
  if (analogRead(lsensor) > whitelvl) {
    leftLine = false; //left sensor is not on the line
  } else {
    leftLine = true;  //left sensor is on the line
  }

  if (analogRead(rsensor) > whitelvl) {
    rightLine = false;
  } else {
    rightLine = true;
  }

  if (analogRead(msensor) > whitelvl) {
    middleLine = false;
  } else {
    middleLine = true;
  }

}

void read_signal() {
  for (int i = 0; i < Receivers; i++) {
    if (receivers[i]->decode(&results)) // have we received an IR signal? (taken from Elegoo tutorial)
    {

      Serial.println(results.value, HEX); //prints the value of
      switch (results.value)
      {
        case 0xB13: //water
          analogWrite(RED, 0);
          analogWrite(BLUE, 128);
          analogWrite(GREEN, 64);
          ledOffTime = millis() + 5000;
          break;
        case 0xC9A: //grass
          analogWrite(RED, 0);
          analogWrite(BLUE, 0);
          analogWrite(GREEN, 255);
          ledOffTime = millis() + 5000;
          break;
        case 0xEA9: //earth
          analogWrite(RED, 128);
          analogWrite(BLUE, 128);
          analogWrite(GREEN, 20);
          ledOffTime = millis() + 5000;
          break;
        case 0xA19:// air
          analogWrite(RED, 255);
          analogWrite(BLUE, 255);
          analogWrite(GREEN, 255);
          ledOffTime = millis() + 5000;
          break;
        case 0xE1E://electric
          analogWrite(RED, 128);
          analogWrite(BLUE, 0);
          analogWrite(GREEN, 128);
          ledOffTime = millis() + 5000;
          break;
        case 0xF19: // Fire
          analogWrite(RED, 255);
          analogWrite(BLUE, 0);
          analogWrite(GREEN, 55);
          ledOffTime = millis() + 5000;
          break;
        case 0x5A5: // Hit
          analogWrite(RED, 128);
          analogWrite(BLUE, 0);
          analogWrite(GREEN, 0);
          ledOffTime = millis() + 5000;
          break;
      }
      receivers[i]->resume(); // receive the next value
      
    }
    delay(20);
  }
}

void moveForward()
{
  rightWheel.writeMicroseconds(1400);
  leftWheel.writeMicroseconds(1600);
}

void moveStop() {
  rightWheel.writeMicroseconds(1500);
  leftWheel.writeMicroseconds(1500);
}

void moveBackward()
{
  rightWheel.writeMicroseconds(1700);
  leftWheel.writeMicroseconds(1300);
}

void spinLeft()
{
  rightWheel.writeMicroseconds(1300);
  leftWheel.writeMicroseconds(1300);
}

void spinRight()
{
  rightWheel.writeMicroseconds(1700);
  leftWheel.writeMicroseconds(1700);
}

void turnLeft()
{
  rightWheel.writeMicroseconds(1300);
  leftWheel.writeMicroseconds(1525);
}

void turnRight()
{
  rightWheel.writeMicroseconds(1475);
  leftWheel.writeMicroseconds(1700);
}

void pivotLeft()
{
  rightWheel.writeMicroseconds(1300);
  leftWheel.writeMicroseconds(1500);
}

void pivotRight()
{
  leftWheel.writeMicroseconds(1700);
  rightWheel.writeMicroseconds(1500);
}
