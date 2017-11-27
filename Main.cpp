#include <Arduino.h>
#include <NewPing.h>

void findWay();
void standby();
void right();
void left();
void forward(bool back);

int ECHO_PIN            = 12;
int TRIGGER_PIN         = 11;

int MOTOR1_DIR_PIN2     = 7;
int MOTOR1_DIR_PIN1     = 6;

int MOTOR1_ENABLE_PIN   =  5;
int MOTOR2_ENABLE_PIN   = 10;

int MOTOR2_DIR_PIN2     = 9;
int MOTOR2_DIR_PIN1     = 8;

int LED_1               = 2; // Center
int LED_2               = 3; // Right
int LED_3               = 4; // Left

#define TURN_SPEED          250
#define MIN_SPEED           90
#define MAX_SPEED           170
#define MAX_DISTANCE        200
#define MIN_DISTANCE        15
#define RIGHT_MOTOR_FACTOR  1.16

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

float velocity = MIN_SPEED;

float factor = 0.425;

bool firstRun = true;

void setup(){
  Serial.begin(115200);

  pinMode(MOTOR1_ENABLE_PIN, OUTPUT);
  pinMode(MOTOR2_ENABLE_PIN, OUTPUT);
  pinMode(MOTOR1_DIR_PIN1, OUTPUT);
  pinMode(MOTOR1_DIR_PIN2, OUTPUT);
  pinMode(MOTOR2_DIR_PIN1, OUTPUT);
  pinMode(MOTOR2_DIR_PIN2, OUTPUT);

  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);

  factor = ((float)MAX_SPEED - (float)MIN_SPEED) / ((float)MAX_DISTANCE - (float)MIN_DISTANCE);

  standby();

  firstRun = true;

  randomSeed(analogRead(0));
}

void loop(){
  if(firstRun){
    delay(1500);
    digitalWrite(LED_3, HIGH);
    delay(1500);
    digitalWrite(LED_1, HIGH);
    delay(1500);
    digitalWrite(LED_2, HIGH);
    firstRun = false;
  }

  int distance = sonar.ping_cm();

  if(distance==0 || distance > MAX_DISTANCE){
    distance = MAX_DISTANCE;
  }

  velocity = factor * (float)distance;

  if(velocity > MAX_SPEED){
    velocity = MAX_SPEED;
  } else if (velocity < MIN_SPEED){
    velocity = MIN_SPEED;
  }

  if(distance < MIN_DISTANCE){
    findWay();
  }else{
    forward(false);
  }

}

void findWay(){
  int rnd = random(2);

  standby();
  delay(350);
  forward(true);
  delay(350);
  standby();

  int distance = -1;
  while(distance==0 || distance < MIN_DISTANCE){
    if(rnd==0){
      left();
    }else{
      right();
    }

    delay(75);

    standby();

    distance = sonar.ping_cm();

    delay(75);
  }
}

void forward(bool back){
  digitalWrite(LED_1, back? HIGH : LOW);
  digitalWrite(LED_3, back? LOW : HIGH);
  digitalWrite(LED_2, back? LOW : HIGH);

  digitalWrite(MOTOR1_DIR_PIN1, back? HIGH : LOW);
  digitalWrite(MOTOR1_DIR_PIN2, back? LOW : HIGH);
  digitalWrite(MOTOR2_DIR_PIN1, back? HIGH : LOW);
  digitalWrite(MOTOR2_DIR_PIN2, back? LOW : HIGH);

  analogWrite(MOTOR1_ENABLE_PIN, (int)(velocity * RIGHT_MOTOR_FACTOR));
  analogWrite(MOTOR2_ENABLE_PIN, (int)velocity);
  delay(back? 0 : 50);
}

void left(){
  digitalWrite(LED_1, LOW);
  digitalWrite(LED_2, LOW);
  digitalWrite(LED_3, HIGH);

  analogWrite(MOTOR1_ENABLE_PIN, TURN_SPEED);
  analogWrite(MOTOR2_ENABLE_PIN, TURN_SPEED);

  delay(150);

  digitalWrite(MOTOR2_DIR_PIN1, HIGH);
  digitalWrite(MOTOR2_DIR_PIN2, LOW);
  digitalWrite(MOTOR1_DIR_PIN1, LOW);
  digitalWrite(MOTOR1_DIR_PIN2, HIGH);
}

void right(){
  digitalWrite(LED_1, LOW);
  digitalWrite(LED_3, LOW);
  digitalWrite(LED_2, HIGH);

  analogWrite(MOTOR1_ENABLE_PIN, TURN_SPEED);
  analogWrite(MOTOR2_ENABLE_PIN, TURN_SPEED);

  delay(150);

  digitalWrite(MOTOR1_DIR_PIN1, HIGH);
  digitalWrite(MOTOR1_DIR_PIN2, LOW);
  digitalWrite(MOTOR2_DIR_PIN1, LOW);
  digitalWrite(MOTOR2_DIR_PIN2, HIGH);
}

void standby(){
  digitalWrite(LED_1, HIGH);
  digitalWrite(LED_2, HIGH);
  digitalWrite(LED_3, HIGH);

  digitalWrite(MOTOR1_DIR_PIN1, LOW);
  digitalWrite(MOTOR1_DIR_PIN2, LOW);
  digitalWrite(MOTOR2_DIR_PIN1, LOW);
  digitalWrite(MOTOR2_DIR_PIN2, LOW);

  analogWrite(MOTOR1_ENABLE_PIN, 0);
  analogWrite(MOTOR2_ENABLE_PIN, 0);
  delay(20);
}
