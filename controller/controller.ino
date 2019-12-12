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
#include <YunClient.h>
#include <YunServer.h>

char command;
int value;
void setup()
{
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

  //Serial.println(digitalRead(A3));
  //Serial.print(digitalRead(A2));
  //float x = digitalRead(A0);
  //Serial.println(x);

  if (Serial.available() > 0)
  {
    // read the incoming bytes
    command = Serial.read();
    value = Serial.parseInt();

    Serial.println(command);
    Serial.println(value);
    Serial.println("----------");


  
    switch (command.charAt(0))
    {
    case 'm':
      int x = value;
      if (x < 0)
      {
        moveBackwards();
        setSpeed(255);
        x = x*-1; 
      }
      else
      {
        moveForward();
        setSpeed(255);
      }
      countUntil(x);
      setSpeed(0);
      break;
    case 'v':
      setSpeed(value);
      break;
    case 's':
      setSpeed(0);
      break;
    default:
      Serial.println("You dum, no such command dude");
      break;
    }
  }
}

void countUntil(int max)
{
  float counter = 0;
  float temp = 0;
  float tempBefore = 0;
  while (counter < value)
  {
   temp = digitalRead(A3);
   if(temp != tempBefore && temp == 1)
   {
     counter++;
   }
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

void moveBackwards()
{ //2
  digitalWrite(7, LOW);
  digitalWrite(8, HIGH);

  digitalWrite(12, LOW);
  digitalWrite(13, HIGH);
}
