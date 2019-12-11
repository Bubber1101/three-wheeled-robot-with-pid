int incomingByte = 0;
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

 

 Serial.begin(9600);
}

void loop() {

//Serial.println(digitalRead(A3));
//Serial.print(digitalRead(A2));
//float x = digitalRead(A0);
//Serial.println(x);

if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();

    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingByte);

    if(incomingByte == 48){
      moveForward();
      delay(500);
      stopMoving(); 
    }
    if(incomingByte == 49){
moveBackwards();
delay(500);
      stopMoving(); 
    }
  }

//moveForward();
// delay(500);
// moveBackwards();
//  delay(500);
// stopMoving(); 
//   delay(1500);

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
