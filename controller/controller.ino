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

//commands
const char MOVE = 'm';
const char SET_VELOCITY = 'v';
const char STOP = 's';
const char ROTATE = 'r';
const char SHARP = 'i';
const char SONAR = 'b';
const char FUZZY = 'f';
const char PID = 'p';

int FUZZY_SET_SIZE = 8;

float CLOSE_DIST[][8] = {{1, 0.5, 0.2, 0, 0, 0, 0, 0}, {30, 40, 50, 60, 70, 80, 90, 100}};

float MED_DIST[][8] = {{0, 0, 0.5, 1, 0.5, 0, 0, 0}, {30, 40, 50, 60, 70, 80, 90, 100}};

float FAR_DIST[][8] = {{0, 0, 0, 0, 0, 0.2, 0.5, 1}, {30, 40, 50, 60, 70, 80, 90, 100}};

float LOW_VEL[][8] = {{1, 0.5, 0.2, 0, 0, 0, 0, 0}, {100, 122, 144, 166, 188, 210, 232, 254}};

float MED_VEL[][8] = {{0, 0, 0, 0.5, 1, 0.5, 0, 0}, {100, 122, 144, 166, 188, 210, 232, 254}};

float HIGH_VEL[][8] = {{0, 0, 0, 0, 0, 0.2, 0.5, 1}, {100, 122, 144, 166, 188, 210, 232, 254}};

float MIN_LOW_VEL[] = {0, 0, 0, 0, 0, 0, 0, 0};

float MIN_MED_VEL[] = {0, 0, 0, 0, 0, 0, 0, 0};

float MIN_HIGH_VEL[] = {0, 0, 0, 0, 0, 0, 0, 0};

float MAX_VEL[][8] = {{0, 0, 0, 0, 0, 0, 0, 0}, {100, 122, 144, 166, 188, 210, 232, 254}};


const int CIRCLE = 2 * PI * 13;

const double kp = 1.7;

const double ki = 0.0007;

const double kd = 50;



void setup()
{
  //sonar
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

      case FUZZY:
        runFuzzy();
        break;

      case PID:
        runPid(value);
        break;

      default:
        Serial.println("You dum, no such command dude");
        break;
    }
  }
}


void runPid(int val) {
  moveForward();
  double lastMillis = millis();
  double counterL = 0;
  double counterR = 0;
  int tempL = 0;
  int tempR = 0;
  int tempLBefore = 0;
  int tempRBefore = 0;

  double speedL = 0;
  double speedR = 0;

  double errorL = 0;
  double cumErrorL = 0;
  double lastErrorL = 0;
  double rateErrorL = 0;


  double errorR = 0;
  double cumErrorR = 0;
  double lastErrorR = 0;
  double rateErrorR = 0;

  double dt = 1000;

  while (true) {

    while (millis() - lastMillis < dt) {
      tempR = digitalRead(A2); //prawe
      tempL = digitalRead(A3); //lewe
      if (tempL != tempLBefore && tempL == 1)
      {
        counterL++;
      }
      if (tempR != tempRBefore && tempR == 1)
      {
        counterR++;
      }
      tempLBefore = tempL;
      tempRBefore = tempR;
      //      delay(1);
    }
    lastMillis = millis();

    errorR = val - counterR;
    cumErrorR += errorR * dt;
    rateErrorR = (errorR - lastErrorR) / dt;
    double outR = (kp * errorR + ki * cumErrorR + kd * rateErrorR );

    errorL = val - counterL;
    cumErrorL += errorL * dt;
    rateErrorL = (errorL - lastErrorL) / dt;
    double outL = (kp * errorL + ki * cumErrorL + kd * rateErrorL );
      Serial.print(counterR);
      Serial.print(" ");
      Serial.print(outR);
      Serial.print("||");
      Serial.print(counterL);
      Serial.print(" ");
      Serial.println(outL);


    lastErrorL = errorL;
    lastErrorR = errorR;
    counterL = 0;
    counterR = 0;
    analogWrite(11, outL);
    analogWrite(10, outR);//Right
  }
}




void runFuzzy() {
  float distance = readSharp();
  float close_ign, med_ign, far_ign;
  int index;
  moveForward();

  while (distance > 30) {
    distance = readSharp();
    index = getIndex(distance);
    close_ign = CLOSE_DIST[0][index];
    med_ign = MED_DIST[0][index];
    far_ign = FAR_DIST[0][index];
    setMinArr(close_ign, LOW_VEL[0], MIN_LOW_VEL);
    setMinArr(med_ign, MED_VEL[0], MIN_MED_VEL);
    setMinArr(far_ign, HIGH_VEL[0], MIN_HIGH_VEL);
    setMaxArr();
    setSpeed(getGravityCenter());
  }
  setSpeed(0);
}

void setMaxArr() {
  for (int i = 0; i < FUZZY_SET_SIZE ; i++) {
    MAX_VEL[0][i] = maxOfThree(MIN_LOW_VEL[i], MIN_MED_VEL[i], MIN_HIGH_VEL[i]);
  }
}

void printArr(float arr[]) {
  Serial.print("[");
  for (int i = 0; i < FUZZY_SET_SIZE ; i++) {
    Serial.print(arr[i]);
    Serial.print(",");
  }
  Serial.println("]");
}

float getGravityCenter() {
  float denominator = 0;
  float numerator = 0;

  for (int i = 0; i < FUZZY_SET_SIZE ; i++) {
    numerator += MAX_VEL[0][i] * MAX_VEL[1][i];
    denominator += MAX_VEL[0][i];
  }

  return numerator / denominator;
}

float maxOfThree(float a, float b, float c) {
  return max(a, max(b, c));
}

void setMinArr(float ign, float arr[], float minArr[]) {
  for (int i = 0; i < FUZZY_SET_SIZE ; i++) {
    minArr[i] = min(ign, arr[i]);
  }
}

int getIndex(float dist) {
  for (int i = 0; i < FUZZY_SET_SIZE - 1; i++) {
    if (CLOSE_DIST[1][i] < dist && CLOSE_DIST[1][i + 1] > dist) {
      return i;
    }
  }
  return FUZZY_SET_SIZE - 1;
}

float readSharp() {
  float sensorValue = (float)analogRead(A0);
  float outputValue = sensorValue / 1023.0 * 3.3;
  return (70.0 / outputValue) - 6.5;
}

float readSonar() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);
  long  duration = pulseIn(echoPin, HIGH);

  //Calculate the distance (in cm) based on the speed of sound.
  float dist = duration / 58.2;
  return dist;
}
void setVelocity(int x ) {
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

void rotate(int x) {
  int route = CIRCLE * x / 360;
  int pin = 0;
  if (x < 0) {
    route = route * -1;
    setToRotateLeft();
    setSpeed(255);
    pin = A2;
  } else {
    setToRotateRight();
    setSpeed(255);
    pin = A3;
  }
  countUntil(route, pin);
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
  countUntil(x, A3);
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
  analogWrite(10, val); //prawo
  analogWrite(11, val); //lewy
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

void setToRotateLeft() {
  digitalWrite(7, HIGH);
  digitalWrite(8, LOW);

  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
}

void setToRotateRight() {
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
