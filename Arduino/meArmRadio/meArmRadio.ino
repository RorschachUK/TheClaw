/* meArm over serial - York Hackspace May 2014
 * Control a Phenoptix meArm robot arm via commands sent over serial.
 * Commands are sent as string tokens separated by semicolons, and take the
 * following forms:
 * "goto(0,100,50);open;goto(-80,100,140);close;"
 *
 * Pins:
 * Arduino  XinoRF    Base   Shoulder  Elbow    Gripper
 *    GND            Brown     Brown   Brown     Brown
 *     5V              Red       Red     Red       Red
 *     11            Yellow
 *     10                     Yellow
 *      8  Enable
 *      9                             Yellow
 *      6                                       Yellow
 *      0     DIN
 *      1    DOUT
 */
#include "meArm.h"
#include <Servo.h>

int basePin = 11;
int shoulderPin = 10;
int elbowPin = 9;
int gripperPin = 6;

int ledPin = 13;
// Ciseco XinoRF Arduino clone connects serial to radio when pin 8 driven high
int radioEnablePin = 8;

String radioBuffer = "";
String token = "";

meArm arm;

void setup() {
  //Enable the XRF radio
  pinMode(radioEnablePin, OUTPUT);
  digitalWrite(radioEnablePin, HIGH);
  //for feedback
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  
  Serial.begin(9600);
  arm.begin(basePin, shoulderPin, elbowPin, gripperPin);
}

void loop() {
  //Read serial and parse out tokens delimited by semicolons
  if (Serial.available()>0)
  {
    char buffer[30];
    int num=Serial.readBytes(buffer, sizeof(buffer));
    if (num>0)
      buffer[num]=0;
    radioBuffer.concat(String(buffer));        
  }
  token="";
  int pos = radioBuffer.indexOf(';');
  if (pos>0)
  {
    token = radioBuffer.substring(0, pos);
    radioBuffer = radioBuffer.substring(pos+1);
  }
  
  if (token.equals("open")) {
    digitalWrite(ledPin, HIGH);
    arm.openGripper();
    digitalWrite(ledPin, LOW);
  } else if (token.equals("close")) {
    digitalWrite(ledPin, HIGH);
    arm.closeGripper();
    digitalWrite(ledPin, LOW);
  } else if (token.equals("reset")) {
    digitalWrite(ledPin, HIGH);
    arm.goDirectlyTo(0,100,50);
    digitalWrite(ledPin, LOW);
  } else if (token.substring(0,4).equals("goto")) {
    //parse out x - from bracket to first comma
    int comma = token.indexOf(',');
    int openbracket = token.indexOf('(');
    char buf[5]; for(int i=0;i<5;i++) buf[i]=0;
    token.substring(openbracket + 1, comma).toCharArray(buf, comma - openbracket);
    int xNum = atoi(buf);
    //parse y - between the two commas
    for(int i=0;i<5;i++) buf[i]=0;
    int comma2 = token.lastIndexOf(',');
    token.substring(comma + 1, comma2).toCharArray(buf, comma2 - comma);
    int yNum = atoi(buf);
    //parse z - between last comma and close bracket
    for(int i=0;i<5;i++) buf[i]=0;
    int closebracket = token.lastIndexOf(')');
    token.substring(comma2 + 1, closebracket).toCharArray(buf, closebracket - comma2);
    int zNum = atoi(buf);
    if (arm.isReachable(xNum, yNum, zNum) && (yNum > 50)) {
      digitalWrite(ledPin, HIGH);
      arm.goDirectlyTo(xNum, yNum, zNum);
      //arm.gotoPoint(xNum, yNum, zNum);
      //delay(20);
      digitalWrite(ledPin, LOW);
    }
  }
}
