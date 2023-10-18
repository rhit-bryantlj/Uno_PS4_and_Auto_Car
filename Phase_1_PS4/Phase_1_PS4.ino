/*
   This Sketch will be used to demonstrate the Phase 1 test
*/

#include <Psx.h>
#include<Servo.h>
#include<IRremote.h>

IRsend irsend; //transmitter goes in pin 3 on

int receiver = 7;

IRrecv irrecv(receiver);
decode_results results;

int transmitCodes [7] = {0xB13, 0xC9A, 0xEA9, 0xA19, 0xE1E, 0xF19, 0x5A5};

unsigned int data;
Psx psx;
Servo rightWheel;
Servo leftWheel;

// Define Pins for RGB
#define BLUE A2  //A2on robot, 5 board
#define GREEN A1 //A1 on robot, 6 board
#define RED A0 //A0 on robot, 9 board

byte count;
byte num = 6; // num for elemental

void setup() {
  psx.setupPins(2, 4, 8, 9, 10);
  rightWheel.attach(13);
  leftWheel.attach(12);
  Serial.begin(9600);
  rightWheel.writeMicroseconds(1500);
  leftWheel.writeMicroseconds(1500);
  irrecv.enableIRIn();
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  data = psx.read();
  ps2Controll();

  count = 0;
  send_signal();

  irrecv.enableIRIn();
  while (count < 3)
  {
    read_signal();
    count++;
  }
}

void read_signal() {
  if (irrecv.decode(&results)) // have we received an IR signal? (taken from Elegoo tutorial)
  {
    Serial.print("Receiver code: ");
    Serial.println(results.value, HEX); //prints the value of

    switch (results.value)
    {
      case 0xB13: //water
        analogWrite(RED, 0);
        analogWrite(BLUE, 128);
        analogWrite(GREEN, 64);
        num =0;
        break;
      case 0xC9A: //grass
        analogWrite(RED, 0);
        analogWrite(BLUE, 0);
        analogWrite(GREEN, 255);
        num = 1;
        break;
      case 0xEA9: //earth
        analogWrite(RED, 128);
        analogWrite(BLUE, 128);
        analogWrite(GREEN, 20);
        num=2;
        break;
      case 0xA19: // air
        analogWrite(RED, 255);
        analogWrite(BLUE, 255);
        analogWrite(GREEN, 255);
        num=3;
        break;
      case 0xE1E: //electric
        analogWrite(RED, 128);
        analogWrite(BLUE, 0);
        analogWrite(GREEN, 128);
        num=4;
        break;
      case 0xF19: // Fire
        analogWrite(RED, 255);
        analogWrite(BLUE, 0);
        analogWrite(GREEN, 55);
        num=5;
        break;
      case 0x5A5: // Hit
        analogWrite(RED, 128);
        analogWrite(BLUE, 0);
        analogWrite(GREEN, 0);
        break;
      default:
        break;
    }
    irrecv.resume(); // receive the next value
  }
  delay(50);
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
      break;
    case psxX:
      Serial.println("X");
      break;
    case psxO:
      Serial.println("O");
      break;
    case psxTri:
      Serial.println("Tri");
      break;
    case psxR1:
      Serial.println("R1");
      pivotRight();
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
      //Serial.println("NOTHING");
      moveStop();
      send_signal();
      break;
  }
}

void moveForward()
{
  rightWheel.writeMicroseconds(1700);
  leftWheel.writeMicroseconds(1300);
}

void moveStop() {
  rightWheel.writeMicroseconds(1500);
  leftWheel.writeMicroseconds(1500);
}

void moveBackward()
{
  rightWheel.writeMicroseconds(1300);
  leftWheel.writeMicroseconds(1700);
}

void spinLeft()
{
  rightWheel.writeMicroseconds(1700);
  leftWheel.writeMicroseconds(1700);
}

void spinRight()
{
  rightWheel.writeMicroseconds(1300);
  leftWheel.writeMicroseconds(1300);
}

void turnLeft()
{
  rightWheel.writeMicroseconds(1700);
  leftWheel.writeMicroseconds(1475);
}

void turnRight()
{
  rightWheel.writeMicroseconds(1525);
  leftWheel.writeMicroseconds(1300);
}

void pivotLeft()
{
  rightWheel.writeMicroseconds(1700);
}

void pivotRight()
{
  leftWheel.writeMicroseconds(1300);
}

void send_signal() {
  for (int i = 0; i < 3; i++) {
    irsend.sendSony(transmitCodes[num], 12); //Transmit the code 0x5A5 signal from IR LED(change value after 0x to change the code sent)
    delay(60);                  // this will be used to transmit signal in the head to head stage
  }
}
