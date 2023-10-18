/*
   Author: Yuya
   you need to connect pins 2,4,Ground,5V,5,6 from brown wire in a row.
   Turn on ps2 controller, and wait a second to connect automatically.
*/

#include <Psx.h>
#include<Servo.h>
#include<IRremote.h>

#define irRecvPin 5

int transmitCodes [7] = {0xB13, 0xC9A, 0xEA9, 0xA19, 0xE1E, 0xF19, 0x5A5};

unsigned int data;
Psx psx;
Servo rightWheel;
Servo leftWheel;

IRsend irsend; //transmitter goes in pin 3 on
int receiver = 7; // ir receiver pin for transmitter

IRrecv irRecv(irRecvPin); //Adds the IR receiver
decode_results IRresults;

IRrecv irrecv(receiver); //IR receiver for tranmitter on front
decode_results results;

// Define Pins for RGB
#define BLUE A2  //A2on robot, 5 board
#define GREEN A1 //A1 on robot, 6 board
#define RED 5 //5 on robot, 9 board

byte count;
byte num;

void setup() {
  irRecv.enableIRIn();
  psx.setupPins(2, 4, 8, 9, 10);
  rightWheel.attach(12);
  leftWheel.attach(13);
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
  //irControl();
  //delay(1);

  //send_signal();
  count = 0;
  irrecv.enableIRIn();
  while (count < 3)
  {
    read_signal();
    count++;
  }
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
      send_signal();
      break;
    case psxO:
      Serial.println("O");
      break;
    case psxTri:
      Serial.println("Tri");
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
      //Serial.println("NOTHING");
      moveStop();
      break;
  }
}

void irControl() {
  if (irRecv.decode(&IRresults)) {        //These four lines read the signals and print to the serial monitor
    Serial.println(IRresults.value, HEX);
    long irVal = IRresults.value;
    irRecv.resume();

    if (irVal == 0xFF22DD) {
      turnLeft();
    } else if (irVal == 0xFFC23D) {
      turnRight();
    } else if (irVal == 0xFFA857) {
      moveForward();
    } else if (irVal == 0xFF629D) {
      moveBackward();
    } else if (irVal == 0xFFA25D) {
      spinLeft();
    } else if (irVal == 0xFFE21D) {
      spinRight();
    } else if (irVal == 0xFFE01F) {
      pivotLeft();
    } else if (irVal == 0xFF906F) {
      pivotRight();
    } else if (irVal == 0xFF02FD) {
      moveStop();
    }
  }
}

void moveForward()
{
  rightWheel.writeMicroseconds(1300);
  leftWheel.writeMicroseconds(1700);
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
        num = 0;
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
        num = 2;
        break;
      case 0xA19: // air
        analogWrite(RED, 255);
        analogWrite(BLUE, 255);
        analogWrite(GREEN, 255);
        num = 3;
        break;
      case 0xE1E: //electric
        analogWrite(RED, 128);
        analogWrite(BLUE, 0);
        analogWrite(GREEN, 128);
        num = 4;
        break;
      case 0xF19: // Fire
        analogWrite(RED, 255);
        analogWrite(BLUE, 0);
        analogWrite(GREEN, 55);
        num = 5;
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

void send_signal() {
  for (int i = 0; i < 3; i++) {
    irsend.sendSony(transmitCodes[6], 12); //Transmit the code 0x5A5 signal from IR LED(change value after 0x to change the code sent)
    delay(60);                  // this will be used to transmit signal in the head to head stage
  }
}
