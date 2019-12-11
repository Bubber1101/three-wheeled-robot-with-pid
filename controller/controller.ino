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

String input;
void setup() {
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

void loop() {

//Serial.println(digitalRead(A3));
//Serial.print(digitalRead(A2));
//float x = digitalRead(A0);
//Serial.println(x);

if (Serial.available() > 0) {
    // read the incoming bytes
    input = Serial.readString();
    String command = input.substring(0,1);
    String value = input.substring(1, input.length());
    

    Serial.println(input);
    Serial.println(command);
    Serial.println(value);
    Serial.println("----------");
    if(command == "v"){
     setSpeed(value.toInt());
    }
     if(command == "s"){
     setSpeed(0);
     }

    

}

}

void setSpeed(int val){
  analogWrite(10,val);
  analogWrite(11,val);
}

void stopMoving(){ //0
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(10, LOW);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  digitalWrite(11, LOW);
}
void moveForward() { //1
  digitalWrite(7, HIGH);
  digitalWrite(8, LOW);
  digitalWrite(10, HIGH);
  
    digitalWrite(12, HIGH);
  digitalWrite(13, LOW);
  digitalWrite(11, HIGH);
}

void moveBackwards() { //2
  digitalWrite(7, LOW);
  digitalWrite(8, HIGH);
  digitalWrite(10, HIGH);
  
    digitalWrite(12, LOW);
  digitalWrite(13, HIGH);
  digitalWrite(11, HIGH);
}
