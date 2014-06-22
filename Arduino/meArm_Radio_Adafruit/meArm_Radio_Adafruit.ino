/* meArm over serial - York Hackspace May 2014
 * Control a Phenoptix meArm robot arm via commands sent over serial.
 * Commands are sent as string tokens separated by semicolons, and take the
 * following forms:
 * "goto(0,100,50);open;goto(-80,100,140);close;"
 *
 * Pins:
 * Arduino  XinoRF    PWMServo  
 *    GND     GND         GND      
 *     5V     VCC    VCC & V+
 *     A4                 SDA
 *     A5                 SCL
 *      8  Enable
 *      0     DIN
 *      1    DOUT
 *
 * The servos attach to the first block of four servo connections on
 * the Adafruit board, brown wire at the bottom, yellow wire at the top.
 * Adafruit    Servo
 *       0      Base
 *       1  Shoulder (right)
 *       2     Elbow (left)
 *       3   Gripper
 *
 * You can attach to a different block of four by changing the 'begin' call
 * to specify a block 0-3, e.g. to use the second block call arm.begin(1);
 * - to mirror movements to all 4 blocks, call arm.begin(-1);
 */
#include "meArm_Adafruit.h"
#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

// Ciseco XinoRF Arduino clone connects serial to radio when pin 8 driven high
int radioEnablePin = 8;

String radioBuffer = "";
String token = "";

meArm arm;
Adafruit_PCD8544 nokia = Adafruit_PCD8544(2, 3, A2, A1, A0);

void setup() {
  //Enable the XRF radio
  pinMode(radioEnablePin, OUTPUT);
  digitalWrite(radioEnablePin, HIGH);
  //for feedback
  nokia.begin();
  nokia.setContrast(150);
  nokia.clearDisplay(); // show splashscreen
  delay(50);
  nokia.setTextSize(1);
  nokia.setTextColor(BLACK);
  nokia.setCursor(0,0);
  nokia.println("Ready!");
  nokia.display();

  Serial.begin(115200);
  arm.begin();
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
  } else {
    radioBuffer = "";
    Serial.flush();
  }
  token="";
  int pos = radioBuffer.indexOf(';');
  if (pos>0)
  {
    token = radioBuffer.substring(0, pos);
    radioBuffer = radioBuffer.substring(pos+1);
    nokia.clearDisplay();
    nokia.setCursor(0,0);
    nokia.print("token='");
    nokia.print(token);
    nokia.println("'");
    nokia.display();
  }
  
  if (token.equals("open")) {
    arm.openGripper();
  } else if (token.equals("close")) {
    arm.closeGripper();
  } else if (token.equals("reset")) {
    arm.goDirectlyTo(0,100,50);
    arm.openGripper();
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
    if (arm.isReachable(xNum, yNum, zNum)) {
      arm.goDirectlyTo(xNum, yNum, zNum);
      //arm.gotoPoint(xNum, yNum, zNum);
      //delay(20);
    }
  }
  nokia.setCursor(0,40);
  nokia.print("(");
  nokia.print(int(arm.getX()));
  nokia.print(",");
  nokia.print(int(arm.getY()));
  nokia.print(",");
  nokia.print(int(arm.getZ()));
  nokia.print(")");
  nokia.display();
}
