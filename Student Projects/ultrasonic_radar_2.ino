#include <Servo.h>

#define TRIG_PIN 7
#define ECHO_PIN 8
#define SERVO_PIN A3
#define BUSSER_PIN 5

#define HCSR04_TIMEOUT 25000
#define LEFT_TONE 200
#define RIGHT_TONE 600
#define STRAIGHT 400
#define THRESHOLD 50

Servo servo;
int angle = 30;
bool forward = true;

const float speedOfSound = 0.0343;

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
pinMode(BUSSER_PIN,OUTPUT);

  servo.attach(SERVO_PIN);
  servo.write(angle);

  Serial.begin(9600);

}


float get_distance(void) {

  long duration = 0;
  float distance = 0;
  // Ultrasonic
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH, HCSR04_TIMEOUT); 
  distance = (duration * speedOfSound) / 2;

  return distance;
}

void loop() {

  // Move servo
  float distance;

  servo.write(angle);

  if (forward) angle++;
  else angle--;

  if (angle >= 150) forward = false;
  if (angle <= 30) forward = true;

  delay(100);

  distance = get_distance();

  Serial.print("Angle = ");
  Serial.print(angle);
  Serial.print(" degrees");
  Serial.print("\tDistance = ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance < THRESHOLD) {
    if (angle < 80){
      tone(BUSSER_PIN, RIGHT_TONE);
    }
    else if (angle > 100){
      tone(BUSSER_PIN, LEFT_TONE);
    }
    else {
      tone(BUSSER_PIN, STRAIGHT);
    }
    
  }
  else {
    noTone(BUSSER_PIN);
  }
}