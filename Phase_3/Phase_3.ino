/*******************************************************************************************************************
   Lakeboys_FinalProjectCode
   Final Project Competition Code
   Logan Bryant, Yuya Hoshi, TJ Rutan
   11/2/2019
   List of functions: autonomous_control(), controller_phase(), follow_line(), readLine(), read_signal(),
   moveForward(), moveStop(), moveBackward(), spinLeft(), spinRight(),turnLeft(), turnRight(), pivotLeft(), 
   pivotRight(), ps2Controll(), send_signal()
 ******************************************************************************************************************/

#include <Psx.h>
#include<Servo.h>
#include<IRremote.h>

// Creates the servo variables to call
Servo rightWheel;
Servo leftWheel;

//Defining pins for receivers
#define Receivers 3
#define Lreceiver 10
#define Rreceiver 9
#define mreceiver 7

//Setting up the receivers in an array, putting the codes in an array to access, and the transmitting LED
IRrecv *receivers[Receivers];
decode_results results;
int transmitCodes [7] = {0xB13, 0xC9A, 0xEA9, 0xA19, 0xE1E, 0xF19, 0x5A5};
IRsend irsend; //transmitter goes in pin 3 on

// Define Pins for RGB
#define BLUE A2  //A2on robot, 5 board
#define GREEN A1 //A1 on robot, 6 board
#define RED 5 //A0 on robot, 9 board

// Define the pins for the white line sensors and create a threshold for the white lvl to read
// as white or black and create bools to say if white is true or false
#define lsensor A5
#define rsensor A4
#define msensor A3
const int whitelvl = 700;
bool leftLine;
bool rightLine;
bool middleLine;

// Define pin for the infrared distance reader
#define infraredDistPin A0

// variable to be used with the read signal to make led light for 5 seconds
unsigned long int ledOffTime;

// The data is read for the ps controller
unsigned int data;
Psx psx;
byte count;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // attach servoss and start the program in autonomous moving forward
  rightWheel.attach(12);
  leftWheel.attach(13);
  leftWheel.writeMicroseconds(1600);
  rightWheel.writeMicroseconds(1400);

  // setup each receiver in the array and enable them
  receivers[0] = new IRrecv(Rreceiver);
  receivers[1] = new IRrecv(mreceiver);
  receivers[2] = new IRrecv(Lreceiver);
  for (byte i = 0; i < Receivers; i++)
  {
    receivers[i]->enableIRIn();
  }

  // enable the red pin as PWM output because its not analog
  pinMode(RED, OUTPUT);

  //create the ps4 controller for input
  psx.setupPins(2, 4, 8, 6, 10);

}

void loop() {
  // put your main code here, to run repeatedly:
  
  while (millis() < 45000) 
  {
    autonomous_control();
  }
  
  while (millis() < 135000) // 45 + 90 seconds = 135000total run time for robot
  {
    controller_phase();
  }
  
  if(millis() > 135000)
  {
    moveStop();
  }
}


void autonomous_control()
{
  if (millis() > ledOffTime) {
    analogWrite(RED, 0);
    analogWrite(BLUE, 0);
    analogWrite(GREEN, 0);
  }
  
  readLine();
  follow_line();
}

void controller_phase()
{
  if (millis() > ledOffTime) {
    analogWrite(RED, 0);
    analogWrite(BLUE, 0);
    analogWrite(GREEN, 0);
  }
  data = psx.read();
  ps2Controll();

  count = 0;
  for (byte i = 0; i < Receivers; i++)
  {
    receivers[i]->enableIRIn();
  }
  while (count < 3)
  {
    read_signal();
    count++;
  }
}

void follow_line() {

  if (analogRead(infraredDistPin) > 575)
  {
    moveStop();
    for (int i = 0; i < 40; i++)
    {
      read_signal();
    }
    spinRight();
    delay(1200);
    moveForward();
    delay(250);
  }
  if (!leftLine && !rightLine && middleLine)
  {
    moveForward();
  }
  
  if (!leftLine && !rightLine && !middleLine)
  {
    moveForward();
  }
  
  if (leftLine && !rightLine && !middleLine)
  {
    turnLeft();
    delay(10);
    moveForward();
  }
  
  if (!leftLine && rightLine && !middleLine)
  {
    turnRight();
    delay(100);
    moveForward();
  }
  
  if (rightLine && middleLine)
  {
    spinRight();
    delay(250);
    moveForward();
    delay(100);
  }
  
  if (leftLine && rightLine && !middleLine)
  {
    spinRight();
    delay(250);
    moveForward();
    delay(100);
  }
  
  if (leftLine && middleLine && !rightLine)
  {
    spinRight();
    delay(275);
    moveForward();
    delay(100);
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
    if (receivers[i]->decode(&results)) // have we received an IR signal?
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
    delay(10);
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

void ps2Controll() {
  switch (data) {
    case psxLeft:
      Serial.println("Left");
      pivotLeft();
      break;
    case psxDown:
      Serial.println("Down");
      moveBackward();
      break;
    case psxRight:
      Serial.println("Right");
      pivotRight();
      break;
    case psxUp:
      Serial.println("Up");
      moveForward();
      break;
    case psxStrt:
      Serial.println("Strt");
      break;
    case psxSqu:
      Serial.println("Squ");
      send_signal();
      break;
    case psxX:
      Serial.println("X");
      send_signal();
      break;
    case psxO:
      Serial.println("O");
      send_signal();
      break;
    case psxTri:
      Serial.println("Tri");
      send_signal();
      break;
    case psxR1:
      Serial.println("R1");
      spinRight();
      break;
    case psxL1:
      Serial.println("L1");
      spinLeft();
      break;
    case psxR2:
      Serial.println("R2");
      turnRight();
      break;
    case psxL2:
      Serial.println("L2");
      turnLeft();
      break;
    default:
      moveStop();
      break;
  }
}

void send_signal() {
  for (int i = 0; i < 5; i++) {
    irsend.sendSony(transmitCodes[6], 12); //Transmit the code 0x5A5 signal from IR LED(change value after 0x to change the code sent)
    delay(40);// this will be used to transmit signal in the head to head stage
  }
}
