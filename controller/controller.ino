#include <Bridge.h>
#include <BridgeClient.h>
#include <BridgeServer.h>
#include <BridgeSSLClient.h>
#include <BridgeUdp.h>
#include <Console.h>
#include <FileIO.h>
#include <HttpClient.h>
#include <Mailbox.h>
#include <Process.h>

#define echoPin A5
#define trigPin A4 

char command;
int value;


const char MOVE = 'm';
const char SET_VELOCITY = 'v';
const char STOP = 's';
const char ROTATE = 'r';
const char SHARP = 'i';
const char SONAR = 'b';

const int CIRCLE = 2 * PI * 13;

void setup()
{//sonar
   pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);
  
  pinMode(10, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(11, OUTPUT);

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);

  digitalWrite(7, HIGH);
  digitalWrite(8, LOW);
  digitalWrite(12, HIGH);
  digitalWrite(13, LOW);

  Serial.begin(9600);
}

void loop()
{
  if (Serial.available() > 0)
  {
    // read the incoming bytes
    command = (char) Serial.read();
    value = Serial.parseInt();

    Serial.println(command);
    Serial.println(value);
    Serial.println("----------");

    switch (command)
    {
      case MOVE:
        move(value);
        Serial.println("Moving");
        break;
      case SET_VELOCITY:
       setVelocity(value);
        
        break;
      case STOP:
        Serial.println("Stopping");
        setSpeed(0);
        break;
      case ROTATE:
        Serial.println("Rotating");
        rotate(value);
        break;
      case SHARP:
      Serial.println(readSharp());
      break;
      case SONAR:
      Serial.println(readSonar());
      break;
      default:
        Serial.println("You dum, no such command dude");
        break;
    }
  }
}

float readSharp(){
  float sensorValue = (float)analogRead(A0);            
  float outputValue = sensorValue/1023.0*3.3;
  return (70.0/outputValue)-6.5;           
}

float readSonar(){
digitalWrite(trigPin, LOW); 
 delayMicroseconds(2); 

 digitalWrite(trigPin, HIGH);
 delayMicroseconds(10); 
 
 digitalWrite(trigPin, LOW);
long  duration = pulseIn(echoPin, HIGH);
 
 //Calculate the distance (in cm) based on the speed of sound.
float dist = duration/58.2;
return dist;
}
void setVelocity(int x ){
  if (x < 0)
  {
    moveBackwards();
    x = x * -1;
    setSpeed(x);
  }
  else
  {
    moveForward();
    setSpeed(x);
  }
}

void rotate(int x){
  int route = CIRCLE*x/360;
  int pin = 0;
  if(x <0){
    route = route*-1;
    setToRotateLeft();
    setSpeed(255);
    pin = A2;
  }else{
    setToRotateRight();
    setSpeed(255);
    pin = A3;
  }
  countUntil(route,pin);
  setSpeed(0);
}

void move(int x) {
  if (x < 0)
  {
    moveBackwards();
    setSpeed(255);
    x = x * -1;
  }
  else
  {
    moveForward();
    setSpeed(255);
  }
  countUntil(x,A3);
    setSpeed(0);

}

void countUntil(int max, int pin)
{
  float counter = 0;
  float temp = 0;
  float tempBefore = 0;
  while (counter < max)
  {
    temp = digitalRead(pin);
    if (temp != tempBefore && temp == 1)
    {
      counter++;
    }
    tempBefore = temp;
  }

}

void setSpeed(int val)
{
  analogWrite(10, val);
  analogWrite(11, val);
}

void stopMoving()
{ //0
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(10, LOW);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  digitalWrite(11, LOW);
}
void moveForward()
{ //1
  digitalWrite(7, HIGH);
  digitalWrite(8, LOW);

  digitalWrite(12, HIGH);
  digitalWrite(13, LOW);
}

void setToRotateLeft(){
    digitalWrite(7, HIGH);
  digitalWrite(8, LOW);

  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
}

void setToRotateRight(){
    digitalWrite(7, LOW);
  digitalWrite(8, LOW);

  digitalWrite(12, HIGH);
  digitalWrite(13, LOW);
}

void moveBackwards()
{ //2
  digitalWrite(7, LOW);
  digitalWrite(8, HIGH);

  digitalWrite(12, LOW);
  digitalWrite(13, HIGH);
}
